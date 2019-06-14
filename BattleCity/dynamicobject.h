#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include <memory>

#include "movealgorithm.h"
#include "displayedobject.h"
#include "map.h"

class DynamicObject: public DisplayedObject
{
public:
    DynamicObject(): DisplayedObject() {}
    ~DynamicObject() override {}

    void virtual changePosition() = 0;

    void virtual attachMoveAlgo( pSharedMoveAlgo pMoveAlgorithm )
    {
        m_pMoveAlgorithm = pMoveAlgorithm;
    }

protected:
    pSharedMoveAlgo m_pMoveAlgorithm;
};

typedef std::shared_ptr < DynamicObject > pSharedDynObj;

#endif // DYNAMICOBJECT_H
