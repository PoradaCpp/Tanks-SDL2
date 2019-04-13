#ifndef DISPLAYEDOBJECT_H
#define DISPLAYEDOBJECT_H

#include <memory>

class DisplayedObject
{
public:
    DisplayedObject() {}
    virtual ~DisplayedObject() {}

    virtual void changeSize() = 0;
    virtual void render() = 0;
};

typedef std::shared_ptr < DisplayedObject > pSharedDisplayedObj;

#endif // DISPLAYEDOBECT_H
