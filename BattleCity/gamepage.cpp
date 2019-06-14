
#include <algorithm>
#include <iostream>

#include "gamepage.h"
#include "state.h"


GamePage::GamePage( InitContainers &&objContainers , pSharedGameEngine &&pGameEngine, State *pState, Renderer renderer ):
    Page( std::move( objContainers ) ), m_pGameEngine( std::move( pGameEngine ) ), m_Renderer( renderer ),
    m_CurrentState( CurrentState::START_PAGE )
{
    m_pMap->loadMap();
    m_pGameEngine->attachGlobalGameState( pState );
    initButtons( pState );
}

GamePage::~GamePage() {}

void GamePage::initButtons( State *pState )
{
    if( static_cast <size_t> ( Buttons::BUTTONS_QUANTITY ) == m_ButtonVc.size() )
    {
        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_START_PAGE_STATE ))->setAction( [] ()
        {
            std::cout << "Asking for pause() function\n";
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_CREATE_MAP_STATE ))->setAction( [pState, this] ()
        {
            pState->changeState( CurrentState::CREATE_MAP );
            m_CurrentState = CurrentState::CREATE_MAP;
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_START_PAGE_STATE ))->setAction( [pState, this] ()
        {
            pState->changeState( CurrentState::START_PAGE );
            m_CurrentState = CurrentState::START_PAGE;
        });
    }
    else if( static_cast <size_t> ( Buttons::BUTTONS_QUANTITY ) < m_ButtonVc.size() )
    {
        std::cerr << "GamePage button init error! Too many initialized buttons (" << m_ButtonVc.size() << ")\n";
        exit(-1);
    }
    else
    {
        std::cerr << "GamePage button init error! Too few initialized buttons\n";
        exit(-1);
    }
}

void GamePage::resize()
{
    Page::resize();
    m_pMap->changeSize();
    m_pGameEngine->resize();
    m_MapBordersRect = m_pMap->getMapBordersRect();
    m_MapBordersRect.x -= 1;
    m_MapBordersRect.y -= 1;
    m_MapBordersRect.w += 2;
    m_MapBordersRect.h += 2;
}

void GamePage::render()
{
    if( m_CurrentState != CurrentState::GAME )
    {
        m_CurrentState = CurrentState::GAME;
        m_pGameEngine->updateNumOfPlayers();
        resize();
    }
    Page::render();
    m_pGameEngine->gameAction();
    renderMapBorder();
}

void GamePage::renderMapBorder()
{
    SDL_SetRenderDrawColor( m_Renderer, Map::GREY.r, Map::GREY.g, Map::GREY.b, Map::GREY.a );
    SDL_RenderDrawRect( m_Renderer, &m_MapBordersRect );
}
