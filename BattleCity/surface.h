#ifndef SURFACE_H
#define SURFACE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>

#include "font.h"

struct TextSurfaceInitData
{
    Font m_Font;
    std::string m_sText;
    SDL_Color m_TextColor;

    TextSurfaceInitData( Font font, std::string sText, SDL_Color TextColor ):
        m_Font( font ), m_sText( sText ), m_TextColor( TextColor ) {}
};

typedef std::shared_ptr < SDL_Surface > pSharedSDLSurface;

class Texture;
class MainMindow;

class Surface
{
public:
    friend class Texture;
    friend class MainWindow;

    Surface();
    Surface( TextSurfaceInitData InitData );
    Surface( std::string sImgPath );
    Surface( const Surface &surface );
    ~Surface();

    void createSurfaceFromText( Font font, std::string &sText, SDL_Color TextColor );
    void createSurfaceFromImg( std::string &sImgPath );
    void free();
    int getHeight();
    int getWidth();

private:
    pSharedSDLSurface m_pSurface;

    int m_nHeight;
    int m_nWidth;

    operator SDL_Surface* ();
};

#endif // SURFACE_H
