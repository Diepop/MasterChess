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
        /// Cria a movimentação de uma peça.
        /// </summary>
        /// <param name="board"></param>
        /// <param name="player"></param>
        /// <returns></returns>
        virtual unique_ptr<IMovement> CreateMovement(IBoard* board, IPlayer* player) = 0;

        /// <summary>
        /// Seleciona uma peça que será usada para a promoção de um peão.
        /// </summary>
        /// <param name="player"></param>
        /// <returns></returns>
        virtual unique_ptr<IPiece> SelectPromotion(IPlayer* player) = 0;
    };
}
