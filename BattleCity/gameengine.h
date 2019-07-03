#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include <array>
#include <list>
#include <memory>

#include "dynamicobject.h"
#include "animation.h"
#include "tank.h"
#include "state.h"
#include "random_uint32_t.h"
#include "objectsmanagement.h"

class GamePage;

class GameEngine
{
public:
    friend class Tank;
    friend class ObjectsManagement;

    GameEngine( std::vector <AnimationInitData> AnimInitDataVc, std::vector <ImgTextureInitData> LivesAndEnemiesInitDataVc,
                ObjectsManagementInitData objectsManagementInitData, Renderer renderer, pSharedMap pMap );
    ~GameEngine();

    void attachGamePage( GamePage *pGamePage );
    void gameStartInit();
    void gameAction();
    void resize();
    void render();
    void setPause();
    void resetPause();
    bool isGameOn();
    bool isGamePaused();

private:
    enum class AnimPurpose
    {
        TANK_MOVE_ANIM      = 0,
        TANK_EXPLOSION_ANIM = 1,
        BOMB_EXPLOSION_ANIM = 2
    };
    enum class TexturePurpose
    {
        TANKS_OF_PLAYER1_LIFE = 0,
        TANKS_OF_PLAYER2_LIFE = 1,
        CREATED_ENEMY_IMAGE   = 2,
        REMAIN_ENEMY_IMAGE    = 3
    };
    enum class GameState
    {
        GAME_ON,
        GAME_PAUSED,
        GAME_OFF,
        GAME_WON
    };

    static const RelativeRect PLAYER1_BIRTH_POS;
    static const RelativeRect PLAYER2_BIRTH_POS;
    static const RelativeRect ENEMY1_BIRTH_POS;
    static const RelativeRect ENEMY2_BIRTH_POS;
    static const RelativeRect ENEMY3_BIRTH_POS;
    static const RelativeRect ENEMY4_BIRTH_POS;
    static const std::array <const RelativeRect*, 4> ENEMY_BIRTH_PLACE_ARR;

    static const int32_t  LIVES_X_COLUMNS_DIV = 2;
    static const int32_t  LIVES_Y_ROWS_DIV    = 6;

    static const uint32_t MAX_NUM_OF_ENEMIES             = 4;
    static const uint32_t NUM_OF_ENEMIES_ON_MAP_1PLAYER  = 4;
    static const uint32_t NUM_OF_ENEMIES_ON_MAP_2PLAYERS = 6;
    static const uint32_t START_PLAYERS_LIVES            = 5;
    static const uint32_t MAX_PLAYERS_LIVES              = 6;
    static const uint32_t FIRST_BONUS_TANK_NUMBER        = 3;
    static const uint32_t SECOND_BONUS_TANK_NUMBER       = 10;
    static const uint32_t THIRD_BONUS_TANK_NUMBER        = 17;
    static const uint32_t GAME_OVER_TIMEOUT              = 5000;
    static const uint32_t GAME_WON_TIMEOUT               = 7000;
    static const size_t   LIVES_COLUMNS_NUMBER           = 3;
    static const size_t   ENEMIES_ROWS_NUMBER            = 7;
    static const size_t   PLAYERS_ROWS_NUMBER            = 2;

    GamePage *m_pGamePage = nullptr;
    pSharedObjectsManagement m_pObjectsManagement;

    std::vector <Animation>  m_AnimTextureVc;
    std::vector <Texture>    m_LivesAndEnemiesTextureVc;
    std::vector <AudioChunk> m_PlayersAudioVc;
    std::vector <AudioChunk> m_EnemyAudioVc;
    std::vector <SDL_Rect>   m_EnemyBirthPlaceVc;
    std::vector <SDL_Rect>   m_EnemyImagesPlaceVc;
    std::vector <SDL_Rect>   m_Player1ImagesPlaceVc;
    std::vector <SDL_Rect>   m_Player2ImagesPlaceVc;

    SDL_Rect     m_Player1BirthPlace;
    SDL_Rect     m_Player2BirthPlace;
    Renderer     m_Renderer;
    pSharedMap   m_pMap;
    NumOfPlayers m_NumOfPlayers           = NumOfPlayers::ONE_PLAYER;
    uint32_t     m_nHighScore             = 0;
    uint32_t     m_nGameStateChangingTime = 0;
    uint32_t     m_nNumOfEnemies          = 0;
    uint32_t     m_nNumOfEnemiesOnMap     = NUM_OF_ENEMIES_ON_MAP_1PLAYER;
    uint32_t     m_nNumOfEnemiesAlive     = 0;
    uint32_t     m_nPlayer1Lives          = START_PLAYERS_LIVES;
    uint32_t     m_nPlayer2Lives          = 0;
    bool         m_fPlayer1Alive          = false;
    bool         m_fPlayer2Alive          = false;

    CommonTanksProperties::TankType
                 m_Pl1DefTankType         = CommonTanksProperties::TankType::ORDINARY_TANK;
    CommonTanksProperties::TankType
                 m_Pl2DefTankType         = CommonTanksProperties::TankType::ORDINARY_TANK;
    GameState    m_CurrentState           = GameState::GAME_OFF;
    GameState    m_PreviousState          = GameState::GAME_PAUSED;
    Random_uint32_t m_Rand1_4             = Random_uint32_t( 1, 4 );

    void animInit( std::vector <AnimationInitData> AnimInitDataVc, Renderer renderer );
    void livesAndEnemiesInit( std::vector <ImgTextureInitData> LivesAndEnemiesInitDataVc, Renderer renderer );
    void soundsInit();

    SDL_Rect RelativeToBase( const RelativeRect &relativeRect );
    void calcLiveRectSize();
    void calcLiveRectSize( std::vector <SDL_Rect> &nImagePlaceVc, TexturePurpose texturePurpose );
    void livesAndEnemies();
    void calcBirthPlacesRectSize();
    void playersBirth( const CommonTanksProperties::TankOwnerIdentity &tankOwner, const uint32_t &nCurrentLives, bool &fPlayerAlive );
    void enemiesBirth();
    void tanksManagement();
    void tankShellsManagement();

    void tanksDecrease( CommonTanksProperties::TankOwnerIdentity tankOwner );
    void livesIncrease( CommonTanksProperties::TankOwnerIdentity tankOwner );
    void setPlayersTankType( CommonTanksProperties::TankOwnerIdentity tankOwner, CommonTanksProperties::TankType tankType );
    void resetPlayersTankType( CommonTanksProperties::TankOwnerIdentity tankOwner );
};

typedef std::shared_ptr <GameEngine> pSharedGameEngine;

#endif // GAMEENGINE_H
