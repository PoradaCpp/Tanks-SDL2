#ifndef CREATEMAPPAGE_H
#define CREATEMAPPAGE_H


#include "page.h"
#include "buildmap.h"

class CreateMapPage: public Page
{
public:
    CreateMapPage( InitContainers &&objContainers, Renderer renderer );

    ~CreateMapPage() override;

    void changeState( State *state ) override;
    void resize() override;
    void render() override;

private:
    enum class Button
    {
        START      = 0,
        MAIN_PAGE  = 1,
        BRICK_WALL = 2
    };

    Renderer m_Renderer;
    pSharedDynObj pMapUnit;
};

#endif // CREATEMAPPAGE_H
