#ifndef STARTPAGE_H
#define STARTPAGE_H

#include "page.h"

class StartPage: public Page
{
public:
    StartPage( InitContainers &&objContainers );

    ~StartPage() override;

    void changeState( State *state ) override;

private:
    enum class Button
    {
        CREATE_MAP = 0,
        START      = 1
    };
};

#endif // STARTPAGE_H
