#include <iostream>

#include "button.h"

Button::Button( ButtonInitData InitData, Renderer renderer ):
    ButtonBase(),
    m_ImageTexture( InitData.m_ImgInitData, renderer ),
    m_State( ButtonState::DEFAULT ),
    m_CursorPos({ 0, 0 }), m_fLock( false )
{
    m_DestImgRect = m_ImageTexture.getDestination();
    setRenderDimensions();
}

Button::Button( ImgTextureInitData InitData, Renderer renderer ):
    ButtonBase(),
    m_ImageTexture( InitData, renderer ),
    m_State( ButtonState::DEFAULT ),
    m_CursorPos({ 0, 0 }), m_fLock( false )
{
    m_DestImgRect = m_ImageTexture.getDestination();
    setRenderDimensions();
}

Button::~Button() {}

void Button::setRenderDimensions()
{
    int nWidht = m_ImageTexture.getWidth();
    int nHeight = m_ImageTexture.getHeight();
    int nSrcRectHeight = nHeight / static_cast <int > ( m_SourceRectArr.size() );

    for( int i = 0, nArrSize = static_cast <int > ( m_SourceRectArr.size() ); i < nArrSize; ++i)
    {
        m_SourceRectArr.at( static_cast <size_t> ( i ) ) = { 0, nSrcRectHeight * i, nWidht, nSrcRectHeight };
    }
}

ButtonState Button::getState()
{
    if ( inFocus() & SDL_BUTTON( SDL_BUTTON_LEFT ))
    {
        m_State = ButtonState::PRESSED;
    }
    else if( ButtonState::PRESSED == m_State )
    {
        m_State = ButtonState::RELEASED;
    }

    return m_State;
}

Uint32 Button::inFocus()
{
    Uint32 nCurrentButtonState( SDL_GetMouseState( &m_CursorPos.x, &m_CursorPos.y ) );

    if( SDL_PointInRect( &m_CursorPos, &m_DestImgRect ) && !m_fLock )
    {
        if( m_State != ButtonState::PRESSED )
        {
            m_State = ButtonState::IN_FOCUS;
        }
        return nCurrentButtonState;
    }
    else
    {
        m_State = ButtonState::DEFAULT;
        return 0;
    }
}

void Button::setAction( Action action )
{
    m_Action = action;
}

void Button::execute()
{
    getState();

    if( ButtonState::RELEASED == m_State && m_Action && !m_fLock )
    {
        m_Action();
    }
}

void Button::lock()
{
    m_fLock = true;
}

void Button::unlock()
{
    m_fLock = false;
}

void Button::changeSize()
{
    m_ImageTexture.changeSize();
    m_DestImgRect = m_ImageTexture.getDestination();
}

void Button::render()
{
    m_ImageTexture.render( &m_SourceRectArr.at( static_cast <size_t> ( m_State != ButtonState::RELEASED ? m_State : ButtonState::IN_FOCUS ) ));
}

