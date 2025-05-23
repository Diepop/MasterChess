#pragma once

namespace MasterChess
{
    struct IPlayer;
    struct GameResult;
    struct Game;
    struct IMovement;
    struct IGameListener
    {
        /// <summary>
        /// Evento executado quando o jogo estiver prestes a começar.
        /// </summary>
        /// <param name="game">Instância do jogo</param>
        virtual void OnGameStart(Game* game) {  }

        /// <summary>
        /// Evento executado quando algum movimento é realizado.
        /// </summary>
        /// <param name="movement"></param>
        virtual void OnMovementExecution(IMovement* movement) {  }

        /// <summary>
        /// Evento executado quando algum movimento é desfeito.
        /// </summary>
        /// <param name="movement"></param>
        virtual void OnMovementUndo(IMovement* movement) {  }

        /// <summary>
        /// Executado ao fim de um jogo.
        /// </summary>
        /// <param name="result">Resultado do jogo contento as suas informações.</param>
        virtual void OnGameOver(GameResult* result) {  }

        /// <summary>
        /// Executada quando um dos jogadores troca a sua cor.
        /// </summary>
        /// <param name="player"></param>
        virtual void OnPlayerColorChange(IPlayer* player) {  }
    };    

}
