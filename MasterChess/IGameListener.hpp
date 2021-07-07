#pragma once

namespace MasterChess
{
    struct Game;
    struct IGameListener
    {
        /// <summary>
        /// Fun��o de evento executada pela classe "Game" quando o jogo estiver prestes a come�ar.
        /// </summary>
        /// <param name="game">Inst�ncia do jogo</param>
        virtual void OnGameStart(Game* game) = 0;
    };
}
