#include <iostream>

#include "buildmap.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief BuildMapMoveAlgo::BuildMapMoveAlgo
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BuildMapMoveAlgo::BuildMapMoveAlgo(): MoveAlgorithm() {}

BuildMapMoveAlgo::~BuildMapMoveAlgo() {}

void BuildMapMoveAlgo::move( SDL_Rect &CurPos )
{
    int nCursorX(0), nCursorY(0);

    SDL_GetMouseState( &nCursorX, &nCursorY );

    CurPos.x = nCursorX - CurPos.w / 2;
    CurPos.y = nCursorY - CurPos.h / 2;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief BuildMap::BuildMap
/// \param renderer
/// \param InitData
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BuildMap::BuildMap( Texture texture, Renderer renderer ): DynamicObject(), m_Texture( texture ), m_Renderer( renderer ),
    m_nCurrentSourceRect( Map::VECTOR_TILE_SIZE )
{
    m_pMoveAlgorithm = std::make_shared < BuildMapMoveAlgo > ();
    m_Texture.setRelativeDestination({ 0, 0,  Map::TILE_WIDTH,  Map::TILE_HEIGHT });
    m_CurPos = m_Texture.getDestination();

    setRenderDimensions();
    changePosition();
}

BuildMap::~BuildMap() {}

void BuildMap::setRenderDimensions()
{
    int nWidth = m_Texture.getWidth();
    int nHeight = m_Texture.getHeight();
    int nSrcRectHeight = nHeight / static_cast <int > ( Map::VECTOR_TILE_HEIGHT );
    int nSrcRectWidth = nWidth / static_cast <int > ( Map::VECTOR_TILE_WIDTH );
    int width( 0 ) , height( 0 );

    for( size_t i = 0 ; i < Map::VECTOR_TILE_SIZE; ++i, ++width )
    {
        if( width >= static_cast <int> ( Map::VECTOR_TILE_WIDTH ) )
        {
            width = 0;
            ++height;
        }
        m_SourceRectArr.at( i ) = { nSrcRectWidth * width, nSrcRectHeight * height, nSrcRectWidth, nSrcRectHeight };
    }
}

void BuildMap::setDestRect( SDL_Rect DestRect )
{
    m_DestRect = DestRect;
}

void BuildMap::setSource( size_t nPos )
{
    m_nCurrentSourceRect = nPos;
}

size_t BuildMap::getSourceRectPos()
{
    return m_nCurrentSourceRect;
}

void BuildMap::changePosition()
{
    m_pMoveAlgorithm->move( m_CurPos );
    m_Texture.setPosition( m_CurPos.x, m_CurPos.y );
}

void BuildMap::changeSize()
{
    m_Texture.changeSize();
    m_CurPos = m_Texture.getDestination();
}

void BuildMap::render()
{
    m_Texture.setAlpha( TRANSPARENT );
    m_Texture.render( &m_SourceRectArr.back(), &m_DestRect );
    m_Texture.setAlpha( OPAQUE );

    if( m_nCurrentSourceRect < Map::VECTOR_TILE_SIZE )
    {
        m_Texture.render( &m_SourceRectArr.at( m_nCurrentSourceRect ) );
    }
    SDL_SetRenderDrawColor( m_Renderer,  Map::GREY.r, Map::GREY.g, Map::GREY.b, Map::GREY.a );
    SDL_RenderDrawRect( m_Renderer, &m_CurPos );
}
