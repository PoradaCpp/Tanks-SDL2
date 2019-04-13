#ifndef MOVEALGORITHM_H
#define MOVEALGORITHM_H

#include <memory>

#include "SDL.h"
#include "texture.h"

struct RelativeRect;

class MoveAlgorithm
{
public:
    MoveAlgorithm() {}
    virtual ~MoveAlgorithm() {}

    virtual void move( SDL_Rect &CurPos ) = 0;
    virtual void move( RelativeRect &CurPos )
    {
        CurPos = { 0, 0, 0, 0 };
    }
};

typedef std::shared_ptr < MoveAlgorithm > pSharedMoveAlgo;

#endif // MOVEALGORITHM_H
