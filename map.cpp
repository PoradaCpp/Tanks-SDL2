
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>

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
    int nSrcRectHeight = nHeight / VECTOR_TILE_HEIGHT;
    int nSrcRectWidth = nWidth / VECTOR_TILE_WIDTH;
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

void Map::changeSize()
{
    size_t nCurrentPos(0);
    SDL_Rect CurPosRect;

    std::for_each( m_MapVc.begin(), m_MapVc.end(), [ &, this ] ( pSharedMapObject &pMapObject )
    {
        if( !nCurrentPos )
        {
            pMapObject->setRelativePosition( TILE_X_BEGIN, TILE_Y_BEGIN );

            m_TileBeginRect = pMapObject->getDestination();
            CurPosRect = m_TileBeginRect;
            CurPosRect.x += m_TileBeginRect.w;
            nCurrentPos = 2;
        }
        else
        {
            pMapObject->setDestination( CurPosRect );

            if( nCurrentPos < COLUMN_SIZE )
            {
                CurPosRect.x += m_TileBeginRect.w;
                ++nCurrentPos;
            }
            else
            {
                CurPosRect.x = m_TileBeginRect.x;
                CurPosRect.y += m_TileBeginRect.h;
                nCurrentPos = 1;
            }
        }
    });

    m_MapBordersRect = { m_TileBeginRect.x, m_TileBeginRect.y, m_TileBeginRect.w * static_cast <int> ( COLUMN_SIZE ),
                        m_TileBeginRect.h * static_cast <int> ( ROW_SIZE )};
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

    std::cout << "Write " << sTempName << " as New Map Path\n";
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
        readMapFromFile( m_MapsPathsVc.at( 0 ) );
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

void Map::readMapFromFile( std::string sPath )
{
    std::ifstream dataFile( sPath );
    std::string dataString;
    size_t nFileSize;

    if(!dataFile)
    {
        std::cerr << "File opening error! Can't open file " << sPath << '\n';
        exit(-1);
    }

    dataFile.seekg( 0, std::ios::end );
    nFileSize = dataFile.tellg();
    dataString.resize( nFileSize );
    dataFile.seekg(0);
    dataFile.read( &dataString[0], nFileSize );
    std::istringstream iDataStream( dataString );
    std::istream_iterator <size_t> dataBegin( iDataStream ), dataEnd;

    std::for_each( m_MapVc.begin(), m_MapVc.end(), [&dataBegin, dataEnd] ( pSharedMapObject &pMapObject )
    {
        if( dataBegin != dataEnd )
        {
            pMapObject->setSourceRectNumber( *dataBegin++ );
        }
    });

    dataFile.close();

    /*std::ifstream dataFile( sPath );

    if( !dataFile )
    {
        std::cerr << "File opening error! Can't open file " << sPath << '\n';
        exit(-1);
    }

    std::istream_iterator <size_t> dataBegin( dataFile ), dataEnd;

    std::for_each( m_MapVc.begin(), m_MapVc.end(), [&dataBegin, dataEnd] ( pSharedMapObject &pMapObject )
    {
        if( dataBegin != dataEnd )
        {
            pMapObject->setSourceRectNumber( *dataBegin++ );
        }
    });

    dataFile.close();*/
}
