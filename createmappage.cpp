
#include <algorithm>
#include <iostream>

#include "createmappage.h"
#include "state.h"


CreateMapPage::CreateMapPage(InitContainers &&objContainers , Renderer renderer): Page( std::move( objContainers ) ),
    m_Renderer( renderer ) {}

CreateMapPage::~CreateMapPage() {}

void CreateMapPage::changeState( State *state )
{
    if( !m_ButtonVc.empty() )
    {
        if( ButtonState::RELEASED == m_ButtonVc[ static_cast <size_t> ( Button::START )]->getState() )
        {

        }

        if( ButtonState::RELEASED == m_ButtonVc[ static_cast <size_t> ( Button::MAIN_PAGE ) ]->getState() )
        {
            state->changeState( CurrentState::START_PAGE );
        }

        if( ButtonState::RELEASED == m_ButtonVc[ static_cast <size_t> ( Button::BRICK_WALL ) ]->getState() )
        {
            ImgTextureInitData InitData( "media/images/BrickWall.png", { 85.5, 8,  6.5, 11.5 } );
            pMapUnit = std::make_shared < BuildMap > ( InitData, m_Renderer );
        }
    }
}

void CreateMapPage::resize()
{
    if( !m_DisplayedObjVc.empty() )
    {
        std::for_each( m_DisplayedObjVc.begin(), m_DisplayedObjVc.end(), [ this ] ( pSharedDisplayedObj &pDisplayedObj )
        {
            pDisplayedObj->changeSize();
        });
    }

    if( pMapUnit )
    {
        pMapUnit->changeSize();
    }
}

void CreateMapPage::render()
{
    if( !m_DynObjVc.empty() )
    {
        std::for_each( m_DynObjVc.begin(), m_DynObjVc.end(), [] ( pSharedDynObj &DynObj )
        {
            DynObj->changePosition();
        });
    }

    if( !m_DisplayedObjVc.empty() )
    {
        std::for_each( m_DisplayedObjVc.begin(), m_DisplayedObjVc.end(), [] ( pSharedDisplayedObj &pDisplayedObj )
        {
            pDisplayedObj->render();
        });
    }

    if( pMapUnit )
    {
        pMapUnit->changePosition();
        pMapUnit->render();
    }
}
