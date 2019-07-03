#ifndef SCORESCOUNTING_H
#define SCORESCOUNTING_H

#include <vector>
#include <array>

#include "text3d.h"
#include "tank.h"

class ScoresCounting
{
public:
    ScoresCounting( ImgTextureInitData BackgroundTextureInitData, std::vector <Text3DInitData> ScoresFormTextInitDataVc,
                    std::vector <Text3DInitData> AmountsTextInitDataVc, std::string sScoresCountingAudioPath, Renderer renderer );
    ~ScoresCounting();

    void render();
    void changeSize();
    void scoresProcessing( const pSharedTank &pTank, const pSharedTankShell &pShell );
    void addToScores( CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity, uint32_t nScores );
    void resetScores();
    void resetStageScores();
    void resetScoresCounter();
    void scoresCounting();
    void setStageNumber( uint32_t nStageNumber );
    void setHighScore( uint32_t nHighScore );
    uint32_t getCurrentScore() const;

private:
    enum class PointsAndAmounts
    {
        HIGH_SCORE            = 0,
        STAGE_NUMBER          = 1,
        PLAYER_1_SCORE        = 2,
        PLAYER_2_SCORE        = 3,
        PLAIN_TANKS_SCORE     = 4,
        AMOUNT_OF_PLAIN_TANKS = 5,
        BTR_SCORE             = 6,
        AMOUNT_OF_BTR         = 7,
        QS_BTR_SCORE          = 8,
        AMOUNT_OF_QS_BTR      = 9,
        HEAVY_TANKS_SCORE     = 10,
        AMOUNT_OF_HEAVY_TANKS = 11,
        ARR_SIZE              = 12
    };

    static const uint32_t SCORE_COUNTING_DELAY_TIME = 400;

    Texture                      m_BackgroundTexture;
    std::vector <pSharedText3D>  m_ScoresFormTextVc;
    std::vector <pSharedText3D>  m_AmountsTextVc;
    std::array  <uint32_t, static_cast <size_t> ( PointsAndAmounts::ARR_SIZE )>
                                 m_PointsAndAmountsArr = { 0 };
    AudioChunk                   m_ScoresCountingAudio;

    bool     m_fScoreCountingPause = false;
    uint32_t m_nScoreCountingTime  = 0;
    uint32_t m_nCurrentScorePos    = 0;
    uint32_t m_nScoreCounter       = 0;
    uint32_t m_nScoreForPiece      = 0;
    uint32_t m_nQuantity           = 0;
};

#endif // SCORESCOUNTING_H
