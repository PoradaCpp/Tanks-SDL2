#include "tankarrow.h"

TankArrowMoveAlgo::TankArrowMoveAlgo( Texture texture, RelativeRect StartPos, RelativeRect AnotherPos, SDL_Scancode Key1,
                                      SDL_Scancode Key2 ): MoveAlgorithm(), m_Texture( texture ), m_StartPos( StartPos),
    m_AnotherPos( AnotherPos ), m_ScancodeArr{ { Key1, Key2 } }, m_CurPos( CurrentPosition::START ) {}

TankArrowMoveAlgo::~TankArrowMoveAlgo() {}

void TankArrowMoveAlgo::move( SDL_Rect &CurPos )
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    if( CurrentPosition::START == m_CurPos )
    {
        if( currentKeyStates[ m_ScancodeArr[0] ] )
        {
            m_Texture.setRelativeDestination( m_AnotherPos );
            CurPos = m_Texture.getDestination();

            m_CurPos = CurrentPosition::ANOTHER;
        }
    }
    else
    {
        if( currentKeyStates[ m_ScancodeArr[1] ] )
        {
            m_Texture.setRelativeDestination( m_StartPos );
            CurPos = m_Texture.getDestination();

            m_CurPos = CurrentPosition::START;
        }
    }
}


TankArrow::TankArrow( TankArrowInitData InitData, Renderer renderer ): DynamicObject(),
     m_Texture( InitData.m_TextureInitData, renderer )
{
    m_pMoveAlgorithm = std::make_shared < TankArrowMoveAlgo > ( m_Texture, InitData.m_TextureInitData.m_DestRect,
                                                                InitData.m_AnotherPos );
}

TankArrow::~TankArrow() {}

void TankArrow::changePosition()
{
    m_pMoveAlgorithm->move( m_CurPos );

    m_Texture.setDestination( m_CurPos );
}

void TankArrow::changeSize()
{
    m_Texture.changeSize();

    m_CurPos = m_Texture.getDestination();
}

void TankArrow::render()
{
    m_Texture.render();
}
