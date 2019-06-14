#ifndef BUILDMAP_H
#define BUILDMAP_H

#include "dynamicobject.h"
#include "texture.h"
#include "map.h"


class BuildMapMoveAlgo: public MoveAlgorithm
{
public:
    BuildMapMoveAlgo();
    ~BuildMapMoveAlgo() override;

    void move( SDL_Rect &CurPos ) override;
};

class BuildMap: public DynamicObject
{
public:
    BuildMap( Texture texture, Renderer renderer );
    ~BuildMap() override;

    void setDestRect( SDL_Rect DestRect );
    void setSource( size_t nPos );
    size_t getSourceRectPos();

    void changePosition() override;
    void changeSize() override;
    void render() override;

private:
    static const Uint8 TRANSPARENT = 80;
    static const Uint8 OPAQUE      = 255;

    Texture m_Texture;
    Renderer m_Renderer;
    SDL_Rect m_CurPos;
    SDL_Rect m_DestRect;
    std::array < SDL_Rect, Map::VECTOR_TILE_SIZE > m_SourceRectArr;
    size_t m_nCurrentSourceRect;

    void setRenderDimensions();
};

typedef std::shared_ptr <BuildMap>  pSharedBuildMap;

#endif // BUILDMAP_H
