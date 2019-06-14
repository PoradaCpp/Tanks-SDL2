#include <iostream>

#include "font.h"

Font::Font(): m_pFont( nullptr ) {}

Font::Font( std::string sFontPath , int nTextSize ): m_pFont( nullptr )
{
    pSharedSDLFont temp_pFont( TTF_OpenFont( sFontPath.c_str(), nTextSize ),
                                     [] ( TTF_Font *f ) { TTF_CloseFont( f ); });
    if( temp_pFont )
    {
        m_pFont = temp_pFont;
    }
    else
    {
        std::cerr << "Unable to load font \"" << sFontPath << "\"! SDL_ttf Error: " << TTF_GetError() << '\n';
        std::exit(-1);
    }
}

Font::Font( const Font &font ): m_pFont( font.m_pFont ) {}

Font::~Font() {}

Font& Font::operator = ( const Font &font )
{
    m_pFont = font.m_pFont;

    return *this;
}

Font::operator bool()
{
    return m_pFont.operator bool();
}

Font::operator TTF_Font* ()
{
    if( m_pFont )
    {
        return m_pFont.get();
    }
    else
    {
        return nullptr;
    }
}
