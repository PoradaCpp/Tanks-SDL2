
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <cmath>

#include "map.h"
#include "state.h"


Map::Map( MapInitData InitData, Renderer renderer ): m_TilesTexture( InitData.m_ImgTextureInitData, renderer ), m_Renderer( renderer ),
    m_sPresentMapsPath( InitData.m_sPresentMapsPath ), m_nCurrentMapIndex( 0 )
{
    m_MapVc.reserve( MAP_SIZE );

    for( size_t i = 0; i < MAP_SIZE; ++i )
    {
        m_MapVc.push_back( std::make_shared <MapObject> ( m_TilesTexture, m_Renderer ) );
    }
    m_MapVc.front()->setRelativePosition( TILE_X_BEGIN, TILE_Y_BEGIN );

    std::ifstream readMapsPaths;
    readMapsPaths.open( m_sPresentMapsPath );

    if( !readMapsPaths )
    {
        std::cerr << "File opening error! Can't open file " << m_sPresentMapsPath << '\n';
        exit(-1);
    }

    std::string sPath;
    while( readMapsPaths >> sPath )
    {
        m_MapsPathsVc.push_back( sPath );
    }
    readMapsPaths.close();

    changeSize();
    setRenderDimensions();
}

Map::~Map() {}

void Map::setRenderDimensions()
{
    int nWidth = m_TilesTexture.getWidth();
    int nHeight = m_TilesTexture.getHeight();
    int nSrcRectHeight = nHeight / static_cast <int> ( VECTOR_TILE_HEIGHT );
    int nSrcRectWidth = nWidth / static_cast <int> ( VECTOR_TILE_WIDTH );
    int width( 0 ) , height( 0 );

    m_SourceRectVc.reserve( VECTOR_TILE_SIZE );
    for( size_t i = 0 ; i < VECTOR_TILE_SIZE; ++i, ++width )
    {
        if( width >= static_cast <int> ( VECTOR_TILE_WIDTH ) )
        {
            width = 0;
            ++height;
        }
        m_SourceRectVc.push_back({ nSrcRectWidth * width, nSrcRectHeight * height, nSrcRectWidth, nSrcRectHeight });
    }
}

void Map::readMapFromFile( std::string sPath )
{
    std::ifstream dataFile( sPath );
    std::string sDataString;
    std::streamoff nFileSize;

    if(!dataFile)
    {
        std::cerr << "File opening error! Can't open file " << sPath << '\n';
        exit(-1);
    }

    dataFile.seekg( 0, std::ios::end );
    nFileSize = dataFile.tellg();

    if( nFileSize < sDataString.max_size() )
    {
        sDataString.resize( static_cast <size_t> ( nFileSize ) );
        dataFile.seekg(0);
        dataFile.read( &sDataString[0], static_cast <std::streamsize> ( nFileSize ) );

        std::istringstream iDataStream( sDataString );
        std::istream_iterator <size_t> dataBegin( iDataStream ), dataEnd;

        std::for_each( m_MapVc.begin(), m_MapVc.end(), [&dataBegin, dataEnd] ( pSharedMapObject &pMapObject )
        {
            if( dataBegin != dataEnd )
            {
                pMapObject->setSourceRectNumber( *dataBegin++ );
            }
        });
    }
    else
    {
        std::cerr << "Map reading error! Too large " << sPath << " size!\n";
        exit(-1);
    }

    dataFile.close();
}

size_t Map::tileNumberForPoint( int nX, int nY )
{
    size_t nTilePosX = static_cast <size_t> (( nX - m_TileBeginRect.x ) / ( 1.0 * m_MapBordersRect.w / ROW_SIZE ));
    size_t nTilePosY = static_cast <size_t> (( nY - m_TileBeginRect.y ) / ( 1.0 * m_MapBordersRect.h / COLUMN_SIZE ));

    if( nTilePosX >= ROW_SIZE )
    {
        nTilePosX = ROW_SIZE - 1;
    }

    if( nTilePosY >= COLUMN_SIZE )
    {
        nTilePosY = COLUMN_SIZE - 1;
    }

    return nTilePosX + ( nTilePosY * ROW_SIZE );
}

void Map::changeSize()
{
    SDL_Rect CurPosRect;
    SDL_Rect UpperPosRect;
    int nCurPosX(0), nCurPosY(0), nCurPosH(0), nDw(0), nDh(0);
    double dTilePosX( TILE_X_BEGIN ), dTilePosY( TILE_Y_BEGIN );

    for( size_t i = 0; i < MAP_SIZE; ++i )
    {
        m_MapVc.at( i )->setRelativePosition( dTilePosX, dTilePosY );
        CurPosRect = m_MapVc.at( i )->getDestination();

        if( !i )
        {
            m_TileBeginRect = CurPosRect;
            m_MapBordersRect = CurPosRect;
            nCurPosX = m_TileBeginRect.x + m_TileBeginRect.w;
            nCurPosY = m_TileBeginRect.y;
            nCurPosH = m_TileBeginRect.h;
        }
        else if( i < ROW_SIZE )
        {
            nDw = nCurPosX - CurPosRect.x;

            if( nDw && CurPosRect.w > nDw )
            {
                CurPosRect.w -= nDw;
                CurPosRect.x = nCurPosX;
            }
            nCurPosX += CurPosRect.w;
            m_MapBordersRect.w += CurPosRect.w;
        }
        else
        {
            UpperPosRect = m_MapVc.at( i - ROW_SIZE )->getDestination();
            CurPosRect.w = UpperPosRect.w;
            CurPosRect.x = UpperPosRect.x;

            if( !( i % ROW_SIZE ) )
            {
                nCurPosY += nCurPosH;
                nCurPosH = CurPosRect.h;
                nDh = nCurPosY - CurPosRect.y;

                if( nDh && nCurPosH > nDh )
                {
                    nCurPosH -= nDh;
                }
                m_MapBordersRect.h += nCurPosH;
            }
        }

        if( i )
        {
            CurPosRect.h = nCurPosH;
            CurPosRect.y = nCurPosY;
            m_MapVc.at( i )->setDestination( CurPosRect );
        }

        if( ( i + 1 ) % ROW_SIZE )
        {
            dTilePosX += TILE_WIDTH;
        }
        else
        {
            dTilePosX = TILE_X_BEGIN;
            dTilePosY += TILE_HEIGHT;
        }
    }
}

void Map::render()
{
    SDL_SetRenderDrawColor( m_Renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a );
    SDL_RenderFillRect( m_Renderer, &m_MapBordersRect );

    std::for_each( m_MapVc.begin(), m_MapVc.end(), [ this ] ( pSharedMapObject &pMapObject )
    {
        if( pMapObject->m_nTileNumber < VECTOR_TILE_SIZE )
        {
            pMapObject->renderTexture( m_SourceRectVc.at( pMapObject->m_nTileNumber ) );
        }
        else
        {
            pMapObject->renderFillRect();
        }
    });
}

Texture Map::getTilesTexture()
{
    return m_TilesTexture;
}

void Map::saveCurrentMap()
{
    std::string sNewMapName( "media/maps/level" + std::to_string( m_MapsPathsVc.size() ) + ".map" );

    std::string sTempName = sNewMapName;
    m_MapsPathsVc.push_back( sNewMapName );

    std::ofstream writeNewMapPath;
    writeNewMapPath.open( m_sPresentMapsPath, std::ios::app );

    if( !writeNewMapPath )
    {
        std::cerr << "File opening error! Can't open file " << m_sPresentMapsPath << '\n';
        exit(-1);
    }

    if( sTempName != sNewMapName )
    {
        std::cout << "sTempName is corrupted: " << sTempName << '\n';
    }
    writeNewMapPath << ' ' << m_MapsPathsVc.back();
    writeNewMapPath.close();

    std::ofstream writeMap;
    writeMap.open( sNewMapName );

    if( !writeMap )
    {
        std::cerr << "File opening error! Can't open file " << sNewMapName << '\n';
        exit(-1);
    }

    std::for_each( m_MapVc.begin(), m_MapVc.end(), [ &writeMap ] ( pSharedMapObject &pMapObject )
    {
        writeMap << std::setw(2) << std::setfill('0') << pMapObject->getTileNumber() << ' ';
    });

    writeMap.close();
}

void Map::loadDefaultMap()
{
    if( !m_MapsPathsVc.empty() )
    {
        readMapFromFile( m_MapsPathsVc.front() );
    }
}

void Map::loadMap()
{
    if( m_nCurrentMapIndex + 1 < m_MapsPathsVc.size() )
    {
        readMapFromFile( m_MapsPathsVc.at( ++m_nCurrentMapIndex ) );
    }
}

SDL_Rect Map::getMapBordersRect() const
{
    return m_MapBordersRect;
}

RelativeRect Map::getRelativeBase() const
{
    return m_MapVc.front()->getRelativeBase();
}

int Map::getPageHeight() const
{
    return m_TilesTexture.getPageHeight();
}

int Map::getPageWidth() const
{
    return m_TilesTexture.getPageWidth();
}

bool Map::checkCollision(const SDL_Rect &CheckingRect )
{
    if( checkBorderCollision( CheckingRect ) )
    {
        return true;
    }

    size_t nTileUpperBegin = tileNumberForPoint( CheckingRect.x, CheckingRect.y );
    size_t nTileUpperEnd   = tileNumberForPoint( CheckingRect.x + CheckingRect.w, CheckingRect.y );
    size_t nTileDownBegin  = tileNumberForPoint( CheckingRect.x + CheckingRect.w, CheckingRect.y + CheckingRect.h );

    for( size_t i = nTileUpperBegin; i <= nTileDownBegin; i += ROW_SIZE )
    {
        for( size_t j = 0, nRowNum = nTileUpperEnd - nTileUpperBegin; j <= nRowNum; ++j )
        {
            if( i + j >= m_MapVc.size() )
            {
                std::cout << "nTileUpperBegin: " << nTileUpperBegin << "\nnTileUpperEnd: " << nTileUpperEnd <<
                             "\nnTileDownBegin: " << nTileDownBegin << "\nnRowNum: " << nRowNum <<"\n\n";
            }

            if( m_MapVc.at( i + j )->checkCollision( CheckingRect ) )
            {
                return true;
            }
        }
    }
    return false;
}

bool Map::checkBorderCollision( const SDL_Rect &CheckingRect )
{
    int nX1 = m_MapBordersRect.x;
    int nY1 = m_MapBordersRect.y;
    int nX2 = m_MapBordersRect.x + m_MapBordersRect.w;
    int nY2 = m_MapBordersRect.y + m_MapBordersRect.h;

    if( SDL_IntersectRectAndLine( &CheckingRect, &nX1, &nY1, &nX2, &nY1 ) )
    {
        return true;
    }
    if( SDL_IntersectRectAndLine( &CheckingRect, &nX2, &nY1, &nX2, &nY2 ) )
    {
        return true;
    }
    if( SDL_IntersectRectAndLine( &CheckingRect, &nX2, &nY2, &nX1, &nY2 ) )
    {
        return true;
    }
    if( SDL_IntersectRectAndLine( &CheckingRect, &nX1, &nY2, &nX1, &nY1 ) )
    {
        return true;
    }
    if( !SDL_HasIntersection( &m_MapBordersRect, &CheckingRect) )
    {
        return true;
    }
    return false;
}

void Map::destroy( const SDL_Rect &CheckingRect, const SDL_Rect &DestroyingRect, CommonTanksProperties::MoveDirection moveDirection )
{
    size_t nTileUpperBegin = tileNumberForPoint( DestroyingRect.x - 5, DestroyingRect.y - 5 );
    size_t nTileUpperEnd   = tileNumberForPoint( DestroyingRect.x + DestroyingRect.w + 5, DestroyingRect.y - 5 );
    size_t nTileDownBegin  = tileNumberForPoint( DestroyingRect.x + DestroyingRect.w + 5, DestroyingRect.y + DestroyingRect.h + 5 );

    SDL_Rect CheckRect;
    CheckRect.w = CheckingRect.w > DestroyingRect.w ? CheckingRect.w : DestroyingRect.w;
    CheckRect.h = CheckingRect.h > DestroyingRect.h ? CheckingRect.h : DestroyingRect.h;
    CheckRect.x = CheckingRect.x + ( CheckingRect.w - CheckRect.w ) / 2;
    CheckRect.y = CheckingRect.y + ( CheckingRect.h - CheckRect.h ) / 2;

    for( size_t i = nTileUpperBegin; i <= nTileDownBegin; i += ROW_SIZE )
    {
        for( size_t j = 0, nRowNum = nTileUpperEnd - nTileUpperBegin; j <= nRowNum; ++j )
        {
            if( m_MapVc.at( i + j )->checkCollision( CheckRect ) )
            {
                m_MapVc.at( i + j )->destroy( DestroyingRect, moveDirection );
            }
        }
    }
}
