#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <SDL.h>
#include <memory>
#include <vector>

#include "page.h"
#include "renderer.h"

typedef std::shared_ptr < SDL_Window > pSharedSDLWindow;

class MainWindow
{
public:
    friend class Renderer;

    MainWindow( std::string sName = "Main Window", std::string sIconPath = "" );
    MainWindow( const MainWindow &window );
    ~MainWindow();

    MainWindow& operator = ( const MainWindow &window );

    operator bool ();

    void init( std::string sName, std::string sIconPath );
    void free();
    void windowClear();
    void renderContents( State *state );
    void windowUpdate();
    void windowProcessing( State *state );
    int getHeight();
    int getWidth();
    Renderer getRenderer();
    void attachPage( pSharedPage pPage );

private:
    static const uint32_t DEFAULT_SCREEN_WIDTH = 1060;
    static const uint32_t DEFAULT_SCREEN_HEIGHT = 600;
    pSharedSDLWindow m_pWindow;
    Renderer m_Renderer;
    std::vector < pSharedPage > m_PageVector;

    int m_nWidth;
    int m_nHeight;

    operator SDL_Window* ();
};

#endif // MAINWINDOW_H
