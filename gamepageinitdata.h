#ifndef GAMEPAGEINITDATA_H
#define GAMEPAGEINITDATA_H

#include <vector>

#include "initdata.h"
#include "renderer.h"
#include "buttonnamed.h"

class GamePageInitData: public InitData
{
public:
    GamePageInitData( Renderer renderer );

    GamePageInitData( std::vector < ImgTextureInitData >  &&ImgTextureInitVc, std::vector < Text3DInitData > &&Text3DInitVc,
                      std::vector < ButtonInitData > &&ButtonInitVc, std::vector<ButtonInitData> &&ButtonNamedInitVc,
                      MapInitData mapInitData, Renderer renderer );

    ~GamePageInitData() override;

    void init() override;

private:
    std::vector < ImgTextureInitData > m_ImgInitVc;
    std::vector < Text3DInitData >     m_Tex3DInitVc;
    std::vector < ButtonInitData >     m_ButtonInitVc;
    std::vector < ButtonInitData >     m_ButtonNamedInitVc;
    MapInitData m_MapInitData;
};

#endif // GAMEPAGEINITDATA_H
