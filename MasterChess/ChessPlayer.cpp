#include "ChessPlayer.hpp"
#include "ChessGame.hpp"

#include <cassert>

namespace MasterChess
{
    void ChessPlayer::SetColor(uint32_t color)
    {
        assert(game && "Call OnGameStart!");
        this->color = color;
        game->NotifyPlayerColorChange(this);
    }

    void ChessPlayer::OnGameStart(Game* game)
    {
        assert(dynamic_cast<ChessGame*>(game));
        this->game = static_cast<ChessGame*>(game);
    }
}
