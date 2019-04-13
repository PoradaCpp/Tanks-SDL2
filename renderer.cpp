
#include <iostream>

#include "renderer.h"
#include "mainwindow.h"


Renderer::Renderer(): m_pRenderer( nullptr ) {}

Renderer::Renderer( MainWindow &pWindow ): m_pRenderer( nullptr )
{
    pSharedSDLRenderer temp_pRenderer( SDL_CreateRenderer( pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ),
                 [] ( SDL_Renderer *r ) { SDL_DestroyRenderer( r ); } );

    if( !temp_pRenderer )
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << '\n';
        std::exit(-1);
    }
    else
    {
        m_pRenderer = temp_pRenderer;
    }
}

Renderer::Renderer( const Renderer &renderer ): m_pRenderer( renderer.m_pRenderer ) {}

Renderer::~Renderer() {}

Renderer& Renderer::operator = ( const Renderer &renderer )
{
    m_pRenderer = renderer.m_pRenderer;

    return *this;
}

void Renderer::free()
{
    m_pRenderer.reset();
}

Renderer::operator bool()
{
    return m_pRenderer.operator bool();
}

Renderer::operator SDL_Renderer* ()
{
    return m_pRenderer.get();
}
