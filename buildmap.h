#ifndef BUILDMAP_H
#define BUILDMAP_H


#include "dynamicobject.h"
#include "texture.h"

class BuildMapMoveAlgo: public MoveAlgorithm
{
public:
    BuildMapMoveAlgo();
    ~BuildMapMoveAlgo();

    void move( SDL_Rect &CurPos ) override;
};



class BuildMap: public DynamicObject
{
public:
    BuildMap( ImgTextureInitData InitData, Renderer renderer );
    ~BuildMap() override;

    void changePosition() override;
    void changeSize() override;
    void render() override;

private:
    Texture m_Texture;
    SDL_Rect m_CurPos;
};

#endif // BUILDMAP_H
