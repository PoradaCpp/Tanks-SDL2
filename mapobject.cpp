#include <iostream>

#include "mapobject.h"

const RelativeRect MapObject::DEFAULT_TILE_SIZE( 0,   0,   3.0, 5.4  );
const RelativeRect MapObject::TILES_BASE_RECT  ( 1.3, 2.3, 84,  95.4 );


MapObject::MapObject( Texture texture, Renderer renderer ): m_Texture( texture ), m_Renderer( renderer ), m_TileRect({ 0, 0, 0, 0 }),
    m_nTileNumber( TILE_DEFAULT_NUMBER )
{
    m_Texture.setRelativeDestination( DEFAULT_TILE_SIZE, TILES_BASE_RECT );
    m_TileRect = m_Texture.getDestination();
}

MapObject::~MapObject() {}

void MapObject::setSourceRectNumber( size_t nTileNumber )
{
    m_nTileNumber = nTileNumber;
}

size_t MapObject::getTileNumber() const
{
    return m_nTileNumber;
}

void MapObject::setRelativePosition( double dX, double dY )
{
    m_Texture.setRelativePosition( dX, dY );
    m_TileRect = m_Texture.getDestination();
}

void MapObject::setDestination( SDL_Rect DestRect )
{
    m_Texture.setDestination( DestRect );
    m_TileRect = DestRect;
}

SDL_Rect MapObject::getDestination() const
{
    return m_Texture.getDestination();
}

void MapObject::changeSize()
{
    m_Texture.changeSize();
    m_TileRect = m_Texture.getDestination();
}

void MapObject::renderTexture( SDL_Rect SourceRect )
{
    m_Texture.render( &SourceRect );
}

void MapObject::renderFillRect()
{
    SDL_SetRenderDrawColor( m_Renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a );
    SDL_RenderFillRect( m_Renderer, &m_TileRect );
}
