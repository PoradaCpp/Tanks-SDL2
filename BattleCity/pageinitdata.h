#ifndef PAGE_INIT_DATA_H
#define PAGE_INIT_DATA_H


#include <memory>
#include <vector>

#include "dynamicobject.h"
#include "buttonnamed.h"
#include "map.h"
#include "audiochunk.h"

struct InitContainers
{
    InitContainers() {}
    InitContainers( std::vector <pSharedDisplayedObj> DisplayedObjVc, std::vector < pSharedButton > ButtonVc, pSharedMap pMap,
                    AudioChunk audioChunk ):
        m_DisplayedObjVc( DisplayedObjVc ),
        m_ButtonVc      ( ButtonVc       ),
        m_pMap          ( pMap           ),
        m_PageAudioChunk( audioChunk     ) {}

    InitContainers( const InitContainers &rObj  ):
        m_DisplayedObjVc( rObj.m_DisplayedObjVc ),
        m_ButtonVc      ( rObj.m_ButtonVc       ),
        m_pMap          ( rObj.m_pMap           ),
        m_PageAudioChunk( rObj.m_PageAudioChunk     ) {}

    InitContainers( const InitContainers &&rObj ):
        m_DisplayedObjVc( std::move( rObj.m_DisplayedObjVc )),
        m_ButtonVc      ( std::move( rObj.m_ButtonVc )      ),
        m_pMap          ( std::move( rObj.m_pMap )          ),
        m_PageAudioChunk( std::move( rObj.m_PageAudioChunk )    ){}

    ~InitContainers() {}

    std::vector <pSharedDisplayedObj> m_DisplayedObjVc;
    std::vector <pSharedButton>       m_ButtonVc;
    pSharedMap m_pMap;
    AudioChunk m_PageAudioChunk;
};

class PageInitData
{
public:
    PageInitData( Renderer renderer ):

        m_Renderer( renderer ),

        m_Hazel     ({ 120,  95,  75, 255 }),
        m_Brown     ({  85,  60,  50, 255 }),
        m_Umber     ({  30,   9,   9, 255 }),
        m_Red_Hazel ({ 255, 180, 160, 255 }),
        m_Red_Brown ({ 140,  60,  50, 255 }),
        m_Red       ({ 255,   0,   0, 255 }),
        m_Black     ({   0,   0,   0, 255 }),
        m_Grey      ({  40,  40,  40, 255 }),
        m_White     ({ 255, 255, 255, 255 }),

        m_Arial    ( "media/fonts/arial.ttf",            70 ),
        m_Calibrib ( "media/fonts/calibrib.ttf",         70 ),
        m_BaltExtB ( "media/fonts/BalticaExtraBold.ttf", 70 ) {}

    virtual ~PageInitData() {}

    virtual operator InitContainers && ()
    {
        return std::move( m_InitContainers );
    }

    virtual void init() = 0;

protected:
    Renderer m_Renderer;

    SDL_Color m_Hazel;
    SDL_Color m_Brown;
    SDL_Color m_Umber;
    SDL_Color m_Red_Hazel;
    SDL_Color m_Red_Brown;
    SDL_Color m_Red;
    SDL_Color m_Black;
    SDL_Color m_Grey;
    SDL_Color m_White;

    Font m_Arial;
    Font m_Calibrib;
    Font m_BaltExtB;

    InitContainers m_InitContainers;
};

#endif // PAGE_INIT_DATA_H
