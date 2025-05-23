#include "Game.hpp"

#include "IBoard.hpp"
#include "IMovement.hpp"
#include "IPiece.hpp"
#include "IPlayer.hpp"
#include "IInput.hpp"

#include <algorithm>
#include <iterator>

#include <cassert>

namespace MasterChess
{
    Game::Game() : currentPlayer(nullptr), isSimulating(false)
    {

    }

    Game::Game(unique_ptr<IBoard> board, vector<unique_ptr<IPlayer>> players, vector<unique_ptr<IPiece>> pieces) : currentPlayer(nullptr)
    {
        SetBoard(move(board));
        for (auto& player : players)
            AddPlayer(move(player));
        for (auto& piece : pieces)
            AddPiece(move(piece));
    }

    bool Game::ValidateMovement(unique_ptr<IMovement>& movement)
    {
        return true;
    }

    unique_ptr<GameResult> Game::IsGameOver(IPlayer* currentPlayer)
    {
        return nullptr;
    }

    GameResult Game::Play()
    {
        for (auto listener : listeners)
            listener->OnGameStart(this);
        size_t i = 0;
        while (true)
        {
            currentPlayer = players[i].get();
            if (auto result = IsGameOver(currentPlayer))
            {
                for (auto listener : listeners)
                    listener->OnGameOver(result.get());
                return *result;
            }
            unique_ptr<IMovement> movement;
            do movement = currentPlayer->Input()->CreateMovement(board.get(), currentPlayer);
            while (!ValidateMovement(movement));
            ExecuteMovement(move(movement));
            ++i %= players.size();
        }
    }

    void Game::AddListener(IGameListener* listener)
    {
        assert(find(listeners.begin(), listeners.end(), listener) == listeners.end() && "Listener is already listening!");
        listeners.emplace_back(listener);
    }

    vector<IPlayer*> Game::Players() const
    {
        vector<IPlayer*> vec;
        transform(players.begin(), players.end(), back_inserter(vec), [](auto& ptr) { return ptr.get(); });
        return vec;
    }

    IPlayer* Game::AddPlayer(unique_ptr<IPlayer> player)
    {
        if (players.empty())
            currentPlayer = player.get();
        AddListener(player.get());
        return players.emplace_back(move(player)).get();
    }

    vector<IPiece*> Game::Pieces() const
    {
        vector<IPiece*> vec;
        transform(pieces.begin(), pieces.end(), back_inserter(vec), [](auto& ptr) { return ptr.get(); });
        return vec;
    }

    IPiece* Game::AddPiece(unique_ptr<IPiece> piece)
    {
        AddListener(piece.get());
        return pieces.emplace_back(move(piece)).get();
    }

    IPiece* Game::RequestPromotion(IPiece* piece)
    {
        assert(currentPlayer == piece->Player());
        if (isSimulating)
            return nullptr;
        auto ptr = AddPiece(currentPlayer->Input()->SelectPromotion(currentPlayer));
        ptr->OnGameStart(this);
        return ptr;
    }

    void Game::NotifyPlayerColorChange(IPlayer* player)
    {
        for (auto listener : listeners)
            listener->OnPlayerColorChange(player);
    }

    void Game::ExecuteMovement(unique_ptr<IMovement> movement)
    {
        movement->Execute();
        movements.emplace_back(move(movement));
        for (auto listener : listeners)
            listener->OnMovementExecution(LastMovement());
    }

    void Game::UndoLastMovement()
    {
        assert(!movements.empty());
        LastMovement()->Undo();
        auto movement = move(movements.back());
        movements.resize(movements.size() - 1);
        for (auto listener : listeners)
            listener->OnMovementUndo(movement.get());
    }

    void Game::SimulateMovementExecution(unique_ptr<IMovement>& movement)
    {
        isSimulating = true;
        movement->Execute();
        movements.emplace_back(move(movement));
        isSimulating = false;
    }

    void Game::SimulateMovementUndo(unique_ptr<IMovement>& movement)
    {
        assert(!movements.empty());
        isSimulating = true;
        LastMovement()->Undo();
        movement = move(movements.back());
        movements.resize(movements.size() - 1);
        isSimulating = false;
    }

    void Game::SetBoard(unique_ptr<IBoard> board)
    {
        assert(!this->board && board);
        AddListener(board.get());
        this->board = move(board);
    }
    
}
