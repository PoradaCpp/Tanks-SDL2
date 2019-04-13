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
        m_dX     ( dX      > 100 ? 100 : ( dX      < 0 ? 0 : dX      )),
        m_dY     ( dY      > 100 ? 100 : ( dY      < 0 ? 0 : dY      )),
        m_dWidth ( dWidth  > 100 ? 100 : ( dWidth  < 0 ? 0 : dWidth  )),
        m_dHeight( dHeight > 100 ? 100 : ( dHeight < 0 ? 0 : dHeight )) {}

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

    TextTextureInitData( Font font, std::string sText, SDL_Color TextColor, RelativeRect DestRect = { 0, 0, 0, 0 },
                         RelativeRect BaseRect = { 0, 0, 0, 0 } ):
        m_TextSurfaceInitData( font, sText, TextColor ), m_DestRect( DestRect ), m_BaseRect( BaseRect ) {}
};

typedef std::shared_ptr < SDL_Texture > pSharedSDLTexture;
typedef std::shared_ptr < Texture > pSharedTexture;

class Texture: public DisplayedObject
{
public:
    Texture( ImgTextureInitData ImgInitData, Renderer renderer );
    Texture( TextTextureInitData TextInitData, Renderer renderer );
    ~Texture();

    void attachRenderer( Renderer renderer );
    void makeTextureFromSurface( Surface &surface );
    void setBlendMode( SDL_BlendMode BlendMode = SDL_BLENDMODE_BLEND );
    void setAlpha( Uint8 Alpha );
    void changeAlpha( Sint8 dAlpha );
    Uint8 getAlpha();
    void free();

    SDL_Rect getDestination() const;
    SDL_Point getPosition() const;
    void setDestination( SDL_Rect DestRect );
    void setPosition( int nX, int nY );
    void setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect = { 0, 0, 0, 0 } );
    void setRelativePosition( double dX, double dY );
    void changeSize() override;

    int getHeight();
    int getWidth();

    void render() override;
    void render( SDL_Rect* SourceRect, SDL_Rect* DestRect = nullptr, double dAngle = 0.0,
                        SDL_Point* Center = nullptr, SDL_RendererFlip Flip = SDL_FLIP_NONE );

private:
    pSharedSDLTexture m_pTexture;
    Renderer m_Renderer;

    int m_nSrcHeight;
    int m_nSrcWidth;

    int m_nPageHeight;
    int m_nPageWidth;

    RelativeRect m_RelativeDestRect;
    SDL_Rect m_DestRect;
    RelativeRect m_RelativeBaseRect;
    SDL_Rect m_BaseRect;

    bool m_fBaseGiven;

    pSharedSDLTexture makeTextureFromSurface( Surface &surface, Renderer &renderer ) const;
    void calcDestFromRelative( const RelativeRect &RelativeDestRect, SDL_Rect &DestRect );
};

#endif // TEXTURE_H
