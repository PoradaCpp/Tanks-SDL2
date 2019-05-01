#ifndef CREATEMAPPAGE_H
#define CREATEMAPPAGE_H


#include "page.h"
#include "buildmap.h"
#include "map.h"

class MapSquareButton
{
public:
    MapSquareButton();
    ~MapSquareButton();

    void attachMap( pSharedMap pMap );
    void getCurrentTileState();
    void action( pSharedBuildMap &pMapUnit );

private:
    enum class MapSquareState
    {
        DEFAULT  = 0,
        IN_FOCUS = 1,
        PRESSED  = 2,
        RELEASED = 3
    };

    pSharedMap m_pMap;

    size_t m_nCurrentTile;
    MapSquareState m_CurrentTileState;
    SDL_Point m_CursorPos;
};

enum class CurrentState;

class CreateMapPage: public Page
{
public:    
    CreateMapPage( InitContainers &&objContainers,  State *pState, Renderer renderer );
    ~CreateMapPage() override;

    void initButtons( State *pState ) override;
    void resize() override;
    void render() override;

    void renderMap();
    void renderMapNet();

private:
    enum class Buttons
    {
        BUILD_MAP_BUTTONS_QUANTITY = 13,
        SET_GAME_STATE             = 14,
        SAVE_CURRENT_MAP           = 15,
        SET_START_PAGE_STATE       = 16,
        BUTTONS_QUANTITY           = 17
    };

    Renderer m_Renderer;
    pSharedBuildMap m_pMapUnit;
    SDL_Point m_CursorPos;
    CurrentState m_CurrentState;
    MapSquareButton m_MapSquareButton;
};

#endif // CREATEMAPPAGE_H
