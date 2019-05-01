
#include <algorithm>
#include <iostream>

#include "startpage.h"
#include "state.h"


StartPage::StartPage( InitContainers &&objContainers , State *pState): Page( std::move( objContainers ) ),
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
            pState->changeState( CurrentState::CREATE_MAP );
            m_CurrentState = CurrentState::CREATE_MAP;
        });

        m_ButtonVc.at( static_cast <size_t> ( Buttons::SET_GAME_STATE ))->setAction( [pState, this] ()
        {
            pState->changeState( CurrentState::GAME );
            m_CurrentState = CurrentState::GAME;
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
    }

    Page::render();
}
