#ifndef STARTPAGE_H
#define STARTPAGE_H

#include "page.h"

enum class CurrentState;

class StartPage: public Page
{
public:
    StartPage( InitContainers &&objContainers, State *pState );

    ~StartPage() override;

    void initButtons( State *pState ) override;
    void render() override;

private:
    enum class Buttons
    {
        SET_CREATE_MAP_STATE = 0,
        SET_GAME_STATE       = 1,
        SET_PLAYERS_QUANTITY = 2,
        BUTTONS_QUANTITY     = 3
    };

    static const SDL_Scancode KEY_ENTER_KP = SDL_SCANCODE_KP_ENTER;
    static const SDL_Scancode KEY_ENTER    = SDL_SCANCODE_RETURN;
    static const SDL_Scancode KEY_SPACE    = SDL_SCANCODE_SPACE;

    State *m_pState;
    CurrentState m_CurrentState;
};

#endif // STARTPAGE_H
