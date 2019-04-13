
#include <algorithm>

#include "createmappageinitdata.h"


CreateMapPageInitData::CreateMapPageInitData( Renderer renderer ):

    InitData( renderer ),

    m_ImgInitVc
    ({
         { "media/images/WoodPanel.png" }
     }),

    m_ButtonInitVc
    ({
         {{ "media/images/wood_button.png",     { 89,   92, 6,   5    }}, { m_BaltExtB, "  Start  ",  m_Brown, m_Black, 150, 255, 1, 2 }},
         {{ "media/images/wood_button.png",     { 88,   85, 8,   5    }}, { m_BaltExtB, "Main Page",  m_Brown, m_Black, 150, 255, 1, 2 }},
         {{ "media/images/BrickWallButton.png", { 85.5, 8,  6.5, 11.5 }}, { m_BaltExtB, " ",          m_Brown, m_Black, 150, 255, 1, 2 }}
     })

{
    init();
}

CreateMapPageInitData::CreateMapPageInitData( std::vector < ImgTextureInitData > &&ImgTextureInitVc,
                                              std::vector <Text3DInitData> &&Text3DInitVc,
                                              std::vector <ButtonInitData> &&ButtonInitVc,
                                              Renderer renderer ):

    InitData      ( renderer                    ),
    m_ImgInitVc   ( std::move( ImgTextureInitVc )),
    m_Tex3DInitVc ( std::move( Text3DInitVc     )),
    m_ButtonInitVc( std::move( ButtonInitVc     ))
{
    init();
}


CreateMapPageInitData::~CreateMapPageInitData() {}


void CreateMapPageInitData::init()
{
    m_InitContainers.m_DisplayedObjVc.reserve( m_ImgInitVc.size() + m_Tex3DInitVc.size() + m_ButtonInitVc.size() );

    std::for_each( m_ImgInitVc.begin(), m_ImgInitVc.end(), [ this ] ( ImgTextureInitData initData )
    {
        m_InitContainers.m_DisplayedObjVc.push_back( std::make_shared < Texture > ( initData, m_Renderer ) );
    });

    std::for_each( m_Tex3DInitVc.begin(), m_Tex3DInitVc.end(), [ this ] ( Text3DInitData initData )
    {
        m_InitContainers.m_DisplayedObjVc.push_back( std::make_shared < Text3D > ( initData, m_Renderer ) );
    });

    std::for_each( m_ButtonInitVc.begin(), m_ButtonInitVc.end(), [ this ] ( ButtonInitData initData )
    {
        auto ptr = std::make_shared < Button > ( initData, m_Renderer );
        m_InitContainers.m_DisplayedObjVc.push_back( ptr );
        m_InitContainers.m_ButtonVc.push_back( ptr );
    });
}
