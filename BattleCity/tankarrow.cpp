#include "tankarrow.h"

TankArrow::TankArrow( TankArrowInitData InitData, Renderer renderer, SDL_Scancode Key1, SDL_Scancode Key2 ): ButtonBase(),
     m_Texture( InitData.m_TextureInitData, renderer ), m_AudioChunk( InitData.m_sAudioPath ),
     m_OnePlayerPos( InitData.m_TextureInitData.m_DestRect ), m_TwoPlayersPos( InitData.m_TwoPlayersPos ),
     m_ScancodeArr{ { Key1, Key2 } }, m_CurNumOfPlayers( NumOfPlayers::ONE_PLAYER ),
     m_CurPos( InitData.m_TextureInitData.m_DestRect ), m_fNumOfPlayersChanged( true ), m_fLock( false ) {}

TankArrow::~TankArrow() {}

void TankArrow::changePosition()
{
    if( !m_fLock )
    {
        const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );

        if( NumOfPlayers::ONE_PLAYER == m_CurNumOfPlayers )
        {
            if( currentKeyStates[ m_ScancodeArr.at(0) ] )
            {
                m_CurPos = m_TwoPlayersPos;
                m_CurNumOfPlayers = NumOfPlayers::TWO_PLAYERS;
                m_fNumOfPlayersChanged = true;
                m_AudioChunk.play();
            }
        }
        else
        {
            if( currentKeyStates[ m_ScancodeArr.at(1) ] )
            {
                m_CurPos = m_OnePlayerPos;
                m_CurNumOfPlayers = NumOfPlayers::ONE_PLAYER;
                m_fNumOfPlayersChanged = true;
                m_AudioChunk.play();
            }
        }
        m_Texture.setRelativeDestination( m_CurPos );
    }
}

void TankArrow::setAction( Action action )
{
    m_Action = action;
}

void TankArrow::execute()
{
    if( m_fNumOfPlayersChanged && m_Action )
    {
        m_Action();
        m_fNumOfPlayersChanged = false;
    }
}

void TankArrow::lock()
{
    m_fLock = true;
}

void TankArrow::unlock()
{
    m_fLock = false;
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
