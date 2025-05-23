#pragma once
#include "IGameListener.hpp"

#include <vector>
#include <memory>
#include <string>

namespace MasterChess
{
    using std::string;
    using std::vector;
    using std::unique_ptr;

    struct IMovement;
    struct ChessPlayer;
    struct IPlayer;
    struct IPiece;
    struct IBoard;

    struct GameResult
    {
        vector<IPlayer*> Winners;
        vector<IPlayer*> Losers;
        IBoard* Board;
        string Message;
    };

    struct Game
    {
        Game();

        Game(unique_ptr<IBoard> board, vector<unique_ptr<IPlayer>> players, vector<unique_ptr<IPiece>> pieces);

        virtual bool ValidateMovement(unique_ptr<IMovement>& movement);

        virtual unique_ptr<GameResult> IsGameOver(IPlayer* currentPlayer);

        GameResult Play();

        IBoard* Board() const { return board.get(); }

        virtual void SetBoard(unique_ptr<IBoard> board);

        IMovement* LastMovement() { return !movements.empty() ? movements.back().get() : nullptr; }

        void AddListener(IGameListener* listener);

        vector<IPlayer*> Players() const;

        IPlayer* Player(int i) const { return players[i].get(); }

        int PlayerCount() const { return (int)players.size(); }

        virtual IPlayer* AddPlayer(unique_ptr<IPlayer> player);

        vector<IPiece*> Pieces() const;

        virtual IPiece* AddPiece(unique_ptr<IPiece> piece);

        int MoveCount() const { return (int)movements.size(); }

        IPlayer* CurrentPlayer() const { return currentPlayer; }

        virtual IPiece* RequestPromotion(IPiece* piece);

        virtual void NotifyPlayerColorChange(IPlayer* player);
        
    protected:

        virtual void ExecuteMovement(unique_ptr<IMovement> movement);

        virtual void UndoLastMovement();

        void SimulateMovementExecution(unique_ptr<IMovement>& movement);

        void SimulateMovementUndo(unique_ptr<IMovement>& movement);

    private:
        unique_ptr<IBoard> board;
        vector<unique_ptr<IPlayer>> players;
        vector<unique_ptr<IPiece>> pieces;
        vector<unique_ptr<IMovement>> movements;
        IPlayer* currentPlayer;
        vector<IGameListener*> listeners;
        bool isSimulating;
    };
    
}
