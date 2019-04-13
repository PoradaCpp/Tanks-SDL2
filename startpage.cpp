
#include <algorithm>
#include <iostream>

#include "startpage.h"
#include "state.h"


StartPage::StartPage( InitContainers &&objContainers ): Page( std::move( objContainers ) ) {}

StartPage::~StartPage() {}

void StartPage::changeState( State *state )
{
    if( !m_ButtonVc.empty() )
    {
        if( ButtonState::RELEASED == m_ButtonVc[ static_cast <size_t> ( Button::CREATE_MAP )]->getState() )
        {
            state->changeState( CurrentState::CREATING_MAP );
        }

        if( ButtonState::RELEASED == m_ButtonVc[ static_cast <size_t> ( Button::START ) ]->getState() )
        {

        }
    }
}

