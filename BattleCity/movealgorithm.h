#ifndef MOVEALGORITHM_H
#define MOVEALGORITHM_H

#include <memory>

#include "SDL.h"
#include "texture.h"

struct RelativeRect;

class MoveAlgorithm
{
public:
    MoveAlgorithm(): m_nDisplayWidth( 0 ) {}
    virtual ~MoveAlgorithm() {}

    virtual void move( SDL_Rect &CurPos ) = 0;
    virtual void changeDirection()
    {

    }
    virtual void setDisplayWidth( uint32_t nDisplayWidth )
    {
        m_nDisplayWidth = nDisplayWidth;
    }

protected:
    uint32_t m_nDisplayWidth;
};

typedef std::shared_ptr < MoveAlgorithm > pSharedMoveAlgo;

#endif // MOVEALGORITHM_H
