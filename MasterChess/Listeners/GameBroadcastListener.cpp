#include "GameBroadcastListener.hpp"

#include <execution>

namespace MasterChess
{
    template<class Range, class Fn, class... Args>
    static void pfor(Range&& rng, Fn fn, Args&&... args)
    {
        for_each(std::execution::par_unseq, std::begin(rng), std::end(rng), [=](IGameListener* listener) { (listener->*fn)(args...); });
    }

    void GameBroadcastListener::OnGameStart(Game* game)
    {
        pfor(listeners, &IGameListener::OnGameStart, game);
    }

    void GameBroadcastListener::OnMovementExecution(IMovement* movement)
    {
        pfor(listeners, &IGameListener::OnMovementExecution, movement);
    }

    void GameBroadcastListener::OnMovementUndo(IMovement* movement)
    {
        pfor(listeners, &IGameListener::OnMovementUndo, movement);
    }

    void GameBroadcastListener::OnGameOver(GameResult* result)
    {
        pfor(listeners, &IGameListener::OnGameOver, result);
    }

    void GameBroadcastListener::OnPlayerColorChange(IPlayer* player)
    {
        pfor(listeners, &IGameListener::OnPlayerColorChange, player);
    }

    void GameBroadcastListener::AddListener(IGameListener* listener)
    {
        listeners.emplace_back(listener);
    }
}
