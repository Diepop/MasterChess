#pragma once
#include "CannonChessPiece.hpp"

namespace MasterChess
{
    /// <summary>
    /// Classe que ser� a representa��o da Dama no jogo.
    /// </summary>
    struct Queen final : CannonChessPiece
    {
        Queen(IPlayer* player, int maxDistance = 8);

        char Id() const override;

    };
}