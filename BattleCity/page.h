#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <algorithm>

#include "startpageinitdata.h"
#include "map.h"

class State;

class Page;
typedef std::shared_ptr < Page > pSharedPage;

class Page
{
public:
    Page() {}
    Page( InitContainers &&objContainers ):
          m_pDisplayedObjVc( std::move( objContainers.m_pDisplayedObjVc )),
          m_ButtonVc       ( std::move( objContainers.m_ButtonVc       )),
          m_pMap           ( std::move( objContainers.m_pMap           ))
    {}

    virtual ~Page() {}

    virtual void initButtons( State *pState ) = 0;

    virtual void resize()
    {
        if( !m_pDisplayedObjVc->empty() )
        {
            std::for_each( m_pDisplayedObjVc->begin(), m_pDisplayedObjVc->end(), [] ( pSharedDisplayedObj &pDisplayedObj )
            {
                pDisplayedObj->changeSize();
            });
        }
    }

    virtual void render()
    {
        if( !m_ButtonVc.empty() )
        {
            std::for_each( m_ButtonVc.begin(), m_ButtonVc.end(), [] ( pSharedButton &pButton )
            {
                if( pButton )
                {
                    pButton->render();
                    pButton->execute();
                }
            });
        }

        if( !m_pDisplayedObjVc->empty() )
        {
            std::for_each( m_pDisplayedObjVc->begin(), m_pDisplayedObjVc->end(), [] ( pSharedDisplayedObj &pDisplayedObj )
            {
                if( pDisplayedObj )
                {
                    pDisplayedObj->render();
                }
            });
        }
    }

protected:
    std::shared_ptr <std::vector <pSharedDisplayedObj>>  m_pDisplayedObjVc;
    std::vector <pSharedButton>                          m_ButtonVc;
    std::vector <pSharedDynObj>                          m_DynObjVc;
    pSharedMap m_pMap;
};

#endif // PAGE_H
