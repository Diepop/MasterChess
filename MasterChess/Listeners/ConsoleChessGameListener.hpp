#pragma once
#include "MasterChess/ChessPlayer.hpp"
#include "MasterChess/IPiece.hpp"
#include "MasterChess/Inputs/TwoStepsInput.hpp"

namespace Unity {
    struct UnityGameListener;
}

namespace MasterChess
{
    using MasterChess::IPiece;
    using Math::Area;
    using Math::Vector2Int;

    struct ChessGame;
    struct ChessBoard;
    struct IConsole;

    struct ConsoleChessGameListener final : IGameListener, TwoStepsInput
    {
        explicit ConsoleChessGameListener(IConsole* console, const Vector2Int& size = {12, 7});

        void OnPlayerColorChange(IPlayer* player) override;

        void OnGameStart(Game* game) override;

        void OnMovementExecution(IMovement* movement) override;

        Game* CurrentGame() override;

        IPiece* SelectPiece(IPlayer* currentPlayer) override;

        Vector2Int SelectPosition(Area area) override;

        void PrintBoard() const;

        void SelectPosition();

        std::unique_ptr<IPiece> SelectPromotion(IPlayer* player) override;

        Unity::UnityGameListener* UnityListener = nullptr;

    private:
        IConsole* console;
        Vector2Int size;
        Vector2Int cursor;
        ChessGame* game;
        ChessBoard* board;
        Area colored;
    };

}
