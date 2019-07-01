#ifndef OBJECTS_MANAGEMENT_H
#define OBJECTS_MANAGEMENT_H

#include <vector>
#include <array>
#include <list>
#include <memory>

#include "dynamicobject.h"
#include "animation.h"
#include "tank.h"
#include "state.h"
#include "random_uint32_t.h"
#include "playersheart.h"
#include "bonus.h"

struct ObjectsManagementInitData
{
    PlayersHeartInitData m_HeartInitData;
    Text3DInitData       m_GameOverTextInitData;
    ImgTextureInitData   m_BonusTextureInitData;
    std::string          m_sBonusAddAudioPath;
    std::string          m_sBonusTakenAudioPath;

    ObjectsManagementInitData( PlayersHeartInitData HeartInitData, Text3DInitData GameOverTextInitData,
                               ImgTextureInitData BonusTextureInitData, std::string sBonusAddAudioPath,
                               std::string sBonusTakenAudioPath ): m_HeartInitData( HeartInitData ),
        m_GameOverTextInitData( GameOverTextInitData ), m_BonusTextureInitData( BonusTextureInitData ),
        m_sBonusAddAudioPath( sBonusAddAudioPath ), m_sBonusTakenAudioPath( sBonusTakenAudioPath ) {}
};

class GameEngine;

typedef std::pair <pSharedTank, pSharedText3D> TankAndTextPair;

class ObjectsManagement
{
public:
    friend class Tank;
    friend class GameEngine;

    ObjectsManagement( GameEngine *pGameEngine, pSharedMap pMap, ObjectsManagementInitData InitData, Renderer renderer );

    ~ObjectsManagement();

    void init();
    void addTank( pSharedTank pTank );
    void disableBonus();
    bool heartIsDestroyed() const;
    void gameEndClear();
    void destroyPlayersTanks();
    bool checkTankCollision( SDL_Rect &checkingRect ) const;
    bool checkHeartCollision( SDL_Rect &checkingRect ) const;
    void mainProcess();

    void resize();
    void render();
    void setPause();

private:
    static constexpr double BONUS_BIRTH_BORDER    = 0.25;
    static constexpr double BONUS_BIRTH_RANGE     = 0.35;

    static const uint32_t ENEMY_TANK_STOP_TIME    = 10000;
    static const uint32_t BASE_PROTECTION_ON_TIME = 20000;
    static const uint32_t BONUS_SCORES_SHOW_TIME  = 1000;
    static constexpr char const *BONUS_SCORES     = "500";

    static constexpr SDL_Color GREY               = {  40,  40,  40, 255 };
    static constexpr SDL_Color WHITE              = { 255, 255, 255, 255 };

    static const RelativeRect SCORES_TEXT_SIZE;

    GameEngine    *m_pGameEngine = nullptr;
    Texture        m_BonusesTexture;
    Renderer       m_Renderer;
    pSharedBonus   m_pBonus;
    PlayersHeart   m_PlayersHeart;
    Text3D         m_GameOverText;
    pSharedMap     m_pMap;
    AudioChunk     m_BonusAddAudio;
    AudioChunk     m_BonusTakenAudio;
    Text3DInitData m_ScoresTextData;
    pSharedText3D  m_pBonusScoresText;

    std::list <TankAndTextPair>  m_TanksList;
    std::list <pSharedTankShell> m_TankShellsList;

    Random_uint32_t m_Rand0_100 = Random_uint32_t( 0, 100 );
    Random_uint32_t m_RandBonus = Random_uint32_t( 0, Bonus::BONUS_QUANTITY - 1 );

    uint32_t m_nNumOfDestroyedPlainTank = 0;
    uint32_t m_nNumOfDestroyedBTR       = 0;
    uint32_t m_nNumOfDestroyedQS_BTR    = 0;
    uint32_t m_nNumOfDestroyedHeavyTank = 0;
    uint32_t m_nTotalPlayer1Scores      = 0;
    uint32_t m_nTotalPlayer2Scores      = 0;

    uint32_t m_nEnemyTankStopTime   = 0;
    bool     m_fEnemyTankStop       = false;

    uint32_t m_nBaseDefenseOnTime   = 0;
    bool     m_fBaseDefenseOn       = false;

    uint32_t m_nBonusScoresShowTime = 0;
    bool     m_fBonusScoresShow     = false;

    void tanksManagement();
    void tankShellsManagement();
    void createBonus();
    void bonusProcessing( pSharedTank pTank );
    void destroyEnemiesTanks();
    void tankStopProcessing();
    void tankUpgrade( pSharedTank pTank );
    void playersBaseDefense();
    void scoresProcessing( TankAndTextPair &TankAndText, const pSharedTankShell &pShell );
    void scoresTextCreating( TankAndTextPair &TankAndText );
    void bonusScoresTextCreating();
    void resetScores();
    void gameOverTextRender();
};

typedef std::shared_ptr <ObjectsManagement> pSharedObjectsManagement;

#endif // OBJECTS_MANAGEMENT_H
