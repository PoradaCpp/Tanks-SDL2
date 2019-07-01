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
          m_DisplayedObjVc( std::move( objContainers.m_DisplayedObjVc )),
          m_ButtonVc      ( std::move( objContainers.m_ButtonVc       )),
          m_pMap          ( std::move( objContainers.m_pMap           )),
          m_AudioChunk    ( std::move( objContainers.m_PageAudioChunk     ))
    {}

    virtual ~Page() {}

    virtual void initButtons( State *pState ) = 0;

    virtual void resize()
    {
        if( !m_DisplayedObjVc.empty() )
        {
            std::for_each( m_DisplayedObjVc.begin(), m_DisplayedObjVc.end(), [] ( pSharedDisplayedObj &pDisplayedObj )
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

        if( !m_DisplayedObjVc.empty() )
        {
            std::for_each( m_DisplayedObjVc.begin(), m_DisplayedObjVc.end(), [] ( pSharedDisplayedObj &pDisplayedObj )
            {
                if( pDisplayedObj )
                {
                    pDisplayedObj->render();
                }
            });
        }
    }

protected:
    std::vector <pSharedDisplayedObj>  m_DisplayedObjVc;
    std::vector <pSharedButton>        m_ButtonVc;
    std::vector <pSharedDynObj>        m_DynObjVc;
    pSharedMap m_pMap;
    AudioChunk m_AudioChunk;
};

#endif // PAGE_H
