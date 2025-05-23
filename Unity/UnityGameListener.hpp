#pragma once
#include "MasterChess/IGameListener.hpp"
#include "generated/UnityRpc.grpc.pb.h"

#include <mutex>

namespace MasterChess
{
    struct ChessGame;
}

namespace Unity
{
    struct UnityGameListener final : UnityRpc::UnityRPCBoard::Service, MasterChess::IGameListener
    {
        UnityGameListener(std::string password);
        UnityGameListener();
        void OnGameStart(MasterChess::Game* game) override;
        void OnMovementExecution(MasterChess::IMovement* movement) override;
        void OnMovementUndo(MasterChess::IMovement* movement) override;
        void OnGameOver(MasterChess::GameResult* result) override;
        grpc::Status Start(grpc::ServerContext* context, grpc::ServerReaderWriter<UnityRpc::ServerMessage, UnityRpc::ClientMessage>* stream) override;
        std::string_view Password() const { return password; }
        const auto& Contexts() const { return contexts; }
    private:
        MasterChess::ChessGame* game;
        std::vector<grpc::ServerReaderWriter<UnityRpc::ServerMessage, UnityRpc::ClientMessage>*> streams;
        std::vector<grpc::ServerContext*> contexts;
        mutable std::mutex m;
        bool gameOver;
        std::string password;
    };
}
