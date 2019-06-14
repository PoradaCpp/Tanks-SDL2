#include <iostream>


#include "surface.h"



Surface::Surface(): m_pSurface( nullptr ), m_nHeight(0), m_nWidth(0) {}

Surface::Surface( TextSurfaceInitData InitData ): m_pSurface( nullptr ), m_nHeight(0), m_nWidth(0)
{
    if( InitData.m_Font )
    {
        createSurfaceFromText( InitData.m_Font, InitData.m_sText, InitData.m_TextColor );
    }
    else
    {
        std::cerr << "Surface::Surface. Given Font is empty!" << '\n';
        std::exit(-1);
    }
}

Surface::Surface( std::string sImgPath ): m_pSurface( nullptr ), m_nHeight(0), m_nWidth(0)
{
    createSurfaceFromImg( sImgPath );
}

Surface::Surface( const Surface &surface ): m_pSurface( surface.m_pSurface ), m_nHeight( surface.m_nHeight ),
    m_nWidth( surface.m_nWidth ) {}

Surface::~Surface()
{
    free();
}

void Surface::createSurfaceFromText( Font font, std::string &sText, SDL_Color TextColor )
{
    free();
    pSharedSDLSurface temp_pSurface( TTF_RenderText_Solid( font, sText.c_str(), TextColor ),
                                     [] ( SDL_Surface *s ) { SDL_FreeSurface( s ); });
    if( temp_pSurface )
    {
        m_pSurface = temp_pSurface;
        m_nHeight = m_pSurface->h;
        m_nWidth = m_pSurface->w;
    }
    else
    {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
        std::exit(-1);
    }
}

void Surface::createSurfaceFromImg(std::string &sImgPath )
{
    free();
    pSharedSDLSurface temp_pSurface( IMG_Load( sImgPath.c_str() ), [] ( SDL_Surface *s ) { SDL_FreeSurface( s ); });

    if( temp_pSurface )
    {
        m_pSurface = temp_pSurface;
        m_nHeight = m_pSurface->h;
        m_nWidth = m_pSurface->w;
    }
    else
    {
        std::cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << '\n';
        std::exit(-1);
    }
}

void Surface::free()
{
    if( m_pSurface )
    {
        m_pSurface.reset();
        m_nHeight = 0;
        m_nWidth = 0;
    }
}

int Surface::getHeight()
{
    return m_nHeight;
}

int Surface::getWidth()
{
    return m_nWidth;
}

Surface::operator SDL_Surface* ()
{
    return m_pSurface.get();
}
