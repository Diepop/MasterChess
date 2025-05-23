#pragma once
#include <memory>

namespace MasterChess
{
    using std::unique_ptr;

    struct IBoard;
    struct IPlayer;
    struct IMovement;
    struct IPiece;

    /// <summary>
    /// Inteface usada para os inputs dos jogadores.
    /// </summary>
    struct IInput
    {
        /// <summary>
        /// Cria a movimenta��o de uma pe�a.
        /// </summary>
        /// <param name="board"></param>
        /// <param name="player"></param>
        /// <returns></returns>
        virtual unique_ptr<IMovement> CreateMovement(IBoard* board, IPlayer* player) = 0;

        /// <summary>
        /// Seleciona uma pe�a que ser� usada para a promo��o de um pe�o.
        /// </summary>
        /// <param name="player"></param>
        /// <returns></returns>
        virtual unique_ptr<IPiece> SelectPromotion(IPlayer* player) = 0;
    };
}
