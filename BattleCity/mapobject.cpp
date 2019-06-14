#include <iostream>
#include <algorithm>
#include <cmath>

#include "mapobject.h"
#include "map.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MapCollisionInfo::static constant RelativeRects:
/// \brief FULL_SIZE, HALF_LEFT_SIZE, HALF_RIGHT_SIZE, HALF_UPPER_SIZE, HALF_DOWN_SIZE, QUARTER_DOWN_RIGHT_SIZE,
/// \brief QUARTER_DOWN_LEFT_SIZE, QUARTER_UPPER_RIGHT_SIZE, QUARTER_UPPER_RIGHT_SIZE, QUARTER_UPPER_LEFT_SIZE, EMPTY_SIZE
/// \brief for checking collisions between objects
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const RelativeRect MapCollisionInfo::FULL_SIZE                ( 0,  0,  100, 100 );
const RelativeRect MapCollisionInfo::HALF_LEFT_SIZE           ( 0,  0,  50,  100 );
const RelativeRect MapCollisionInfo::HALF_RIGHT_SIZE          ( 50, 0,  50,  100 );
const RelativeRect MapCollisionInfo::HALF_UPPER_SIZE          ( 0,  0,  100, 50  );
const RelativeRect MapCollisionInfo::HALF_DOWN_SIZE           ( 0,  50, 100, 50  );
const RelativeRect MapCollisionInfo::QUARTER_DOWN_RIGHT_SIZE  ( 50, 50, 50,  50  );
const RelativeRect MapCollisionInfo::QUARTER_DOWN_LEFT_SIZE   ( 0,  50, 50,  50  );
const RelativeRect MapCollisionInfo::QUARTER_UPPER_RIGHT_SIZE ( 50, 0,  50,  50  );
const RelativeRect MapCollisionInfo::QUARTER_UPPER_LEFT_SIZE  ( 0,  0,  50,  50  );
const RelativeRect MapCollisionInfo::EMPTY                    ( 0,  0,  0,   0   );

MapCollisionInfo::MapCollisionInfo():
    m_SizeCoeffArr({ &FULL_SIZE, &FULL_SIZE, &HALF_LEFT_SIZE, &HALF_LEFT_SIZE, &HALF_UPPER_SIZE, &HALF_UPPER_SIZE, &HALF_RIGHT_SIZE,
                    &HALF_RIGHT_SIZE, &HALF_DOWN_SIZE, &HALF_DOWN_SIZE, &FULL_SIZE, &FULL_SIZE, &FULL_SIZE, &QUARTER_DOWN_RIGHT_SIZE,
                    &QUARTER_DOWN_LEFT_SIZE, &QUARTER_UPPER_RIGHT_SIZE, &QUARTER_UPPER_LEFT_SIZE, &QUARTER_DOWN_RIGHT_SIZE,
                    &QUARTER_DOWN_LEFT_SIZE, &EMPTY }) {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MapObject::DEFAULT_TILE_SIZE
/// \brief MapObject::TILES_BASE_RECT
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const RelativeRect MapObject::DEFAULT_TILE_SIZE( 0,   0,   2.9,   5.2  );
const RelativeRect MapObject::TILES_BASE_RECT  ( 1.3, 2.3, 81.2,  93.6 );

////////////////////////////////////////////////////////////////
/// \brief MapObject::COLLISION_INFO
/// \brief Object with data about real or "solid" part of tile
////////////////////////////////////////////////////////////////
const MapCollisionInfo MapObject::COLLISION_INFO;


MapObject::MapObject( Texture texture, Renderer renderer ): m_Texture( texture ), m_Renderer( renderer ), m_TileRealPart({ 0, 0, 0, 0 }),
    m_TileRect({ 0, 0, 0, 0 }), m_nTileNumber( TILE_DEFAULT_NUMBER )
{
    m_Texture.setRelativeDestination( DEFAULT_TILE_SIZE, TILES_BASE_RECT );
    m_TileRect = m_Texture.getDestination();
    fillTileAtomsArr();
}

MapObject::~MapObject() {}

void MapObject::calcTileRealSize()
{
    m_TileRealPart.x = m_TileRect.x + lround( m_TileRect.w * m_TileRelativeRealPart.m_dX / 100 );
    m_TileRealPart.y = m_TileRect.y + lround( m_TileRect.h * m_TileRelativeRealPart.m_dY / 100 );
    m_TileRealPart.w = lround( m_TileRect.w * m_TileRelativeRealPart.m_dWidth / 100  );
    m_TileRealPart.h = lround( m_TileRect.h * m_TileRelativeRealPart.m_dHeight / 100 );
}

void MapObject::fillTileAtomsArr()
{
    int nDx = lround( 1.0 * m_TileRect.w / static_cast <int> ( TILE_ATOMS_IN_DIMENSION ) );
    int nDy = lround( 1.0 * m_TileRect.h / static_cast <int> ( TILE_ATOMS_IN_DIMENSION ) );
    SDL_Rect AtomRect = { m_TileRect.x, m_TileRect.y, nDx, nDy };

    m_TileAtomsVc.clear();

    for( size_t i = 0; i < TILE_ATOMS_NUMBER; ++i )
    {
        if( ( m_nTileNumber != TILE_DEFAULT_NUMBER ) && SDL_RectContains( m_TileRealPart, AtomRect ) )
        {
            m_TileAtomsVc.emplace_back( true, AtomRect );
        }

        if( ( i + 1 ) % TILE_ATOMS_IN_DIMENSION )
        {
            AtomRect.x += nDx;

            if( ( AtomRect.x + AtomRect.w ) > ( m_TileRealPart.x + m_TileRealPart.w ) )
            {
                int dx = AtomRect.x + AtomRect.w - m_TileRealPart.x - m_TileRealPart.w;

                if( ( AtomRect.w - dx ) > 0 )
                {
                    AtomRect.w -= dx;
                }
            }
        }
        else
        {
            AtomRect.x = m_TileRect.x;
            AtomRect.w = nDx;
            AtomRect.h = nDy;
            AtomRect.y += nDy;

            if( ( AtomRect.y + AtomRect.h ) > ( m_TileRealPart.y + m_TileRealPart.h ) )
            {
                int dy = AtomRect.y + AtomRect.h - m_TileRealPart.y - m_TileRealPart.h;

                if( ( AtomRect.h - dy ) > 0 )
                {
                    AtomRect.h -= dy;
                }
            }
        }
    }
}

void MapObject::setSourceRectNumber( size_t nTileNumber )
{
    m_nTileNumber = nTileNumber;

    if( nTileNumber < COLLISION_INFO.m_SizeCoeffArr.size() )
    {
        m_TileRelativeRealPart = *COLLISION_INFO.m_SizeCoeffArr.at( nTileNumber );
        calcTileRealSize();
        fillTileAtomsArr();
    }
}

bool MapObject::SDL_RectContains( SDL_Rect &MoreLargeRect, SDL_Rect &MoreSmallRect )
{
    return ( ( MoreLargeRect.x <= MoreSmallRect.x ) && ( MoreLargeRect.y <= MoreSmallRect.y ) &&
             ( ( MoreLargeRect.x + MoreLargeRect.w ) >= ( MoreSmallRect.x + MoreSmallRect.w ) ) &&
             ( ( MoreLargeRect.y + MoreLargeRect.h ) >= ( MoreSmallRect.y + MoreSmallRect.h ) ) );
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
    calcTileRealSize();
    fillTileAtomsArr();
}

SDL_Point MapObject::getPosition() const
{
    return m_Texture.getPosition();
}

SDL_Rect MapObject::getDestination() const
{
    return m_Texture.getDestination();
}

void MapObject::changeSize()
{
    m_Texture.changeSize();
    m_TileRect = m_Texture.getDestination();
    calcTileRealSize();
    fillTileAtomsArr();
}

void MapObject::renderTexture( SDL_Rect SourceRect )
{
    m_Texture.render( &SourceRect );

    SDL_SetRenderDrawColor( m_Renderer, Map::BLACK.r, Map::BLACK.g, Map::BLACK.b, Map::BLACK.a );
    std::for_each( m_TileAtomsVc.begin(), m_TileAtomsVc.end(), [this] ( std::pair <bool, SDL_Rect> RealAtomPair )
    {
        if( !RealAtomPair.first )
        {
            SDL_RenderFillRect( m_Renderer, &RealAtomPair.second );
        }
    });
}

void MapObject::renderFillRect()
{
    SDL_SetRenderDrawColor( m_Renderer, Map::BLACK.r, Map::BLACK.g, Map::BLACK.b, Map::BLACK.a );
    SDL_RenderFillRect( m_Renderer, &m_TileRect );
}

SDL_Rect MapObject::getBase() const
{
    return m_Texture.getBase();
}

RelativeRect MapObject::getRelativeDestination() const
{
    return m_Texture.getRelativeDestination();
}

RelativeRect MapObject::getRelativeBase() const
{
    return m_Texture.getRelativeBase();
}

bool MapObject::checkCollision( const SDL_Rect &CheckingRect )
{
    for( size_t i = 0, nVcSize = m_TileAtomsVc.size(); i < nVcSize; ++i )
    {
        if( m_TileAtomsVc.at( i ).first && SDL_HasIntersection( &m_TileAtomsVc.at( i ).second, &CheckingRect ) )
        {
            return true;
        }
    }

    return false;
}

void MapObject::destroy( const SDL_Rect &DestroyingRect )
{
    for( size_t i = 0, nVcSize = m_TileAtomsVc.size(); i < nVcSize; ++i )
    {
        if( m_TileAtomsVc.at( i ).first && SDL_HasIntersection( &m_TileAtomsVc.at( i ).second, &DestroyingRect ) )
        {
            m_TileAtomsVc.at( i ).first = false;
        }
    }
}