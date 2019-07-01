#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include "mapobject.h"
#include "map.h"

//std::vector <SDL_Rect> vc;


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
const RelativeRect MapCollisionInfo::EMPTY                    ( 0,  0,  0,   0   );

MapCollisionInfo::MapCollisionInfo():
    m_SizeCoeffArr({ &FULL_SIZE, &FULL_SIZE, &HALF_LEFT_SIZE, &HALF_LEFT_SIZE, &HALF_UPPER_SIZE, &HALF_UPPER_SIZE, &HALF_RIGHT_SIZE,
                     &HALF_RIGHT_SIZE, &HALF_DOWN_SIZE, &HALF_DOWN_SIZE, &FULL_SIZE, &FULL_SIZE, &FULL_SIZE, &QUARTER_DOWN_RIGHT_SIZE,
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
    m_TileRect({ 0, 0, 0, 0 }), m_nTileNumber( TILE_DEFAULT_NUMBER ),
    m_TileCollisionProperties( TileCollisionProperties::FULL_COLLISION ),
    m_DisplayingLayer( DisplayingLayer::LOWER_LAYER )
{
    m_Texture.setRelativeDestination( DEFAULT_TILE_SIZE, TILES_BASE_RECT );
    m_TileRect = m_Texture.getDestination();
    determineTileType();
    fillTileAtomsArr();
}

MapObject::~MapObject() {}

void MapObject::determineTileType()
{
    if( ( m_nTileNumber < static_cast <uint32_t> ( TileType::SOLID_WALLS_END ) &&
          !( m_nTileNumber % 2 ) ) || ( static_cast <uint32_t> ( TileType::ICE          ) < m_nTileNumber &&
                                        static_cast <uint32_t> ( TileType::WHITE_SQUARE ) > m_nTileNumber ) )
    {
        m_TileType = TileType::BRICK;
        m_TileCollisionProperties = TileCollisionProperties::FULL_COLLISION;
        m_DisplayingLayer = DisplayingLayer::LOWER_LAYER;
    }
    else if( m_nTileNumber <= static_cast <uint32_t> ( TileType::SOLID_WALLS_END ) && m_nTileNumber % 2 )
    {
        m_TileType = TileType::GRANITE;
        m_TileCollisionProperties = TileCollisionProperties::FULL_COLLISION;
        m_DisplayingLayer = DisplayingLayer::LOWER_LAYER;
    }
    else if( static_cast <uint32_t> ( TileType::FOREST ) == m_nTileNumber )
    {
        m_TileType = TileType::FOREST;
        m_TileCollisionProperties = TileCollisionProperties::NO_COLLISION;
        m_DisplayingLayer = DisplayingLayer::UPPER_LAYER;
    }
    else if( static_cast <uint32_t> ( TileType::WATER ) == m_nTileNumber )
    {
        m_TileType = TileType::WATER;
        m_TileCollisionProperties = TileCollisionProperties::TANK_ONLY_COLLISION;
        m_DisplayingLayer = DisplayingLayer::LOWER_LAYER;
    }
    else if( static_cast <uint32_t> ( TileType::ICE ) == m_nTileNumber )
    {
        m_TileType = TileType::ICE;
        m_TileCollisionProperties = TileCollisionProperties::NO_COLLISION;
        m_DisplayingLayer = DisplayingLayer::LOWER_LAYER;
    }
    else
    {
        m_TileType = TileType::EMPTY;
        m_TileCollisionProperties = TileCollisionProperties::NO_COLLISION;
        m_DisplayingLayer = DisplayingLayer::LOWER_LAYER;
    }
}

void MapObject::calcTileRealSize()
{
    m_TileRealPart.x = m_TileRect.x + lround( m_TileRect.w * m_TileRelativeRealPart.m_dX / 100 );
    m_TileRealPart.y = m_TileRect.y + lround( m_TileRect.h * m_TileRelativeRealPart.m_dY / 100 );
    m_TileRealPart.w = lround( m_TileRect.w * m_TileRelativeRealPart.m_dWidth / 100  );
    m_TileRealPart.h = lround( m_TileRect.h * m_TileRelativeRealPart.m_dHeight / 100 );
}

void MapObject::fillTileAtomsArr()
{
    double dDx = 1.0 * m_TileRect.w / static_cast <double> ( TILE_ATOMS_IN_DIMENSION );
    double dDy = 1.0 * m_TileRect.h / static_cast <double> ( TILE_ATOMS_IN_DIMENSION );
    double dWidth = m_TileRect.x + dDx;
    double dHeight = m_TileRect.y + dDy;
    int nNextPosX = lround( dDx ) + m_TileRect.x;
    int nNextPosY = lround( dDy ) + m_TileRect.y;
    SDL_Rect AtomRect = { m_TileRect.x, m_TileRect.y, lround( dDx ), lround( dDy ) };
    bool fAtomsCreated = !m_TileAtomsVc.empty();

    for( size_t i = 0, j = 0, nVcSize = m_TileAtomsVc.size(); i < TILE_ATOMS_NUMBER; ++i )
    {
        if( ( m_nTileNumber != TILE_DEFAULT_NUMBER ) && SDL_RectContains( m_TileRealPart, AtomRect ) )
        {
            if( fAtomsCreated )
            {
                m_TileAtomsVc.at( j++ ).second = AtomRect;
                if( j >= nVcSize )
                {
                    break;
                }
            }
            else
            {
                m_TileAtomsVc.emplace_back( true, AtomRect );
            }
        }

        if( ( i + 1 ) % TILE_ATOMS_IN_DIMENSION )
        {
            dWidth += dDx;
            AtomRect.x = nNextPosX;
            AtomRect.w = lround( dWidth - AtomRect.x );
            nNextPosX = AtomRect.x + AtomRect.w;

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
            AtomRect.x = m_TileRect.x;;
            AtomRect.w = lround( dDx );
            nNextPosX = AtomRect.x + AtomRect.w;
            dWidth = m_TileRect.x + dDx;
            AtomRect.y = nNextPosY;
            dHeight += dDy;
            AtomRect.h = lround( dHeight - AtomRect.y );
            nNextPosY = AtomRect.y + AtomRect.h;

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
    m_TileAtomsVc.clear();
    m_nTileNumber = nTileNumber;
    determineTileType();

    if( nTileNumber < COLLISION_INFO.m_SizeCoeffArr.size() )
    {
        m_TileRelativeRealPart = *COLLISION_INFO.m_SizeCoeffArr.at( nTileNumber );
        calcTileRealSize();
        fillTileAtomsArr();
    }
}

bool MapObject::SDL_RectContains( SDL_Rect &MoreLargeRect, SDL_Rect &MoreSmallRect )
{
    return (   ( MoreLargeRect.x <= MoreSmallRect.x ) && ( MoreLargeRect.y <= MoreSmallRect.y ) &&
             ( ( MoreLargeRect.x  + MoreLargeRect.w ) >= ( MoreSmallRect.x  + MoreSmallRect.w ) ) &&
             ( ( MoreLargeRect.y  + MoreLargeRect.h ) >= ( MoreSmallRect.y  + MoreSmallRect.h ) ) );
}

void MapObject::destroyingRectAlignment( SDL_Rect &DestroyingRect, CommonTanksProperties::MoveDirection moveDirection )
{
    SDL_Point Point1, Point2, CheckPoint;
    int nLenght = DestroyingRect.w < DestroyingRect.h ? DestroyingRect.w : DestroyingRect.h;
    nLenght = lround( nLenght * 1.2 );

    switch( moveDirection )
    {
    case CommonTanksProperties::MoveDirection::UP:
        Point1 = { DestroyingRect.x, DestroyingRect.y + DestroyingRect.h };
        Point2 = { DestroyingRect.x + DestroyingRect.w, DestroyingRect.y + DestroyingRect.h };
        break;

    case CommonTanksProperties::MoveDirection::DOWN:
        Point1 = { DestroyingRect.x, DestroyingRect.y };
        Point2 = { DestroyingRect.x + DestroyingRect.w, DestroyingRect.y };
        break;

    case CommonTanksProperties::MoveDirection::RIGHT:
        Point1 = { DestroyingRect.x + DestroyingRect.w, DestroyingRect.y };
        Point2 = { DestroyingRect.x + DestroyingRect.w, DestroyingRect.y + DestroyingRect.h };
        break;

    case CommonTanksProperties::MoveDirection::LEFT:
        Point1 = { DestroyingRect.x, DestroyingRect.y };
        Point2 = { DestroyingRect.x, DestroyingRect.y + DestroyingRect.h };
        break;
    }
    CheckPoint = Point1;

    for( int i = 0; i < nLenght && !checkCollisionWithLine( Point1, Point2 ); ++i )
    {
        switch( moveDirection )
        {
        case CommonTanksProperties::MoveDirection::UP:
            --Point1.y;
            --Point2.y;
            break;

        case CommonTanksProperties::MoveDirection::DOWN:
            ++Point1.y;
            ++Point2.y;
            break;

        case CommonTanksProperties::MoveDirection::RIGHT:
            --Point1.x;
            --Point2.x;
            break;

        case CommonTanksProperties::MoveDirection::LEFT:
            ++Point1.x;
            ++Point2.x;
            break;
        }
    }
    DestroyingRect.x += Point1.x - CheckPoint.x;
    DestroyingRect.y += Point1.y - CheckPoint.y;
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

void MapObject::render( SDL_Rect SourceRect )
{
    m_Texture.render( &SourceRect );

    SDL_SetRenderDrawColor( m_Renderer, Map::BLACK.r, Map::BLACK.g, Map::BLACK.b, Map::BLACK.a );
    std::for_each( m_TileAtomsVc.begin(), m_TileAtomsVc.end(), [this] ( std::pair <bool, SDL_Rect> RealAtomPair )
    {
        if( !RealAtomPair.first )
        {
            //SDL_SetRenderDrawColor( m_Renderer, Map::BLACK.r, Map::BLACK.g, Map::BLACK.b, Map::BLACK.a );
            SDL_RenderFillRect( m_Renderer, &RealAtomPair.second );
        }
//        else
//        {
//            SDL_SetRenderDrawColor( m_Renderer, Map::GREY.r, Map::GREY.g, Map::GREY.b, Map::GREY.a );
//            SDL_RenderDrawRect( m_Renderer, &RealAtomPair.second );
//        }
    });

//    SDL_SetRenderDrawColor( m_Renderer, 0, 255, 0, 255 );
//    SDL_RenderDrawRect( m_Renderer, &m_TileRealPart );

//    SDL_SetRenderDrawColor( m_Renderer, 0, 0, 255, 255 );
//    std::for_each( vc.begin(), vc.end(), [this] ( SDL_Rect Rect )
//    {
//        SDL_RenderDrawRect( m_Renderer, &Rect );
//    });
}

void MapObject::renderLowerLayer( SDL_Rect SourceRect )
{
    if( DisplayingLayer::LOWER_LAYER == m_DisplayingLayer )
    {
        render( SourceRect );
    }
}

void MapObject::renderUpperLayer( SDL_Rect SourceRect )
{
    if( DisplayingLayer::UPPER_LAYER == m_DisplayingLayer )
    {
        render( SourceRect );
    }
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

bool MapObject::checkCollision( const SDL_Rect &CheckingRect , bool isShell )
{
    if( ( (            TileCollisionProperties::NO_COLLISION   != m_TileCollisionProperties ) ||
          ( isShell && TileCollisionProperties::FULL_COLLISION == m_TileCollisionProperties ) ) &&
            SDL_HasIntersection( &m_TileRealPart, &CheckingRect ) )
    {
        for( size_t i = 0, nVcSize = m_TileAtomsVc.size(); i < nVcSize; ++i )
        {
            if( m_TileAtomsVc.at( i ).first && SDL_HasIntersection( &m_TileAtomsVc.at( i ).second, &CheckingRect ) )
            {
                return true;
            }
        }
    }
    return false;
}

bool MapObject::checkCollisionWithLine( SDL_Point Point1, SDL_Point Point2 )
{
    for( size_t i = 0, nVcSize = m_TileAtomsVc.size(); i < nVcSize; ++i )
    {
        if( m_TileAtomsVc.at( i ).first &&
            SDL_IntersectRectAndLine( &m_TileAtomsVc.at( i ).second, &Point1.x, &Point1.y, &Point2.x, &Point2.y ) )
        {
            return true;
        }
    }
    return false;
}

void MapObject::destroy( SDL_Rect DestroyingRect, CommonTanksProperties::MoveDirection moveDirection )
{
    if( TileType::GRANITE == m_TileType )
    {
        int &DestroyingDepth = DestroyingRect.w > DestroyingRect.h ? DestroyingRect.h : DestroyingRect.w;

        if( DestroyingDepth < m_TileRect.w / 2 )
        {
            return;
        }
        else
        {
            DestroyingDepth /= 4;

            if( CommonTanksProperties::MoveDirection::DOWN == moveDirection )
            {
                DestroyingRect.y += DestroyingRect.h;
            }
            else if( CommonTanksProperties::MoveDirection::LEFT == moveDirection )
            {
                DestroyingRect.x += DestroyingRect.w;
            }
        }
    }
    else if( m_TileType != TileType::BRICK )
    {
        return;
    }

    destroyingRectAlignment( DestroyingRect, moveDirection );

    //vc.push_back( DestroyingRect );

    for( size_t i = 0, nVcSize = m_TileAtomsVc.size(); i < nVcSize; ++i )
    {
        if( m_TileAtomsVc.at( i ).first && SDL_HasIntersection( &m_TileAtomsVc.at( i ).second, &DestroyingRect ) )
        {
            m_TileAtomsVc.at( i ).first = false;
        }
    }
}

void MapObject::clear()
{
    m_TileAtomsVc.clear();
}

MapObject::DisplayingLayer MapObject::getDisplayingLayer() const
{
    return m_DisplayingLayer;
}
