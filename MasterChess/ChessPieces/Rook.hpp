#pragma once
#include "CannonChessPiece.hpp"

namespace MasterChess
{
    /// <summary>
    /// Classe que ser� a representa��o da Torre no jogo.
    /// </summary>
    struct Rook final : CannonChessPiece
    {
        Rook(IPlayer* player, int maxDistance = 8);

        char Id() const override;

    };
}