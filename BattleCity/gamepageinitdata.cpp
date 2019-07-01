
#include <algorithm>

#include "gamepageinitdata.h"


GamePageInitData::GamePageInitData( Renderer renderer ):

    PageInitData( renderer ),

    m_ImgInitVc1
    ({
         { "media/images/wood_panel21.png" }
     }),

    m_ImgInitVc2
    ({
         { "media/images/tanks/tank_icon_green.png",    { 86.5, 50.0, 3.4, 3.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_yellow.png",   { 86.5, 65.2, 3.4, 3.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_dark_red.png", { 94.5, 39.8, 3.4, 3.1 }, { 1.2, 2.1, 99, 95.4 }},
         { "media/images/tanks/tank_icon_red.png",      { 86.5, 10.4, 3.4, 3.1 }, { 1.2, 2.1, 99, 95.4 }}
     }),

    m_Tex3DInitVc1
     ({
         { m_BaltExtB, "STAGE 1",           m_Red_Hazel, m_Black, 90,  200, 1, 2, { 85.7, 0.6,  13,    4.5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "ENEMIES:",          m_Red_Hazel, m_Black, 120, 255, 1, 2, { 87.1, 5.5,  9.5,   3.5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "PLAYER ONE LIVES:", m_Red_Hazel, m_Black, 125, 255, 1, 2, { 84.6, 45.3, 14.2,  3.5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "PLAYER TWO LIVES:", m_Red_Hazel, m_Black, 125, 255, 1, 2, { 84.6, 60.5, 14.2,  3.5 }, { 1.2, 2.1, 99, 95.4 }}
      }),

    m_Tex3DInitVc2
     ({
         { m_BaltExtB, "HI SCORE:",       m_Red_Hazel, m_Black, 90,  200, 1, 2, { 40.4, 17,   12.2, 5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "STAGE",           m_Red_Hazel, m_Black, 120, 255, 1, 2, { 44.1, 25,   10.0, 6 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "PLAYER 1",        m_Red_Hazel, m_Black, 125, 255, 1, 2, { 19.5, 25,   11.9, 5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "PLAYER 2",        m_Red_Hazel, m_Black, 125, 255, 1, 2, { 61.9, 25,   11.9, 5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "PTS",             m_Red_Hazel, m_Black, 125, 255, 1, 2, { 40.7, 35.5, 4.4,  5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "PTS",             m_Red_Hazel, m_Black, 125, 255, 1, 2, { 40.7, 46.4, 4.4,  5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "PTS",             m_Red_Hazel, m_Black, 125, 255, 1, 2, { 40.7, 57.3, 4.4,  5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "PTS",             m_Red_Hazel, m_Black, 125, 255, 1, 2, { 40.7, 68.2, 4.4,  5 }, { 1.2, 2.1, 99, 95.4 }},
         { m_BaltExtB, "TOTAL:  20",      m_Red_Hazel, m_Black, 125, 255, 1, 2, { 36.4, 77,   12.4, 5 }, { 1.2, 2.1, 99, 95.4 }}
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

    m_MapInitData({ "media/images/tiles_image.png" }, "media/maps/maps.data" ),

    m_AnimationInitDataVc
    ({
        {{ "media/images/tanks/complite_tanks_animation.png", { 0, 0, 0, 0 }, { 1.3, 2.3, 81.2, 93.6 } }, 4, 26 },
        {{ "media/images/tanks/tank_explosion_animation.png", { 0, 0, 0, 0 }, { 1.3, 2.3, 81.2, 93.6 } }, 4, 16 },
        {{ "media/images/tanks/bomb_explosion_animation.png", { 0, 0, 0, 0 }, { 1.3, 2.3, 81.2, 93.6 } }, 4, 12 },
     }),

     m_sPageAudioChunkPath( "media/audio/level_start.wav" ),

     m_ObjectsManagementInitData
     ({
         { {{ "media/images/heart_anim.png", { 39, 86, 5.7, 10 }, { 1.3, 2.3, 81.2, 93.6 } }, 4, 8, 9 },
              "media/audio/players_heart_breaking.wav" },
         { m_BaltExtB, "GAME OVER", m_White, m_Grey, 120, 150, -5, -5, { 16.3, 33, 50, 30 } },
         {    "media/images/bonuses.png", { 39, 70, 5.7, 10 } },
              "media/audio/bonus_add.wav",
              "media/audio/bonus_taken.wav"
      }),

     m_BackgroundTextureInitData( "media/images/score_counting.png", { 15, 15, 70, 70 } )

{
    init();
}

GamePageInitData::GamePageInitData( std::vector <ImgTextureInitData> &&ImgTextureInitVc1,
                                    std::vector <ImgTextureInitData> &&ImgTextureInitVc2,
                                    std::vector <Text3DInitData> &&Text3DInitVc1,
                                    std::vector<Text3DInitData> &&Text3DInitVc2,
                                    std::vector <ButtonInitData> &&ButtonInitVc,
                                    std::vector <ButtonInitData> &&ButtonNamedInitVc,
                                    MapInitData mapInitData,
                                    std::vector<AnimationInitData> AnimationInitDataVc,
                                    std::string sPageAudioChunkPath,
                                    ObjectsManagementInitData objectsManagementInitData,
                                    ImgTextureInitData BackgroundTextureInitData,
                                    Renderer renderer ):

    PageInitData                ( renderer                      ),
    m_ImgInitVc1                ( std::move( ImgTextureInitVc1 )),
    m_ImgInitVc2                ( std::move( ImgTextureInitVc2 )),
    m_Tex3DInitVc1              ( std::move( Text3DInitVc1     )),
    m_Tex3DInitVc2              ( std::move( Text3DInitVc2     )),
    m_ButtonInitVc              ( std::move( ButtonInitVc      )),
    m_ButtonNamedInitVc         ( std::move( ButtonNamedInitVc )),
    m_MapInitData               ( mapInitData                   ),
    m_AnimationInitDataVc       ( AnimationInitDataVc           ),
    m_sPageAudioChunkPath       ( sPageAudioChunkPath           ),
    m_ObjectsManagementInitData ( objectsManagementInitData     ),
    m_BackgroundTextureInitData ( BackgroundTextureInitData     )

{
    init();
}


GamePageInitData::~GamePageInitData() {}


void GamePageInitData::init()
{
    m_InitContainers.m_DisplayedObjVc.reserve( m_ImgInitVc1.size() + m_Tex3DInitVc1.size() + m_ButtonInitVc.size()
                                               + m_ButtonNamedInitVc.size());

    std::for_each( m_ImgInitVc1.begin(), m_ImgInitVc1.end(), [this] ( ImgTextureInitData initData )
    {
        m_InitContainers.m_DisplayedObjVc.push_back( std::make_shared <Texture> ( initData, m_Renderer ) );
    });

    std::for_each( m_Tex3DInitVc1.begin(), m_Tex3DInitVc1.end(), [this] ( Text3DInitData initData )
    {
        m_InitContainers.m_DisplayedObjVc.push_back( std::make_shared <Text3D> ( initData, m_Renderer ) );
    });

    std::for_each( m_ButtonInitVc.begin(), m_ButtonInitVc.end(), [this] ( ButtonInitData initData )
    {
        auto ptr = std::make_shared <Button> ( initData, m_Renderer );
        m_InitContainers.m_DisplayedObjVc.push_back( ptr );
        m_InitContainers.m_ButtonVc.push_back( ptr );
    });

    std::for_each( m_ButtonNamedInitVc.begin(), m_ButtonNamedInitVc.end(), [this] ( ButtonInitData initData )
    {
        auto ptr = std::make_shared <ButtonNamed> ( initData, m_Renderer );
        m_InitContainers.m_DisplayedObjVc.push_back( ptr );
        m_InitContainers.m_ButtonVc.push_back( ptr );
    });

    m_InitContainers.m_PageAudioChunk.createAudio( m_sPageAudioChunkPath );
    m_InitContainers.m_pMap = std::make_shared <Map> ( m_MapInitData, m_Renderer );
    m_pGameEngine = std::make_shared <GameEngine> ( m_AnimationInitDataVc, m_ImgInitVc2, m_ObjectsManagementInitData,
                                                    m_BackgroundTextureInitData, m_Tex3DInitVc2, m_Renderer, m_InitContainers.m_pMap );
}

GamePageInitData::operator pSharedGameEngine && ()
{
    return std::move( m_pGameEngine );
}
