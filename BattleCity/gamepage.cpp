
#include <algorithm>
#include <iostream>

#include "gamepage.h"
#include "state.h"


GamePage::GamePage( InitContainers &&objContainers , pSharedGameEngine &&pGameEngine, State *pState, Renderer renderer ):
    Page( std::move( objContainers ) ), m_pState( pState ), m_pGameEngine( std::move( pGameEngine ) ), m_Renderer( renderer ),
    m_CurrentState( CurrentState::START_PAGE )
{
    m_pMap->loadMap();
    m_pGameEngine->attachGamePage( this );
    initButtons( pState );
}

GamePage::~GamePage() {}

void GamePage::initButtons( State *pState )
{
    if( static_cast <size_t> ( Buttons::BUTTONS_QUANTITY ) == m_ButtonVc.size() )
    {
        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_PAUSE ))->setAction( [this] ()
        {
            if( m_pGameEngine->isGamePaused() )
            {
                m_pGameEngine->resetPause();
            }
            else
            {
                m_pGameEngine->setPause();
            }
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_PREVIOUS_STATE ))->setAction( [pState, this] ()
        {
            if( m_pGameEngine->isGameOn() )
            {
                pState->lockPlayersQuantity();
                m_pGameEngine->setPause();
            }
            else
            {
                pState->unlockPlayersQuantity();
            }
            pState->changeState( m_PreviousState );
            m_CurrentState = m_PreviousState;
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
        m_PreviousState = m_pState->getPreviousState();

        if( !m_pGameEngine->isGamePaused() )
        {
            m_pGameEngine->gameStartInit();
            m_AudioChunk.play();
        }
        else
        {
            m_pGameEngine->resetPause();
        }
        resize();
    }
    Page::render();
    m_pGameEngine->gameAction();
    renderMapBorder();
    m_pGameEngine->render();

    if( m_PauseKey.keyClick() )
    {
        if( m_pGameEngine->isGamePaused() )
        {
            m_pGameEngine->resetPause();
        }
        else
        {
            m_pGameEngine->setPause();
        }
    }
}

void GamePage::renderMapBorder()
{
    SDL_SetRenderDrawColor( m_Renderer, Map::GREY.r, Map::GREY.g, Map::GREY.b, Map::GREY.a );
    SDL_RenderDrawRect( m_Renderer, &m_MapBordersRect );
}

void GamePage::changeState( CurrentState State )
{
    m_CurrentState = State;
    m_pState->changeState( State );
}

void GamePage::playStartSound()
{
    m_AudioChunk.play();
}

NumOfPlayers GamePage::getNumOfPlayers() const
{
    return m_pState->getNumOfPlayers();
}

uint32_t GamePage::getHighScore() const
{
    return m_pState->getHighScore();
}

void GamePage::setCurrentScore( uint32_t nCurrentScore )
{
    m_pState->setCurrentScore( nCurrentScore );
}
