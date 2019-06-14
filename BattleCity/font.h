#ifndef FONT_H
#define FONT_H


#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>

class Surface;

typedef std::shared_ptr < TTF_Font > pSharedSDLFont;

class Font
{
public:
    friend class Surface;

    Font();
    Font( std::string sFontPath, int nTextSize );
    Font( const Font &font );
    ~Font();

    Font& operator = ( const Font &font );

private:
    pSharedSDLFont m_pFont;

    operator bool();
    operator TTF_Font* ();
};

#endif // FONT_H
