#include "buildmap.h"


BuildMapMoveAlgo::BuildMapMoveAlgo(): MoveAlgorithm() {}

BuildMapMoveAlgo::~BuildMapMoveAlgo() {}

void BuildMapMoveAlgo::move( SDL_Rect &CurPos )
{
    int nCursorX(0), nCursorY(0);

    SDL_GetMouseState( &nCursorX, &nCursorY );

    CurPos.x = nCursorX - CurPos.w / 2;
    CurPos.y = nCursorY - CurPos.h / 2;
}



BuildMap::BuildMap( ImgTextureInitData InitData, Renderer renderer ): DynamicObject(), m_Texture( InitData, renderer )
{
    m_pMoveAlgorithm = std::make_shared < BuildMapMoveAlgo > ();

    m_CurPos = m_Texture.getDestination();

    changePosition();
}

BuildMap::~BuildMap() {}

void BuildMap::changePosition()
{
    m_pMoveAlgorithm->move( m_CurPos );

    m_Texture.setDestination( m_CurPos );
}

void BuildMap::changeSize()
{
    m_Texture.changeSize();
    m_CurPos = m_Texture.getDestination();
}

void BuildMap::render()
{
    m_Texture.render();
}
