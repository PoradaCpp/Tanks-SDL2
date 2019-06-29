
#include <algorithm>
#include <iostream>

#include "startpage.h"
#include "state.h"


StartPage::StartPage( InitContainers &&objContainers, State *pState): Page( std::move( objContainers ) ), m_pState( pState ),
    m_CurrentState( CurrentState::CREATE_MAP )
{
    initButtons( pState );
}

StartPage::~StartPage() {}

void StartPage::initButtons( State *pState )
{
    if( static_cast <size_t> ( Buttons::BUTTONS_QUANTITY ) == m_ButtonVc.size() )
    {
        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_CREATE_MAP_STATE ))->setAction( [pState, this] ()
        {
            m_AudioChunk.stop();
            pState->changeState( CurrentState::CREATE_MAP );
            m_CurrentState = CurrentState::CREATE_MAP;
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_GAME_STATE ))->setAction( [pState, this] ()
        {
            m_AudioChunk.stop();
            pState->changeState( CurrentState::GAME );
            m_CurrentState = CurrentState::GAME;
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_PLAYERS_QUANTITY ))->setAction( [pState] ()
        {
            pState->setPlayersQuantity( NumOfPlayers::ONE_PLAYER == pState->getNumOfPlayers() ? NumOfPlayers::TWO_PLAYERS :
                                                                                                NumOfPlayers::ONE_PLAYER );
        });
    }
    else if( static_cast <size_t> ( Buttons::BUTTONS_QUANTITY ) < m_ButtonVc.size() )
    {
        std::cerr << "StartPage button init error! Too many initialized buttons\n";
        exit(-1);
    }
    else
    {
        std::cerr << "StartPage button init error! Too few initialized buttons\n";
        exit(-1);
    }
}

void StartPage::render()
{
    if( m_CurrentState != CurrentState::START_PAGE )
    {
        m_CurrentState = CurrentState::START_PAGE;
        resize();

        m_AudioChunk.play( AudioChunk::INFINITELY_PLAYING );

        if( m_pState->isPlayersQuantityLocked() )
        {
            m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_PLAYERS_QUANTITY ))->lock();
        }
        else
        {
            m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_PLAYERS_QUANTITY ))->unlock();
        }
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );

    if( currentKeyStates[ KEY_ENTER_KP ] || currentKeyStates[ KEY_ENTER ] || currentKeyStates[ KEY_SPACE ] )
    {
        m_pState->changeState( CurrentState::GAME );
    }

    Page::render();
}
