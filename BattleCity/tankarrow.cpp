#include "tankarrow.h"

TankArrow::TankArrow( TankArrowInitData InitData, Renderer renderer, SDL_Scancode Key1, SDL_Scancode Key2 ): DisplayedObject(),
     m_Texture( InitData.m_TextureInitData, renderer ), m_OnePlayerPos( InitData.m_TextureInitData.m_DestRect ),
     m_TwoPlayersPos( InitData.m_TwoPlayersPos ), m_ScancodeArr{ { Key1, Key2 } }, m_CurNumOfPlayers( NumOfPlayers::ONE_PLAYER ),
     m_CurPos( InitData.m_TextureInitData.m_DestRect ) {}

TankArrow::~TankArrow() {}

void TankArrow::changePosition()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );

    if( NumOfPlayers::ONE_PLAYER == m_CurNumOfPlayers )
    {
        if( currentKeyStates[ m_ScancodeArr.at(0) ] )
        {
            m_CurPos = m_TwoPlayersPos;
            m_CurNumOfPlayers = NumOfPlayers::TWO_PLAYERS;
        }
    }
    else
    {
        if( currentKeyStates[ m_ScancodeArr.at(1) ] )
        {
            m_CurPos = m_OnePlayerPos;
            m_CurNumOfPlayers = NumOfPlayers::ONE_PLAYER;
        }
    }

    m_Texture.setRelativeDestination( m_CurPos );
}

void TankArrow::changeSize()
{
    m_Texture.changeSize();
}

void TankArrow::render()
{
    changePosition();
    m_Texture.render();
}
