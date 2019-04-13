#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <memory>


typedef std::shared_ptr < SDL_Renderer > pSharedSDLRenderer;

class Texture;
class MainWindow;

class Renderer
{
public:
    friend class MainWindow;
    friend class Texture;

    Renderer();
    Renderer( MainWindow &pWindow );
    Renderer( const Renderer &renderer );
    ~Renderer();

    Renderer& operator = ( const Renderer &renderer );

private:
    pSharedSDLRenderer m_pRenderer;

    void free();
    operator bool();
    operator SDL_Renderer* ();
};

#endif // RENDERER_H
