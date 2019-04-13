#include <iostream>

#include "button.h"

Button::Button( ButtonInitData InitData, Renderer renderer ):
    DisplayedObject(),
    m_RelativeDestRect( InitData.m_ImgInitData.m_DestRect ),
    m_ImageTexture( InitData.m_ImgInitData, renderer ),
    m_NameText3D( InitData.m_Text3DInitData, renderer ),
    m_State( ButtonState::DEFAULT )
{
    m_DestImgRect = m_ImageTexture.getDestination();
    setRenderDimensions();
}

Button::Button( Texture ImageTexture, Text3D ButtonName, RelativeRect DestRect, RelativeRect BaseRect ):
    DisplayedObject(),
    m_RelativeDestRect( DestRect ),
    m_ImageTexture( ImageTexture ),
    m_NameText3D( ButtonName ),
    m_State( ButtonState::DEFAULT )
{
    m_ImageTexture.setRelativeDestination( DestRect, BaseRect );

    m_DestImgRect = m_ImageTexture.getDestination();
    setRenderDimensions();
}

Button::Button( Texture ImageTexture, Text3D ButtonName, ButtonDestinationData InitData ):
    DisplayedObject(),
    m_RelativeDestRect( InitData.m_RelativeDestRect ),
    m_ImageTexture( ImageTexture ),
    m_NameText3D( ButtonName ),
    m_State( ButtonState::DEFAULT )
{
    m_ImageTexture.setRelativeDestination( InitData.m_RelativeDestRect, InitData.m_RelativeBaseRect );

    m_DestImgRect = m_ImageTexture.getDestination();
    setRenderDimensions();
}

Button::~Button() {}

void Button::setRenderDimensions()
{
    int nWidht = m_ImageTexture.getWidth();
    int nHeight = m_ImageTexture.getHeight();
    int nSrcRectHeight = nHeight / m_RectArr.size();

    for( int i = 0, nArrSize = m_RectArr.size(); i < nArrSize; ++i)
    {
        m_RectArr[i] = { 0, nSrcRectHeight * i, nWidht, nSrcRectHeight };
    }

    double dIndentX = m_RelativeDestRect.m_dWidth / 10;
    double dIndentY = m_RelativeDestRect.m_dHeight / 8;

    m_RelativeDestTxtRect = { m_RelativeDestRect.m_dX + dIndentX, m_RelativeDestRect.m_dY + dIndentY,
                              m_RelativeDestRect.m_dWidth - dIndentX * 2, m_RelativeDestRect.m_dHeight - dIndentY * 2 };

    m_NameText3D.setRelativeDestination( m_RelativeDestTxtRect );
}

ButtonState Button::getState()
{
    int nCursorX(0), nCursorY(0);

    if ( inFocus( nCursorX, nCursorY ) & SDL_BUTTON( SDL_BUTTON_LEFT ))
    {
        m_State = ButtonState::PRESSED;

    }
    else if( ButtonState::PRESSED == m_State )
    {
        m_State = ButtonState::RELEASED;
    }

    return m_State;
}

Uint32 Button::inFocus( int &nCursorX, int &nCursorY )
{
    Uint32 nCurrentButtonState = SDL_GetMouseState( &nCursorX, &nCursorY );
    SDL_Point CursorPos = { nCursorX, nCursorY };

    if( SDL_PointInRect( &CursorPos, &m_DestImgRect ) )
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

void Button::changeSize()
{
    m_ImageTexture.changeSize();
    m_NameText3D.changeSize();

    m_DestImgRect = m_ImageTexture.getDestination();
}

void Button::render()
{
    m_ImageTexture.render( &m_RectArr[ static_cast <size_t> ( m_State != ButtonState::RELEASED ? m_State : ButtonState::IN_FOCUS ) ]);
    m_NameText3D.render();
}
