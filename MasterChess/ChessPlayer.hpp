#pragma once
#include "IPlayer.hpp"

namespace MasterChess
{
    struct ChessGame;
    struct IBoard;

    struct ChessPlayer final : IPlayer
    {
        ChessPlayer(IInput* input, int team, uint32_t color) : game(nullptr), input(input), team(team), color(color) {  }
        IInput* Input() const override { return input; }
        int Team() const override { return team; }
        uint32_t Color() const override { return color; }
        void SetColor(uint32_t color) override;
        void OnGameStart(Game* game) override;
    private:
        ChessGame* game;
        IInput* input;
        int team;
        uint32_t color;
    };
}
