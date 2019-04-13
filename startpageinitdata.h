#ifndef START_PAGE_INIT_DATA_H
#define START_PAGE_INIT_DATA_H

#include <vector>

#include "initdata.h"
#include "renderer.h"
#include "button.h"
#include "tankarrow.h"

class StartPageInitData: public InitData
{
public:
    StartPageInitData( Renderer renderer );

    StartPageInitData( std::vector < ImgTextureInitData > &&ImgTextureInitVc, std::vector < Text3DInitData > &&Text3DInitVc,
                       std::vector < ButtonInitData > &&ButtonInitVc, TankArrowInitData &&tankArrowInitData, Renderer renderer );

    ~StartPageInitData() override;

    void init() override;

private:
    std::vector < ImgTextureInitData > m_ImgInitVc;
    std::vector < Text3DInitData >     m_Tex3DInitVc;
    std::vector < ButtonInitData >     m_ButtonInitVc;

    TankArrowInitData m_TankArrowInitData;
};

#endif // START_PAGE_INIT_DATA_H
