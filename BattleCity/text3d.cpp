#include "text3d.h"

Text3D::Text3D( Text3DInitData text3DInitData, Renderer renderer ):
    DisplayedObject(),
    m_TopTexture         ( text3DInitData.TopTextInitData,    renderer ),
    m_BottomTexture      ( text3DInitData.BottomTextInitData, renderer ),
    m_nTopTextureShiftX  ( text3DInitData.m_nTopTextureShiftX          ),
    m_nTopTextureShiftY  ( text3DInitData.m_nTopTextureShiftY          )
{
    m_TopTexture.setBlendMode();
    m_TopTexture.setAlpha( text3DInitData.m_nTopTextureAlpha );

    SDL_Rect TextureDestRect = m_TopTexture.getDestination();
    TextureDestRect.x += m_nTopTextureShiftX;
    TextureDestRect.y += m_nTopTextureShiftY;
    m_TopTexture.setDestination( TextureDestRect );

    m_BottomTexture.setBlendMode();
    m_BottomTexture.setAlpha( text3DInitData.m_nBottomTextureAlpha );
}

Text3D::~Text3D() {}

void Text3D::setTopTextureAlpha( Uint8 Alpha )
{
    m_TopTexture.setAlpha( Alpha );
}

void Text3D::setBottomTextureAlpha( Uint8 Alpha )
{
    m_BottomTexture.setAlpha( Alpha );
}

void Text3D::setTopTextureShift( int nDx, int nDy )
{
    m_nTopTextureShiftX = nDx;
    m_nTopTextureShiftY = nDy;
}

void Text3D::setText( std::string sText )
{
    m_BottomTexture.setText( sText );
    m_TopTexture.setText( sText );

    if( m_BottomTexture.getTextSize() != sText.size() )
    {
        SDL_Rect DestRect = m_BottomTexture.getDestination();
        DestRect.x += m_nTopTextureShiftX;
        DestRect.y += m_nTopTextureShiftY;

        m_TopTexture.setDestination( DestRect );
    }
}

void Text3D::setAlignment( TextTexture::TextAlignment textAlignment )
{
    m_BottomTexture.setAlignment( textAlignment );
    m_TopTexture.setAlignment( textAlignment );
}

void Text3D::setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect )
{
    m_BottomTexture.setRelativeDestination( RelativeDestRect, RelativeBaseRect );

    SDL_Rect DestRect = m_BottomTexture.getDestination();
    DestRect.x += m_nTopTextureShiftX;
    DestRect.y += m_nTopTextureShiftY;

    m_TopTexture.setDestination( DestRect );
}

void Text3D::setDestination( SDL_Rect DestRect )
{
    m_BottomTexture.setDestination( DestRect );

    SDL_Rect rect = m_BottomTexture.getDestination();
    rect.x += m_nTopTextureShiftX;
    rect.y += m_nTopTextureShiftY;

    m_TopTexture.setDestination( rect );
}

void Text3D::setPosition( int nX, int nY )
{
    m_BottomTexture.setPosition( nX, nY );

    int x = nX + m_nTopTextureShiftX;
    int y = nY + m_nTopTextureShiftY;

    m_TopTexture.setPosition( x, y );
}

void Text3D::changeSize()
{
    m_BottomTexture.changeSize();

    SDL_Rect DestRect = m_BottomTexture.getDestination();
    DestRect.x += m_nTopTextureShiftX;
    DestRect.y += m_nTopTextureShiftY;

    m_TopTexture.setDestination( DestRect );
}

void Text3D::render()
{
    m_BottomTexture.render();
    m_TopTexture.render();
}
