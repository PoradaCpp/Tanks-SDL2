#ifndef GAMEPAGEINITDATA_H
#define GAMEPAGEINITDATA_H

#include <vector>

#include "pageinitdata.h"
#include "renderer.h"
#include "buttonnamed.h"
#include "gameengine.h"
#include "tank.h"

class GamePageInitData: public PageInitData
{
public:
    GamePageInitData( Renderer renderer );

    GamePageInitData( std::vector <ImgTextureInitData> &&ImgTextureInitVc1, std::vector <ImgTextureInitData> &&ImgTextureInitVc2,
                      std::vector <Text3DInitData> &&Text3DInitVc1, std::vector <Text3DInitData> &&Text3DInitVc2,
                      std::vector <ButtonInitData> &&ButtonInitVc, std::vector<ButtonInitData> &&ButtonNamedInitVc,
                      MapInitData mapInitData, std::vector <AnimationInitData> AnimationInitDataVc, std::string sPageAudioChunkPath,
                      ObjectsManagementInitData objectsManagementInitData, ImgTextureInitData BackgroundTextureInitData,
                      Renderer renderer );

    ~GamePageInitData() override;

    void init() override;    
    operator pSharedGameEngine && ();

private:
    std::vector <ImgTextureInitData> m_ImgInitVc1;
    std::vector <ImgTextureInitData> m_ImgInitVc2;
    std::vector <Text3DInitData>     m_Tex3DInitVc1;
    std::vector <Text3DInitData>     m_Tex3DInitVc2;
    std::vector <ButtonInitData>     m_ButtonInitVc;
    std::vector <ButtonInitData>     m_ButtonNamedInitVc;
    MapInitData                      m_MapInitData;
    std::vector <AnimationInitData>  m_AnimationInitDataVc;
    pSharedGameEngine                m_pGameEngine;
    std::string                      m_sPageAudioChunkPath;
    ObjectsManagementInitData        m_ObjectsManagementInitData;
    ImgTextureInitData               m_BackgroundTextureInitData;
};

#endif // GAMEPAGEINITDATA_H
