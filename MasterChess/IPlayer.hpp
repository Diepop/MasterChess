#pragma once
#include "IGameListener.hpp"

#include <cstdint>

namespace MasterChess
{
    struct IInput;

    struct IPlayer : IGameListener
    {
        virtual IInput* Input() const = 0;
        virtual int Team() const = 0;
        virtual uint32_t Color() const = 0;
        virtual void SetColor(uint32_t color) = 0;
    };
}
