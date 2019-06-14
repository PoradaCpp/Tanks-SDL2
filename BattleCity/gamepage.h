#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include "page.h"
#include "map.h"
#include "gameengine.h"

enum class CurrentState;

class GamePage: public Page
{
public:
    GamePage( InitContainers &&objContainers, pSharedGameEngine &&pGameEngine, State *pState, Renderer renderer );
    ~GamePage() override;

    void initButtons( State *pState ) override;
    void resize() override;
    void render() override;

    void renderMapBorder();

private:
    enum class Buttons
    {
        SET_PAUSE            = 0,
        SET_CREATE_MAP_STATE = 1,
        SET_START_PAGE_STATE = 2,
        BUTTONS_QUANTITY     = 3
    };

    pSharedGameEngine m_pGameEngine;
    Renderer m_Renderer;
    CurrentState m_CurrentState;
    SDL_Rect m_MapBordersRect;
};

#endif // GAMEPAGE_H
