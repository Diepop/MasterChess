#pragma once

namespace MasterChess
{
    struct IPiece;
    /// <summary>
    /// Abstra��o de um movimento do jogo.
    /// </summary>
    struct IMovement
    {
        /// <summary>
        /// Pe�a que criou a movimenta��o.
        /// </summary>
        /// <returns></returns>
        virtual IPiece* Piece() const = 0;
        /// <summary>
        /// Executa a movimenta��o.
        /// </summary>
        virtual void Execute() = 0;
        /// <summary>
        /// Desfaz a movimenta��o.
        /// </summary>
        virtual void Undo() = 0;
    };
}
