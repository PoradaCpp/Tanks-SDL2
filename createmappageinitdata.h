#ifndef CREATEMAPPAGEINITDATA_H
#define CREATEMAPPAGEINITDATA_H


#include <vector>

#include "initdata.h"
#include "renderer.h"
#include "buttonnamed.h"
#include "tankarrow.h"

class CreateMapPageInitData: public InitData
{
public:
    CreateMapPageInitData( Renderer renderer );

    CreateMapPageInitData( std::vector < ImgTextureInitData >  &&ImgTextureInitVc, std::vector < Text3DInitData > &&Text3DInitVc,
                           std::vector < ButtonInitData > &&ButtonInitVc, std::vector<ButtonInitData> &&ButtonNamedInitVc,
                           MapInitData mapInitData, Renderer renderer );

    ~CreateMapPageInitData() override;

    void init() override;

private:
    std::vector < ImgTextureInitData > m_ImgInitVc;
    std::vector < Text3DInitData >     m_Tex3DInitVc;
    std::vector < ButtonInitData >     m_ButtonInitVc;
    std::vector < ButtonInitData >     m_ButtonNamedInitVc;
    MapInitData m_MapInitData;
};

#endif // CREATEMAPPAGEINITDATA_H
