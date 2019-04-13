#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <algorithm>

#include "startpageinitdata.h"

class State;

class Page;
typedef std::shared_ptr < Page > pSharedPage;

class Page
{
public:
    Page() {}
    Page( InitContainers &&objContainers ):
        m_DisplayedObjVc( std::move( objContainers.m_DisplayedObjVc )),
        m_ButtonVc      ( std::move( objContainers.m_ButtonVc       )),
        m_DynObjVc      ( std::move( objContainers.m_DynObjVc       ))
    {}

    virtual ~Page() {}

    virtual void changeState( State *state ) = 0;

    virtual void resize()
    {
        if( !m_DisplayedObjVc.empty() )
        {
            std::for_each( m_DisplayedObjVc.begin(), m_DisplayedObjVc.end(), [ this ] ( pSharedDisplayedObj &pDisplayedObj )
            {
                pDisplayedObj->changeSize();
            });
        }
    }

    virtual void render()
    {
        if( !m_DynObjVc.empty() )
        {
            std::for_each( m_DynObjVc.begin(), m_DynObjVc.end(), [] ( pSharedDynObj &DynObj )
            {
                DynObj->changePosition();
            });
        }

        if( !m_DisplayedObjVc.empty() )
        {
            std::for_each( m_DisplayedObjVc.begin(), m_DisplayedObjVc.end(), [] ( pSharedDisplayedObj &pDisplayedObj )
            {
                pDisplayedObj->render();
            });
        }
    }

protected:
    std::vector < pSharedDisplayedObj >  m_DisplayedObjVc;
    std::vector < pSharedButton >        m_ButtonVc;
    std::vector < pSharedDynObj >        m_DynObjVc;
};

#endif // PAGE_H
