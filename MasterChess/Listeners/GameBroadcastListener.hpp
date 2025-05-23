#pragma once
#include "MasterChess/IGameListener.hpp"

#include <vector>

namespace MasterChess
{
    /// <summary>
    /// Classe que invoca os eventos do jogo de mono assíncrono.
    /// </summary>
    struct GameBroadcastListener : IGameListener
    {
        explicit GameBroadcastListener(std::vector<IGameListener*> listeners = {}) : listeners(std::move(listeners)) {  }
        void OnGameStart(Game* game) override;
        void OnMovementExecution(IMovement* movement) override;
        void OnMovementUndo(IMovement* movement) override;
        void OnGameOver(GameResult* result) override;
        void OnPlayerColorChange(IPlayer* player) override;
        void AddListener(IGameListener* listener);
    private:
        std::vector<IGameListener*> listeners;
    };

}
