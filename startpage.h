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
        BUTTONS_QUANTITY     = 2
    };

    CurrentState m_CurrentState;
};

#endif // STARTPAGE_H
