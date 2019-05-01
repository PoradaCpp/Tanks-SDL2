
#include <algorithm>
#include <iostream>

#include "createmappage.h"
#include "state.h"

//////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MapSquareButton::MapSquareButton
//////////////////////////////////////////////////////////////////////////////////////////////
MapSquareButton::MapSquareButton(): m_pMap( nullptr ), m_nCurrentTile( 0 ), m_CurrentTileState( MapSquareState::DEFAULT ) {}

MapSquareButton::MapSquareButton::~MapSquareButton() {}

void MapSquareButton::attachMap( pSharedMap pMap )
{
    m_pMap = pMap;
}

void MapSquareButton::getCurrentTileState()
{
    SDL_GetMouseState( &m_CursorPos.x, &m_CursorPos.y );

    if( SDL_PointInRect( &m_CursorPos, &m_pMap->m_MapBordersRect ) )
    {
        size_t nTilePosX = ( m_CursorPos.x - m_pMap->m_MapBordersRect.x ) / m_pMap->m_TileBeginRect.w;
        size_t nTilePosY = ( m_CursorPos.y - m_pMap->m_MapBordersRect.y ) / m_pMap->m_TileBeginRect.h;

        size_t nCurrentTile = nTilePosX + ( nTilePosY * m_pMap->COLUMN_SIZE );

        if( m_nCurrentTile == nCurrentTile )
        {
            if ( SDL_GetMouseState( &m_CursorPos.x, &m_CursorPos.y ) & SDL_BUTTON( SDL_BUTTON_LEFT ))
            {
                m_CurrentTileState = MapSquareState::PRESSED;
            }
            else if( MapSquareState::PRESSED == m_CurrentTileState )
            {
                m_CurrentTileState = MapSquareState::RELEASED;
            }
        }
        else
        {
            m_nCurrentTile = nCurrentTile;
            m_CurrentTileState = MapSquareState::IN_FOCUS;
        }
    }
}

void MapSquareButton::action( pSharedBuildMap &pMapUnit )
{
    getCurrentTileState();

    if( MapSquareState::IN_FOCUS == m_CurrentTileState )
    {
        pMapUnit->setDestRect( m_pMap->m_MapVc.at( m_nCurrentTile )->getDestination() );
    }
    else if( MapSquareState::RELEASED == m_CurrentTileState )
    {
        m_pMap->m_MapVc.at( m_nCurrentTile )->setSourceRectNumber( pMapUnit->getSourceRectPos() );
        m_CurrentTileState = MapSquareState::IN_FOCUS;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief CreateMapPage::CreateMapPage
/// \param objContainers
/// \param pState
/// \param renderer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CreateMapPage::CreateMapPage( InitContainers &&objContainers , State *pState, Renderer renderer ): Page( std::move( objContainers ) ),
     m_Renderer( renderer ), m_CurrentState( CurrentState::START_PAGE )
{
    m_pMap->loadDefaultMap();
    m_pMapUnit = std::make_shared < BuildMap > ( m_pMap->getTilesTexture(), m_Renderer );
    m_MapSquareButton.attachMap( m_pMap );
    initButtons( pState );
}

CreateMapPage::~CreateMapPage() {}

void CreateMapPage::initButtons( State *pState )
{
    if( static_cast <size_t> ( Buttons::BUTTONS_QUANTITY ) == m_ButtonVc.size() )
    {
        for( size_t i = 0; i < static_cast <size_t> ( Buttons::BUILD_MAP_BUTTONS_QUANTITY ); ++i )
        {
            m_ButtonVc.at( i )->setAction( [ this, i ] ()
            {
                m_pMapUnit->setSource( i );
            });
        }

        m_ButtonVc.at( static_cast <size_t> ( Buttons::BUILD_MAP_BUTTONS_QUANTITY ))->setAction( [ this ] ()
        {
            m_pMapUnit->setSource( static_cast <size_t> ( MapObject::TILE_DEFAULT_NUMBER ) );
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_GAME_STATE ))->setAction( [pState, this] ()
        {
            pState->changeState( CurrentState::GAME );
            m_CurrentState = CurrentState::GAME;
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SAVE_CURRENT_MAP ))->setAction( [this] ()
        {
            m_pMap->saveCurrentMap();
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_START_PAGE_STATE ))->setAction( [pState, this] ()
        {
            pState->changeState( CurrentState::START_PAGE );
            m_CurrentState = CurrentState::START_PAGE;
        });
    }
    else if( static_cast <size_t> ( Buttons::BUTTONS_QUANTITY ) < m_ButtonVc.size() )
    {
        std::cerr << "CreateMapPage button init error! Too many initialized buttons\n";
        exit(-1);
    }
    else
    {
        std::cerr << "CreateMapPage button init error! Too few initialized buttons\n";
        exit(-1);
    }
}

void CreateMapPage::resize()
{
    Page::resize();

    m_pMap->changeSize();
    m_pMapUnit->changeSize();
}

void CreateMapPage::render()
{
    if( m_CurrentState != CurrentState::CREATE_MAP )
    {
        m_CurrentState = CurrentState::CREATE_MAP;
        resize();
    }

    Page::render();

    renderMap();

    SDL_GetMouseState( &m_CursorPos.x, &m_CursorPos.y );
    if( SDL_PointInRect( &m_CursorPos, &m_pMap->m_MapBordersRect ) )
    {
        m_pMapUnit->changePosition();
        m_pMapUnit->render();
    }
}

void CreateMapPage::renderMap()
{
    m_pMap->render();
    m_MapSquareButton.action( m_pMapUnit );
    renderMapNet();
}

void CreateMapPage::renderMapNet()
{
    Uint32 nLinePosX( m_pMap->m_TileBeginRect.x ),
           nLinePosY( m_pMap->m_TileBeginRect.y ),
           nHorizLineLenght( m_pMap->m_TileBeginRect.x + m_pMap->m_TileBeginRect.w * m_pMap->COLUMN_SIZE ),
           nVertLineLenght ( m_pMap->m_TileBeginRect.y + m_pMap->m_TileBeginRect.h * m_pMap->ROW_SIZE );

    SDL_SetRenderDrawColor( m_Renderer,  Map::BROWN.r,  Map::BROWN.g,  Map::BROWN.b,  Map::BROWN.a );

    for( Uint32 nPosX = 0; nPosX <= m_pMap->COLUMN_SIZE; ++nPosX )
    {
        SDL_RenderDrawLine( m_Renderer, nLinePosX, nLinePosY, nLinePosX, nVertLineLenght );
        nLinePosX += m_pMap->m_TileBeginRect.w;
    }

    nLinePosX = m_pMap->m_TileBeginRect.x;
    nLinePosY = m_pMap->m_TileBeginRect.y;

    for( Uint32 nPosY = 0; nPosY <= m_pMap->ROW_SIZE; ++nPosY )
    {
        SDL_RenderDrawLine( m_Renderer, nLinePosX, nLinePosY, nHorizLineLenght, nLinePosY );
        nLinePosY += m_pMap->m_TileBeginRect.h;
    }
}

