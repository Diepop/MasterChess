#include "UnityGameListener.hpp"
#include "MasterChess/ChessGame.hpp"
#include "MasterChess/ChessPieces/ChessPiece.hpp"
#include "MasterChess/ChessPieces/King.hpp"
#include "MasterChess/ChessPieces/Pawn.hpp"

#include <format>
#include <google/protobuf/empty.pb.h>

#include <chrono>
#include <thread>
#include <future>
#include <optional>

#include <random>

using namespace UnityRpc;
using namespace std::chrono_literals;
using std::this_thread::sleep_for;
using std::chrono::high_resolution_clock;

namespace Unity
{

    static Point2D convert(const Math::Vector2Int& v)
    {
        Point2D point;
        point.set_x(v.x);
        point.set_y(v.y);
        return point;
    }

    UnityGameListener::UnityGameListener(std::string password) : game(nullptr), gameOver(false), password(move(password))
    {

    }

    UnityGameListener::UnityGameListener() : UnityGameListener([]
    {
        std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count() ^ 0xDEADBEEF);
        std::uniform_int_distribution<uint32_t> dist(1, 0xFFFF);
        return std::format("{:x}", dist(engine));
    }())
    {

    }

    void UnityGameListener::OnGameStart(MasterChess::Game* game)
    {
        assert(dynamic_cast<MasterChess::ChessGame*>(game));
        this->game = static_cast<MasterChess::ChessGame*>(game);
    }

    void UnityGameListener::OnMovementExecution(MasterChess::IMovement* movement)
    {
        assert(dynamic_cast<MasterChess::ChessPiece::Movement*>(movement));
        auto mov = static_cast<MasterChess::ChessPiece::Movement*>(movement);
        auto players = game->Players();
        auto playerIndex = int32_t(distance(players.begin(), find(players.begin(), players.end(), movement->Piece()->Player())));
        ServerMessage msg;
        switch (mov->Piece()->Id())
        {
        case 'p':
        {
            if (auto ptr = dynamic_cast<MasterChess::Pawn::EnPassantMovement*>(mov))
            {
                auto ep = msg.mutable_enpassant();
                ep->set_playerindex(playerIndex);
                *ep->mutable_origin() = convert(ptr->Origin());
                *ep->mutable_destination() = convert(ptr->Destination());
                *ep->mutable_capturedposition() = convert(ptr->Destination() - ptr->Piece()->Direction());
            }
            else if (auto ptr = dynamic_cast<MasterChess::Pawn::PromotionMovement*>(mov))
            {
                auto ep = msg.mutable_promotion();
                ep->set_playerindex(playerIndex);
                *ep->mutable_origin() = convert(ptr->Origin());
                *ep->mutable_destination() = convert(ptr->Destination());
                switch (ptr->PromotedPiece()->Id())
                {
                case 'q': ep->set_piecetype(Queen); break;
                case 'r': ep->set_piecetype(Rook); break;
                case 'b': ep->set_piecetype(Bishop); break;
                case 'k': ep->set_piecetype(Knight); break;
                }
            }
            else goto def;
        }break;
        case 'k':
            if (auto ptr = dynamic_cast<MasterChess::King::CastleMovement*>(mov))
            {
                auto cm = msg.mutable_castle();
                cm->set_playerindex(playerIndex);
                *cm->add_origins() = convert(ptr->Origin());
                *cm->add_destinations() = convert(ptr->Destination());
                *cm->add_origins() = convert(ptr->CastlePieceOrigin());
                *cm->add_destinations() = convert(ptr->CastlePieceDestination());
                break;
            }
        def:
        default:
        {
            auto nm = msg.mutable_normalmovement();
            nm->set_playerindex(playerIndex);
            *nm->mutable_origin() = convert(mov->Origin());
            *nm->mutable_destination() = convert(mov->Destination());
        }break;
        }
        {
            std::lock_guard l(m);
            for (auto stream : streams)
                stream->Write(msg);
        }
    }

    void UnityGameListener::OnMovementUndo(MasterChess::IMovement* movement)
    {
        
    }

    void UnityGameListener::OnGameOver(MasterChess::GameResult* result)
    {
        grpc::ClientContext ctx;
        ServerMessage msg;
        auto req = msg.mutable_result();
        req->set_status(!result->Winners.empty() ? Result_StatusType_CheckMate : Result_StatusType_Draw);

        auto players = game->Players();
        auto& [winners, losers, board, message] = *result;

        
        for (auto player : winners) 
            req->add_winners(int32_t(distance(players.begin(), find(players.begin(), players.end(), player))));
        for (auto player : losers)
            req->add_losers(int32_t(distance(players.begin(), find(players.begin(), players.end(), player))));

        for (auto stream : streams)
            stream->Write(msg, {});
        gameOver = true;
    }

    grpc::Status UnityGameListener::Start(grpc::ServerContext* context, grpc::ServerReaderWriter<ServerMessage, ClientMessage>* stream)
    {
        auto ids = context->peer();

        {
            std::lock_guard l(m);
            contexts.emplace_back(context);
            streams.emplace_back(stream);
            while (!game)
                sleep_for(1ms);
            ServerMessage msg;
            msg.mutable_position()->set_fenstring(game->Fen());
            stream->Write(msg);
        }

        auto changeColor = [=](const ColorChange& cc)
        {
            auto i = cc.player();
            if (i >= 0 && i < game->PlayerCount())
                game->Player(i)->SetColor(cc.color());
        };
        auto read = [=]
        {
            return async(std::launch::async, [=]
            {
                ClientMessage msg;
                return stream->Read(&msg) ? std::make_optional(msg) : std::nullopt;
            });
        };
        bool boardControl = false;
        auto task = read();
        while (!context->IsCancelled() && !gameOver)
        {
            if (task.wait_for(1ms) == std::future_status::ready)
            if (auto msg = task.get())
            {
                switch (msg->command_case())
                {
                case ClientMessage::kColorChange:
                {
                    if (!boardControl) break;
                    changeColor(msg->colorchange());
                }break;
                case ClientMessage::kBoardControl:
                {
                    ServerMessage sm;
                    auto res = sm.mutable_controlresponse();
                    auto& m = msg->boardcontrol();
                    if (boardControl || m.password() == password)
                    {
                        boardControl = true;
                        for (auto& change : m.colorchanges())
                            changeColor(change);
                        res->set_accepted(true);
                    }
                    else res->set_accepted(false);
                    stream->Write(sm);
                }break;
                default: ;
                }
                task = read();
            }
        }

        {
            std::lock_guard l(m);
            stream->WriteLast({}, {});
            streams.erase(find(streams.begin(), streams.end(), stream));
            contexts.erase(find(contexts.begin(), contexts.end(), context));
        }

        return grpc::Status::OK;
    }
}
