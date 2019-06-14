
#include <algorithm>

#include "createmappageinitdata.h"


CreateMapPageInitData::CreateMapPageInitData( Renderer renderer ):

    PageInitData( renderer ),

    m_ImgInitVc
    ({
         { "media/images/wood_panel3.png" }
     }),

    m_ButtonInitVc
    ({
        {{ "media/images/buttons/BrickWallButton.png",            { 86.5, 4,  4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/GraniteWallButton.png",          { 92.5, 4,  4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/BrickHalfWallLeftButton.png",    { 86.5, 15, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/GraniteHalfWallLeftButton.png",  { 92.5, 15, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/BrickHalfWallUpButton.png",      { 86.5, 26, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/GraniteHalfWallUpButton.png",    { 92.5, 26, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/BrickHalfWallRightButton.png",   { 86.5, 37, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/GraniteHalfWallRightButton.png", { 92.5, 37, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/BrickHalfWallDownButton.png",    { 86.5, 48, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/GraniteHalfWallDownButton.png",  { 92.5, 48, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/ForestButton.png",               { 86.5, 59, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/WaterButton.png",                { 92.5, 59, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/IceButton.png",                  { 86.5, 70, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}},
        {{ "media/images/buttons/EmptyButton.png",                { 92.5, 70, 4, 7 }, { 1.3, 2.3, 99, 95.4 }}}
     }),

    m_ButtonNamedInitVc
    ({
        {{ "media/images/buttons/wood_button.png", { 89,   86.5, 6, 5 }, { 1.3, 2.3, 99, 95.4 }}, { m_BaltExtB, "  Start  ",  m_Brown,
           m_Black, 150, 255, 1, 2 }},
        {{ "media/images/buttons/wood_button.png", { 88.5, 93,   7, 5 }, { 1.3, 2.3, 99, 95.4 }}, { m_BaltExtB, "Save Map",   m_Brown,
           m_Black, 150, 255, 1, 2 }},
        {{ "media/images/buttons/wood_button.png", { 88,   80,   8, 5 }, { 1.3, 2.3, 99, 95.4 }}, { m_BaltExtB, "Start Page", m_Brown,
           m_Black, 150, 255, 1, 2 }}
     }),
    m_MapInitData({ "media/images/tiles_image.png" }, "media/maps/maps.data" )

{
    init();
}

CreateMapPageInitData::CreateMapPageInitData( std::vector < ImgTextureInitData > &&ImgTextureInitVc,
                                              std::vector <Text3DInitData> &&Text3DInitVc,
                                              std::vector <ButtonInitData> &&ButtonInitVc,
                                              std::vector <ButtonInitData> &&ButtonNamedInitVc,
                                              MapInitData mapInitData,
                                              Renderer renderer ):

    PageInitData           ( renderer                     ),
    m_ImgInitVc        ( std::move( ImgTextureInitVc  )),
    m_Tex3DInitVc      ( std::move( Text3DInitVc      )),
    m_ButtonInitVc     ( std::move( ButtonInitVc      )),
    m_ButtonNamedInitVc( std::move( ButtonNamedInitVc )),
    m_MapInitData      ( mapInitData                  )
{
    init();
}


CreateMapPageInitData::~CreateMapPageInitData() {}


void CreateMapPageInitData::init()
{
    m_InitContainers.m_pDisplayedObjVc->reserve( m_ImgInitVc.size() + m_Tex3DInitVc.size() + m_ButtonInitVc.size()
                                               + m_ButtonNamedInitVc.size());

    std::for_each( m_ImgInitVc.begin(), m_ImgInitVc.end(), [ this ] ( ImgTextureInitData initData )
    {
        m_InitContainers.m_pDisplayedObjVc->push_back( std::make_shared < Texture > ( initData, m_Renderer ) );
    });

    std::for_each( m_Tex3DInitVc.begin(), m_Tex3DInitVc.end(), [ this ] ( Text3DInitData initData )
    {
        m_InitContainers.m_pDisplayedObjVc->push_back( std::make_shared < Text3D > ( initData, m_Renderer ) );
    });

    std::for_each( m_ButtonInitVc.begin(), m_ButtonInitVc.end(), [ this ] ( ButtonInitData initData )
    {
        auto ptr = std::make_shared < Button > ( initData, m_Renderer );
        m_InitContainers.m_pDisplayedObjVc->push_back( ptr );
        m_InitContainers.m_ButtonVc.push_back( ptr );
    });

    std::for_each( m_ButtonNamedInitVc.begin(), m_ButtonNamedInitVc.end(), [ this ] ( ButtonInitData initData )
    {
        auto ptr = std::make_shared < ButtonNamed > ( initData, m_Renderer );
        m_InitContainers.m_pDisplayedObjVc->push_back( ptr );
        m_InitContainers.m_ButtonVc.push_back( ptr );
    });

    m_InitContainers.m_pMap = std::make_shared < Map > ( m_MapInitData, m_Renderer );
    m_InitContainers.m_pDisplayedObjVc->push_back( m_InitContainers.m_pMap );
}
