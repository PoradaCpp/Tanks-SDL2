#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <algorithm>

#include "mainwindow.h"
#include "state.h"
#include "surface.h"

MainWindow::MainWindow(): m_pWindow( nullptr ), m_nWidth( 0 ), m_nHeight( 0 ) {}

MainWindow::MainWindow( std::string sName, std::string sIconPath ): m_pWindow( nullptr ), m_nWidth( 0 ), m_nHeight( 0 )
{
    init( sName, sIconPath );
}

MainWindow::MainWindow( const MainWindow &window ):
    m_pWindow( window.m_pWindow ),
    m_Renderer( window.m_Renderer ),
    m_PageVector( window.m_PageVector ),
    m_nWidth( window.m_nWidth ),
    m_nHeight( window.m_nHeight) {}

MainWindow::~MainWindow()
{
    free();
}

MainWindow& MainWindow::operator = ( const MainWindow &window )
{
    m_pWindow    = window.m_pWindow;
    m_Renderer   = window.m_Renderer;
    m_PageVector = window.m_PageVector;
    m_nWidth     = window.m_nWidth;
    m_nHeight    = window.m_nHeight;

    return *this;
}

MainWindow::operator bool ()
{
    return m_pWindow.operator bool();
}

void MainWindow::init( std::string sName, std::string sIconPath )
{
    free();

    pSharedSDLWindow temp_pWindow( SDL_CreateWindow( sName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                                  DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE ),
                                [] ( SDL_Window *w ) { SDL_DestroyWindow( w ); } );

    if( temp_pWindow )
    {
        m_nHeight = DEFAULT_SCREEN_HEIGHT;
        m_nWidth = DEFAULT_SCREEN_WIDTH;
        m_pWindow = temp_pWindow;

        Renderer tempRenderer( *this );
        m_Renderer = tempRenderer;
        if( !sIconPath.empty() )
        {
            Surface WindowIcon( sIconPath );

            SDL_SetWindowIcon( m_pWindow.get(), WindowIcon );
        }
    }
    else
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
        std::exit(-1);
    }
}

void MainWindow::free()
{
    if( m_pWindow )
    {
        if( m_Renderer )
        {
            m_Renderer.free();
        }

        m_pWindow.reset();
        m_nWidth = 0;
        m_nHeight = 0;
        m_pWindow = nullptr;
    }
}

void MainWindow::windowClear()
{
    SDL_SetRenderDrawColor( m_Renderer, 0x10, 0x10, 0x10, 0xFF );
    SDL_RenderClear( m_Renderer );
}

void MainWindow::renderContents( State *state )
{
    int nHeight(0), nWidth(0);

    SDL_GetWindowSize( m_pWindow.get(), &nHeight, &nWidth );

    if( nHeight != m_nHeight || nWidth != m_nWidth )
    {
        m_nHeight = nHeight;
        m_nWidth = nWidth;

        m_PageVector.at( static_cast <size_t> ( state->getState() ) )->resize();
    }
    m_PageVector.at( static_cast <size_t> ( state->getState() ) )->render();
}

void MainWindow::windowUpdate()
{
    SDL_RenderPresent( m_Renderer );
}

void MainWindow::windowProcessing( State *state )
{
    windowClear();
    renderContents( state );
    windowUpdate();
}

int MainWindow::getHeight()
{
    return m_nHeight;
}

int MainWindow::getWidth()
{
    return m_nWidth;
}

Renderer MainWindow::getRenderer() const
{
    return m_Renderer;
}

void MainWindow::attachPage( pSharedPage pPage )
{
    m_PageVector.push_back( pPage );
}

MainWindow::operator SDL_Window* ()
{
    return m_pWindow.get();
}
