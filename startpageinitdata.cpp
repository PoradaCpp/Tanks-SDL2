
#include <algorithm>

#include "startpageinitdata.h"


StartPageInitData::StartPageInitData( Renderer renderer ):

    InitData( renderer ),

    m_ImgInitVc
    ({
         { "media/images/wood_panel42.png"                    },
         { "media/images/title_text2.png", { 22, 15, 56, 32  }}
     }),

    m_Tex3DInitVc
    ({
        { m_Arial,    "MY SCORE:",          m_Red_Brown, m_Black, 185, 150, 2, 2, { 5,    3,    15,   4                             }},
        { m_Arial,    "HI SCORE: 20000",    m_Red_Brown, m_Black, 185, 150, 2, 2, { 40,   3,    23,   4                             }},
        { m_Calibrib, "1 PLAYER",           m_Hazel,     m_Black, 150, 120, 2, 2, { 45,   55,   16.5, 5   }, { 27, 57,   36.8, 20   }},
        { m_Calibrib, "2 PLAYERS",          m_Hazel,     m_Black, 150, 120, 2, 2, { 45,   65,   18.8, 5   }, { 27, 57,   36.8, 20   }},
        { m_Calibrib, "Start",              m_Red_Hazel, m_Black, 110, 150, 2, 2, { 10,   78.7, 4.6,  3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "Pause",              m_Red_Hazel, m_Black, 110, 150, 2, 2, { 10,   83,   5.0,  3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "Move",               m_Red_Hazel, m_Black, 110, 150, 2, 2, { 10,   89,   4.6,  3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "Shot",               m_Red_Hazel, m_Black, 110, 150, 2, 2, { 10,   93.3, 4.0,  3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "Space        Enter", m_Hazel,     m_Black, 200, 150, 2, 2, { 18,   78.7, 12.8, 3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "P",                  m_Hazel,     m_Black, 200, 150, 2, 2, { 18,   83,   1.4,  3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "\'Up\', \'Down\', \'Left\', \'Right\'        \'W\', \'A\', \'S\', \'D\'",
                                            m_Hazel,     m_Black, 200, 150, 2, 2, { 18,   89,   36,   3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "J        Space        Num0",
                                            m_Hazel,     m_Black, 200, 150, 2, 2, { 18,   93.3, 16.5, 3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "OR",                 m_Red_Brown, m_Black, 200, 150, 2, 2, { 23.5, 78.7, 2.3,  3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "OR",                 m_Red_Brown, m_Black, 200, 150, 2, 2, { 39.5, 89,   2.3,  3.2 }, { 10, 78.7, 44,   17.8 }},
        { m_Calibrib, "OR              OR", m_Red_Brown, m_Black, 200, 150, 2, 2, { 19.2, 93.3, 10,   3.2 }, { 10, 78.7, 44,   17.8 }}
     }),

    m_ButtonInitVc
    ({
         {{ "media/images/buttons/wood_button.png", { 88, 85, 8, 5 }}, { m_BaltExtB, "Create Map", m_Brown, m_Black, 150, 255, 1, 2 }},
         {{ "media/images/buttons/wood_button.png", { 89, 92, 6, 5 }}, { m_BaltExtB, "  Start  ",  m_Brown, m_Black, 150, 255, 1, 2 }}
     }),

    m_TankArrowInitData( "media/images/tanks/heavy_tank.png", { 27, 52, 15, 10  }, { 27, 62, 15, 10 }, { 27, 57, 36.8, 20 } )
{
    init();
}

StartPageInitData::StartPageInitData( std::vector < ImgTextureInitData > &&ImgTextureInitVc,
                                      std::vector <Text3DInitData> &&Text3DInitVc, std::vector <ButtonInitData> &&ButtonInitVc,
                                      TankArrowInitData &&tankArrowInitData , Renderer renderer ):

    InitData           ( renderer                     ),
    m_ImgInitVc        ( std::move( ImgTextureInitVc )),
    m_Tex3DInitVc      ( std::move( Text3DInitVc     )),
    m_ButtonInitVc     ( std::move( ButtonInitVc     )),
    m_TankArrowInitData( tankArrowInitData            )
{
    init();
}


StartPageInitData::~StartPageInitData() {}


void StartPageInitData::init()
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
        auto ptr = std::make_shared < ButtonNamed > ( initData, m_Renderer );
        m_InitContainers.m_DisplayedObjVc.push_back( ptr );
        m_InitContainers.m_ButtonVc.push_back( ptr );
    });

    auto ptr = std::make_shared < TankArrow > ( m_TankArrowInitData, m_Renderer );
    m_InitContainers.m_DisplayedObjVc.push_back( ptr );
    m_InitContainers.m_DynObjVc.push_back( ptr );
}
