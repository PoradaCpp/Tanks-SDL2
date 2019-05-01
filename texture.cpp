#include <iostream>
#include <cmath>

#include "texture.h"

Texture::Texture( Renderer renderer ): DisplayedObject(), m_pTexture( nullptr ), m_Renderer( renderer ), m_nSrcHeight( 0 ),
    m_nSrcWidth( 0 ), m_nPageHeight( 0 ), m_nPageWidth( 0 ), m_DestRect({ 0, 0, 0, 0 }), m_BaseRect({ 0, 0, 0, 0 }),
    m_fBaseGiven( false ) {}

Texture::Texture( ImgTextureInitData ImgInitData, Renderer renderer ): DisplayedObject(), m_pTexture( nullptr ),
    m_Renderer( renderer ), m_RelativeDestRect( ImgInitData.m_DestRect ), m_DestRect({ 0, 0, 0, 0 }),
    m_RelativeBaseRect( ImgInitData.m_BaseRect ), m_BaseRect({ 0, 0, 0, 0 })
{
    m_fBaseGiven = m_RelativeBaseRect.relativeRectNotEmpty();

    changeSize();

    Surface surface( ImgInitData.m_sImgPath );
    makeTextureFromSurface( surface );
}

Texture::Texture( TextTextureInitData TextInitData, Renderer renderer): DisplayedObject(), m_pTexture( nullptr ),
    m_Renderer( renderer ), m_RelativeDestRect( TextInitData.m_DestRect ), m_DestRect({ 0, 0, 0, 0 }),
    m_RelativeBaseRect( TextInitData.m_BaseRect ), m_BaseRect({ 0, 0, 0, 0 })
{
    m_fBaseGiven = m_RelativeBaseRect.relativeRectNotEmpty();

    changeSize();

    Surface surface( TextInitData.m_TextSurfaceInitData );
    makeTextureFromSurface( surface );
}

Texture::~Texture()
{
    free();
}

Renderer Texture::getRenderer()
{
    return m_Renderer;
}

void Texture::shareInstantTexture( Texture texture )
{
    m_pTexture = texture.m_pTexture;
    m_nSrcWidth = texture.getWidth();
    m_nSrcHeight = texture.getHeight();
}

pSharedSDLTexture Texture::makeTextureFromSurface( Surface &surface , Renderer &renderer ) const
{
    pSharedSDLTexture temp_pTexture( SDL_CreateTextureFromSurface( renderer, surface ),
                                  [] ( SDL_Texture *texture ) { SDL_DestroyTexture( texture ); });

    if( !temp_pTexture )
    {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << '\n';
        std::exit(-1);
    }

    return temp_pTexture;
}

void Texture::makeTextureFromSurface( Surface &surface )
{
    m_pTexture = makeTextureFromSurface( surface, m_Renderer );

    m_nSrcWidth = surface.getWidth();
    m_nSrcHeight = surface.getHeight();
}

void Texture::setBlendMode( SDL_BlendMode BlendMode )
{
    SDL_SetTextureBlendMode( m_pTexture.get(), BlendMode );
}

void Texture::setAlpha( Uint8 Alpha )
{
    SDL_SetTextureAlphaMod( m_pTexture.get(), Alpha );
}

void Texture::changeAlpha( Sint8 dAlpha )
{
    Uint8 Alpha( 0 );
    SDL_GetTextureAlphaMod( m_pTexture.get(), &Alpha );

    Sint16 NextAlpha = Alpha + dAlpha;

    if( NextAlpha > 255 )
    {
        Alpha = 255;
    }
    else if( NextAlpha < 0 )
    {
        Alpha = 0;
    }
    else
    {
        Alpha = NextAlpha;
    }

    SDL_SetTextureAlphaMod( m_pTexture.get(), Alpha );
}

Uint8 Texture::getAlpha()
{
    Uint8 Alpha( 0 );
    SDL_GetTextureAlphaMod( m_pTexture.get(), &Alpha );

    return Alpha;
}

void Texture::free()
{
    if( m_pTexture )
    {
        m_pTexture.reset();

        m_DestRect = { 0, 0, 0, 0 };

        m_nSrcWidth = 0;
        m_nSrcHeight = 0;
    }
}

SDL_Rect Texture::getDestination() const
{
    return m_DestRect;
}

SDL_Point Texture::getPosition() const
{
    SDL_Point TempPoint( { m_DestRect.x, m_DestRect.y } );

    return TempPoint;
}

RelativeRect Texture::getBase() const
{
    return m_RelativeBaseRect;
}

void Texture::setDestination( SDL_Rect DestRect )
{
    m_DestRect = DestRect;
}

void Texture::setPosition( int nX, int nY )
{
    m_DestRect.x = nX;
    m_DestRect.y = nY;
}

void Texture::setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect )
{
    if( RelativeBaseRect.relativeRectNotEmpty() )
    {
        m_fBaseGiven = true;
        m_RelativeBaseRect = RelativeBaseRect;
    }

    m_RelativeDestRect = RelativeDestRect;
    changeSize();
}

void Texture::setRelativePosition( double dX, double dY )
{
    m_RelativeDestRect.m_dX = dX;
    m_RelativeDestRect.m_dY = dY;
    changeSize();
}

void Texture::changeSize()
{
    if( SDL_GetRendererOutputSize( m_Renderer, &m_nPageWidth, &m_nPageHeight ) < 0 )
    {
        std::cerr << "Cannot receive the window size! SDL Error: " << SDL_GetError() << '\n';
        exit( -1 );
    }

    RelativeRect RelativeDestRect = m_RelativeDestRect;

    if( m_fBaseGiven )
    {
        calcDestFromRelative( m_RelativeBaseRect, m_BaseRect );

        RelativeRect RelativeBaseRect( ( m_BaseRect.x * 100.0 ) / m_nPageWidth, ( m_BaseRect.y * 100.0 ) / m_nPageHeight,
                                       ( m_BaseRect.w * 100.0 ) / m_nPageWidth, ( m_BaseRect.h * 100.0 ) / m_nPageHeight );

        double dXScale = RelativeBaseRect.m_dWidth / m_RelativeBaseRect.m_dWidth;
        double dYScale = RelativeBaseRect.m_dHeight / m_RelativeBaseRect.m_dHeight;
        double dXCenter = RelativeBaseRect.m_dX + RelativeBaseRect.m_dWidth / 2;
        double dYCenter = RelativeBaseRect.m_dY + RelativeBaseRect.m_dHeight / 2;

        RelativeDestRect.m_dWidth *= dXScale;
        RelativeDestRect.m_dHeight *= dYScale;
        RelativeDestRect.m_dX = dXCenter - ( dXCenter - RelativeDestRect.m_dX ) * dXScale;
        RelativeDestRect.m_dY = dYCenter - ( dYCenter - RelativeDestRect.m_dY ) * dYScale;
    }

    calcDestFromRelative( RelativeDestRect, m_DestRect );
}

int Texture::getHeight()
{
    return m_nSrcHeight;
}

int Texture::getWidth()
{
    return m_nSrcWidth;
}

void Texture::render()
{
    SDL_Rect *DestRect = nullptr;

    if( !SDL_RectEmpty( &m_DestRect ))
    {
        DestRect = &m_DestRect;
    }

    SDL_RenderCopyEx( m_Renderer, m_pTexture.get(), nullptr, DestRect, 0, nullptr, SDL_FLIP_NONE );
}

void Texture::render( SDL_Rect *SourceRect, SDL_Rect *DestRect, double dAngle, SDL_Point *Center, SDL_RendererFlip Flip )
{
    if( !DestRect && !SDL_RectEmpty( &m_DestRect ))
    {
        DestRect = &m_DestRect;
    }

    SDL_RenderCopyEx( m_Renderer, m_pTexture.get(), SourceRect, DestRect, dAngle, Center, Flip );
}

Texture::operator bool ()
{
    return m_pTexture.operator bool();
}

void Texture::calcDestFromRelative(const RelativeRect &RelativeDestRect, SDL_Rect &DestRect )
{
    double dDestRectHeight = ( RelativeDestRect.m_dHeight * m_nPageHeight ) / 100;
    double dDestRectWidth = ( RelativeDestRect.m_dWidth * m_nPageWidth ) / 100;

    if( !SDL_RectEmpty( &DestRect ) )
    {
        double dHeightToWidthRatio = ( DestRect.h * 1.0 ) / DestRect.w;

        if( ( dDestRectHeight  / DestRect.h ) > ( dDestRectWidth / DestRect.w ) )
        {
            DestRect.w = dDestRectWidth;
            DestRect.h = std::round( dDestRectWidth * dHeightToWidthRatio );
        }
        else
        {
            DestRect.h = dDestRectHeight;
            DestRect.w = std::round( dDestRectHeight / dHeightToWidthRatio );
        }
    }
    else
    {
        DestRect.h = dDestRectHeight;
        DestRect.w = dDestRectWidth;
    }

    double RelativeDestRectWidht = ( DestRect.w * 100.0 ) / m_nPageWidth;
    double RelativeDestRectHeight = ( DestRect.h * 100.0 ) / m_nPageHeight;

    DestRect.x = std::round( (( RelativeDestRect.m_dX + ( RelativeDestRect.m_dWidth - RelativeDestRectWidht ) / 2 ) *
                                m_nPageWidth ) / 100 );
    DestRect.y = std::round( (( RelativeDestRect.m_dY + ( RelativeDestRect.m_dHeight - RelativeDestRectHeight ) / 2 ) *
                                m_nPageHeight ) / 100 );
}
