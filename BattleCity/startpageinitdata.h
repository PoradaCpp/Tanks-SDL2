#ifndef START_PAGE_INIT_DATA_H
#define START_PAGE_INIT_DATA_H

#include <vector>

#include "pageinitdata.h"
#include "renderer.h"
#include "buttonnamed.h"
#include "tankarrow.h"

struct ScoresText3DData
{
    Text3DInitData m_MyScoreTextInitData;
    Text3DInitData m_HighScoreTextInitData;

    ScoresText3DData( Text3DInitData MyScoreTextInitData, Text3DInitData HighScoreTextInitData ):
        m_MyScoreTextInitData( MyScoreTextInitData ), m_HighScoreTextInitData( HighScoreTextInitData ) {}
};

class StartPageInitData: public PageInitData
{
public:
    StartPageInitData( Renderer renderer );

    StartPageInitData( std::vector <ImgTextureInitData> &&ImgTextureInitVc, std::vector <Text3DInitData> &&Text3DInitVc,
                       std::vector <ButtonInitData> &&ButtonInitVc, std::string sPageAudioChunkPath,
                       TankArrowInitData tankArrowInitData, ScoresText3DData scoresText3DData, Renderer renderer );

    ~StartPageInitData() override;

    void init() override;
    operator ScoresText3DData && ();

private:
    std::vector <ImgTextureInitData> m_ImgInitVc;
    std::vector <Text3DInitData>     m_Tex3DInitVc;
    std::vector <ButtonInitData>     m_ButtonInitVc;
    std::string                      m_sPageAudioChunkPath;
    TankArrowInitData                m_TankArrowInitData;
    ScoresText3DData                 m_ScoresText3DData;
};

#endif // START_PAGE_INIT_DATA_H
