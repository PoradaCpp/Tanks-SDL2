#ifndef BUTTONBASE_H
#define BUTTONBASE_H

#include <string>
#include <array>
#include <functional>

#include "text3d.h"
#include "displayedobject.h"

enum class ButtonState
{
    DEFAULT  = 0,
    IN_FOCUS = 1,
    PRESSED  = 2,
    RELEASED = 3
};

class ButtonBase: public DisplayedObject
{
public:
    typedef std::function <void()> Action;

    ButtonBase(): DisplayedObject() {}
    virtual ~ButtonBase() override {}

    virtual void setAction( Action action ) = 0;
    virtual void execute() = 0;
    virtual void lock() = 0;
    virtual void unlock() = 0;

protected:
    Action m_Action;
};

typedef std::shared_ptr <ButtonBase> pSharedButton;

#endif // BUTTONBASE_H
