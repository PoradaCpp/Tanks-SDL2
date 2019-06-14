#include <iostream>

#include "buttonnamed.h"

ButtonNamed::ButtonNamed( ButtonInitData InitData, Renderer renderer ):
    Button( InitData.m_ImgInitData, renderer ),
    m_RelativeDestRect( InitData.m_ImgInitData.m_DestRect ),
    m_NameText3D( InitData.m_Text3DInitData, renderer )
{
    m_DestImgRect = m_ImageTexture.getDestination();
    setRenderDimensions();
}

ButtonNamed::~ButtonNamed() {}

void ButtonNamed::setRenderDimensions()
{
    int nWidht = m_ImageTexture.getWidth();
    int nHeight = m_ImageTexture.getHeight();
    int nSrcRectHeight = nHeight /static_cast <int> ( m_SourceRectArr.size() );

    for( int i = 0, nArrSize = static_cast <int> ( m_SourceRectArr.size() ); i < nArrSize; ++i)
    {
        m_SourceRectArr.at( static_cast <size_t> ( i ) ) = { 0, nSrcRectHeight * i, nWidht, nSrcRectHeight };
    }

    double dIndentX = m_RelativeDestRect.m_dWidth / 10;
    double dIndentY = m_RelativeDestRect.m_dHeight / 8;

    m_RelativeDestTxtRect = { m_RelativeDestRect.m_dX + dIndentX, m_RelativeDestRect.m_dY + dIndentY,
                              m_RelativeDestRect.m_dWidth - dIndentX * 2, m_RelativeDestRect.m_dHeight - dIndentY * 2 };

    m_NameText3D.setRelativeDestination( m_RelativeDestTxtRect, m_ImageTexture.getRelativeBase() );
}

void ButtonNamed::changeSize()
{
    m_ImageTexture.changeSize();
    m_NameText3D.changeSize();

    m_DestImgRect = m_ImageTexture.getDestination();
}

void ButtonNamed::render()
{
    m_ImageTexture.render( &m_SourceRectArr.at( static_cast <size_t> ( m_State != ButtonState::RELEASED ? m_State : ButtonState::IN_FOCUS ) ));
    m_NameText3D.render();
}
