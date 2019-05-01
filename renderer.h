#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <memory>


typedef std::shared_ptr < SDL_Renderer > pSharedSDLRenderer;

class MainWindow;

class Renderer
{
public:
    friend class MainWindow;
    friend class Texture;
    friend class CreateMapPage;
    friend class GamePage;
    friend class MapObject;
    friend class Map;
    friend class BuildMap;

    Renderer();
    Renderer( MainWindow &pWindow );
    ~Renderer();

    Renderer& operator = ( const Renderer &renderer );

private:
    pSharedSDLRenderer m_pRenderer;

    void free();
    operator bool();
    operator SDL_Renderer* ();
};

#endif // RENDERER_H
