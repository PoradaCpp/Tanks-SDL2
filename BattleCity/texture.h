#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>

#include "surface.h"
#include "renderer.h"
#include "displayedobject.h"

struct RelativeRect
{
    double m_dX;
    double m_dY;
    double m_dWidth;
    double m_dHeight;

    RelativeRect( double dX = 0, double dY = 0, double dWidth = 0, double dHeight = 0 ):
        m_dX     ( dX      > 100 ? 100 : ( dX      < -100 ? -100 : dX      )),
        m_dY     ( dY      > 100 ? 100 : ( dY      < -100 ? -100 : dY      )),
        m_dWidth ( dWidth  > 100 ? 100 : ( dWidth  < -100 ? -100 : dWidth  )),
        m_dHeight( dHeight > 100 ? 100 : ( dHeight < -100 ? -100 : dHeight )) {}

    bool relativeRectNotEmpty()
    {
        return ( m_dHeight * m_dWidth ) > 0;
    }
};

struct ImgTextureInitData
{
    std::string m_sImgPath;
    RelativeRect m_DestRect;
    RelativeRect m_BaseRect;

    ImgTextureInitData( std::string sImgPath, RelativeRect DestRect = { 0, 0, 0, 0 }, RelativeRect BaseRect = { 0, 0, 0, 0 } ):
        m_sImgPath( sImgPath ), m_DestRect( DestRect ), m_BaseRect( BaseRect ) {}
};

struct TextTextureInitData
{
    TextSurfaceInitData m_TextSurfaceInitData;

    RelativeRect m_DestRect;
    RelativeRect m_BaseRect;

    TextTextureInitData( Font font = Font(), std::string sText = std::string(), SDL_Color TextColor = { 0, 0, 0, 0 },
                         RelativeRect DestRect = { 0, 0, 0, 0 }, RelativeRect BaseRect = { 0, 0, 0, 0 } ):
        m_TextSurfaceInitData( font, sText, TextColor ), m_DestRect( DestRect ), m_BaseRect( BaseRect ) {}
};

typedef std::shared_ptr < SDL_Texture > pSharedSDLTexture;

class Texture: public DisplayedObject
{
public:
    explicit Texture( Renderer renderer );
    Texture( ImgTextureInitData ImgInitData, Renderer renderer );
    Texture( TextTextureInitData TextInitData, Renderer renderer );

    ~Texture() override;

    Renderer getRenderer();
    void shareInstantTexture( Texture texture );
    void makeTextureFromSurface( Surface &surface );
    void setBlendMode( SDL_BlendMode BlendMode = SDL_BLENDMODE_BLEND );
    void setAlpha( Uint8 Alpha );
    void changeAlpha( Sint8 dAlpha );
    Uint8 getAlpha();
    void free();

    SDL_Rect getDestination() const;
    SDL_Point getPosition() const;
    SDL_Rect getBase() const;
    RelativeRect getRelativeBase() const;
    RelativeRect getRelativeDestination() const;
    void setDestination( SDL_Rect DestRect );
    void setPosition( int nX, int nY );
    void setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect = { 0, 0, 0, 0 } );
    void setRelativePosition( double dX, double dY );
    void changeSize() override;

    int getHeight() const;
    int getWidth() const;
    int getPageHeight() const;
    int getPageWidth() const;

    void render() override;
    void render( SDL_Rect* SourceRect, SDL_Rect* DestRect = nullptr, double dAngle = 0.0, SDL_Point* Center = nullptr,
                 SDL_RendererFlip Flip = SDL_FLIP_NONE );

    operator bool ();

protected:
    pSharedSDLTexture m_pTexture    = nullptr;
    Renderer m_Renderer;

    int m_nSrcHeight                = 0;
    int m_nSrcWidth                 = 0;

    int m_nPageHeight               = 0;
    int m_nPageWidth                = 0;

    RelativeRect m_RelativeDestRect = { 0, 0, 0, 0 };
    SDL_Rect m_DestRect             = { 0, 0, 0, 0 };
    RelativeRect m_RelativeBaseRect = { 0, 0, 0, 0 };
    SDL_Rect m_BaseRect             = { 0, 0, 0, 0 };

    bool m_fBaseGiven               = false;
    bool m_fDestChanged             = false;

    void calcDestFromRelative( const RelativeRect &RelativeDestRect, SDL_Rect &DestRect );
    void recalcRelativePos();
};

#endif // TEXTURE_H
