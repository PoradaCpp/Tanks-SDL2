#include <cmath>

#include "texttexture.h"

TextTexture::TextTexture( TextTextureInitData TextInitData, Renderer renderer ): Texture( TextInitData, renderer ),
    m_Font( TextInitData.m_TextSurfaceInitData.m_Font ), m_sText( TextInitData.m_TextSurfaceInitData.m_sText ),
    m_TextColor( TextInitData.m_TextSurfaceInitData.m_TextColor ) {}

TextTexture::~TextTexture() {}

void TextTexture::setText( std::string sText )
{
    if( m_sText != sText )
    {
        if( m_sText.size() != sText.size() )
        {
            double dSymbolSize = m_RelativeDestRect.m_dWidth / m_sText.size();
            double dNewSize    = dSymbolSize * sText.size();
            double dSizeScale  = dNewSize / m_RelativeDestRect.m_dWidth;
            int nWidth = lround( m_DestRect.w * dSizeScale );

            switch( m_TextAlignment )
            {
            case TextAlignment::LEFT_SIDE:
                m_DestRect.w = lround( m_DestRect.w * dSizeScale );
                m_RelativeDestRect.m_dWidth = dNewSize;
                break;

            case TextAlignment::RIGHT_SIDE:
                m_DestRect.x = m_DestRect.x + m_DestRect.w - nWidth;
                m_DestRect.w = nWidth;
                m_RelativeDestRect.m_dX = m_RelativeDestRect.m_dX + m_RelativeDestRect.m_dWidth - dNewSize;
                m_RelativeDestRect.m_dWidth = dNewSize;
                break;

            case TextAlignment::CENTER:
                m_DestRect.x = m_DestRect.x + ( m_DestRect.w - nWidth ) / 2;
                m_DestRect.w = nWidth;
                m_RelativeDestRect.m_dX = m_RelativeDestRect.m_dX + ( m_RelativeDestRect.m_dWidth - dNewSize ) / 2;
                m_RelativeDestRect.m_dWidth = dNewSize;
                break;
            }
        }
        m_sText = sText;
        Uint8 alpha = getAlpha();
        Surface surface({ m_Font, m_sText, m_TextColor });
        makeTextureFromSurface( surface );
        setAlpha( alpha );
    }
}

void TextTexture::setAlignment( TextAlignment textAlignment )
{
    m_TextAlignment = textAlignment;
}

size_t TextTexture::getTextSize() const
{
    return m_sText.size();
}
