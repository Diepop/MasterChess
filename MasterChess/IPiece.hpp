#pragma once
#include "IGameListener.hpp"
#include "IMovement.hpp"
#include "Math/Area.hpp"
#include "Math/Vector2Int.hpp"

#include <memory>
#include <unordered_map>

namespace MasterChess
{
    struct IBoard;
    struct IPlayer;
    struct Game;

    using std::unordered_map;
    using std::unique_ptr;
    using Math::Vector2Int;
    using Math::Area;

    struct MovementMap : unordered_map<Vector2Int, unique_ptr<IMovement>>
    {
        using unordered_map<Vector2Int, unique_ptr<IMovement>>::unordered_map;

        Area MappedArea() const
        {
            Area a;
            for (auto&& [v, m] : *this)
                a |= v;
            return a;
        }

    };

    struct IPiece : IGameListener
    {
        virtual ~IPiece() = default;

        /// <summary>
        /// Jogador que comanda a pe�a.
        /// </summary>
        virtual IPlayer* Player() const = 0;

        /// <summary>
        /// Tabuleiro em que a pe�a se encontra.
        /// </summary>
        virtual IBoard* Board() const = 0;

        /// <summary>
        /// Identificador da pe�a.
        /// </summary>
        /// <returns></returns>
        virtual char Id() const = 0;

        /// <summary>
        /// Posi��o da pe�a.
        /// </summary>
        virtual Vector2Int Position() const = 0;

        /// <summary>
        /// Quantidade de movimentos feito pelas pe�a.
        /// </summary>
        /// <returns></returns>
        virtual int MovementCount() const = 0;

        /// <summary>
        /// Movimentos que podem ser feitos pela pe�a.
        /// </summary>
        /// <returns></returns>
        virtual MovementMap PossibleMovements() = 0;

        virtual void OnAddToBoard(IBoard* board) = 0;

        virtual void OnRemoveFromBoard() = 0;

    };
}
