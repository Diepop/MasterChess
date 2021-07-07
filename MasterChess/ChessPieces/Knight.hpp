#pragma once
#include "MultiPositionChessPiece.hpp"

namespace MasterChess
{
    /// <summary>
    /// Classe que ser� a representa��o da Dama no jogo.
    /// </summary>
    struct Knight final : MultiPositionChessPiece
    {
        Knight(IPlayer* player);

        char Id() const override;

    };
}
