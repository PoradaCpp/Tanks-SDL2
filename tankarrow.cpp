#include "tankarrow.h"

TankArrowMoveAlgo::TankArrowMoveAlgo( RelativeRect StartPos, RelativeRect AnotherPos, SDL_Scancode Key1,
                                      SDL_Scancode Key2 ): MoveAlgorithm(), m_StartPos( StartPos ),
    m_AnotherPos( AnotherPos ), m_ScancodeArr{ { Key1, Key2 } }, m_CurPos( CurrentPosition::START ) {}

TankArrowMoveAlgo::~TankArrowMoveAlgo() {}

void TankArrowMoveAlgo::move( RelativeRect &CurPos )
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    if( CurrentPosition::START == m_CurPos )
    {
        if( currentKeyStates[ m_ScancodeArr.at(0) ] )
        {
            CurPos = m_AnotherPos;
            m_CurPos = CurrentPosition::ANOTHER;
        }
    }
    else
    {
        if( currentKeyStates[ m_ScancodeArr.at(1) ] )
        {
            CurPos = m_StartPos;
            m_CurPos = CurrentPosition::START;
        }
    }
}

void TankArrowMoveAlgo::move( SDL_Rect &CurPos )
{
    CurPos = { 0, 0, 0, 0 };
}


TankArrow::TankArrow( TankArrowInitData InitData, Renderer renderer ): DynamicObject(),
     m_Texture( InitData.m_TextureInitData, renderer ), m_CurPos( InitData.m_TextureInitData.m_DestRect )
{
    m_pMoveAlgorithm = std::make_shared < TankArrowMoveAlgo > ( InitData.m_TextureInitData.m_DestRect, InitData.m_AnotherPos );
}

TankArrow::~TankArrow() {}

void TankArrow::changePosition()
{
    m_pMoveAlgorithm->move( m_CurPos );

    m_Texture.setRelativeDestination( m_CurPos );
}

void TankArrow::changeSize()
{
    m_Texture.changeSize();
}

void TankArrow::render()
{
    m_Texture.render();
}
