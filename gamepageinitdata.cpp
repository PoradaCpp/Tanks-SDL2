
#include <algorithm>

#include "gamepageinitdata.h"


GamePageInitData::GamePageInitData( Renderer renderer ):

    InitData( renderer ),

    m_ImgInitVc
    ({
         { "media/images/wood_panel21.png"                                                           },
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 9.9,  4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 9.9,  4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 14.8, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 14.8, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 19.7, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 19.7, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 24.6, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 24.6, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 29.5, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 29.5, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 34.4, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 34.4, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 39.3, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 39.3, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 44.2, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 44.2, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 49.1, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 49.1, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 87.2, 54.5, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",   { 92.5, 54.5, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_green.png", { 87.2, 64.7, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_green.png", { 92.5, 64.7, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_green.png", { 87.2, 69.6, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_green.png", { 92.5, 69.6, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_green.png", { 87.2, 74.5, 4.5, 4.1 }, { 1.2, 2.1, 99, 95.4 }}
     }),
    m_Tex3DInitVc
     ({
         { m_BaltExtB, "STAGE 1",  m_Red_Hazel, m_Black, 90,  200, 1, 2, { 86.2, 0.6,  12.5, 4.4 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "ENEMIES:", m_Red_Hazel, m_Black, 120, 255, 1, 2, { 87.1, 5.5,  9.5,  3.5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "LIVES:",   m_Red_Hazel, m_Black, 125, 255, 1, 2, { 88.7, 60.5, 5.5,  3.5 }, { 1.2, 2.1, 99, 95.4 }},
      }),

    m_ButtonNamedInitVc
    ({
        {{ "media/images/buttons/wood_button.png", { 89,   86.5, 6, 5 }, { 1.3, 2.3, 99, 95.4 }}, { m_BaltExtB, "  Pause  ",  m_Brown,
           m_Black, 150, 255, 1, 2 }},
        {{ "media/images/buttons/wood_button.png", { 88.5, 93,   7, 5 }, { 1.3, 2.3, 99, 95.4 }}, { m_BaltExtB, "Create Map", m_Brown,
           m_Black, 150, 255, 1, 2 }},
        {{ "media/images/buttons/wood_button.png", { 88,   80,   8, 5 }, { 1.3, 2.3, 99, 95.4 }}, { m_BaltExtB, "Start Page", m_Brown,
           m_Black, 150, 255, 1, 2 }}
     }),

    m_MapInitData({ "media/images/tiles_image.png" }, "media/maps/maps.data" )

{
    init();
}

GamePageInitData::GamePageInitData( std::vector < ImgTextureInitData > &&ImgTextureInitVc,
                                              std::vector <Text3DInitData> &&Text3DInitVc,
                                              std::vector <ButtonInitData> &&ButtonInitVc,
                                              std::vector <ButtonInitData> &&ButtonNamedInitVc,
                                              MapInitData mapInitData,
                                              Renderer renderer ):

    InitData           ( renderer                     ),
    m_ImgInitVc        ( std::move( ImgTextureInitVc  )),
    m_Tex3DInitVc      ( std::move( Text3DInitVc      )),
    m_ButtonInitVc     ( std::move( ButtonInitVc      )),
    m_ButtonNamedInitVc( std::move( ButtonNamedInitVc )),
    m_MapInitData      ( mapInitData                  )
{
    init();
}


GamePageInitData::~GamePageInitData() {}


void GamePageInitData::init()
{
    m_InitContainers.m_DisplayedObjVc.reserve( m_ImgInitVc.size() + m_Tex3DInitVc.size() + m_ButtonInitVc.size()
                                               + m_ButtonNamedInitVc.size());

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

    std::for_each( m_ButtonNamedInitVc.begin(), m_ButtonNamedInitVc.end(), [ this ] ( ButtonInitData initData )
    {
        auto ptr = std::make_shared < ButtonNamed > ( initData, m_Renderer );
        m_InitContainers.m_DisplayedObjVc.push_back( ptr );
        m_InitContainers.m_ButtonVc.push_back( ptr );
    });

    m_InitContainers.m_pMap = std::make_shared < Map > ( m_MapInitData, m_Renderer );
}
