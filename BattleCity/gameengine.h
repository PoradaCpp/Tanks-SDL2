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

class GameEngine
{
public:
    friend class Tank;

    GameEngine( std::vector <AnimationInitData> AnimInitDataVc, std::vector <ImgTextureInitData> TextureInitDataVc,
                Renderer renderer, pSharedMap pMap );
    ~GameEngine();

    void attachGlobalGameState( State *pGlobalGameState );
    void updateNumOfPlayers();
    void gameAction();
    void resize();
    void render();

    static const size_t   MAX_ACTIVE_TANKS_NUMBER        = 8;
    static const uint32_t MAX_NUM_OF_ENEMIES             = 20;
    static const uint32_t NUM_OF_ENEMIES_ON_MAP_1PLAYER  = 4;
    static const uint32_t NUM_OF_ENEMIES_ON_MAP_2PLAYERS = 6;
    static const uint32_t START_PLAYERS_LIVES            = 5;
    static const uint32_t FIRST_BONUS_TANK_NUMBER        = 4;
    static const uint32_t SECOND_BONUS_TANK_NUMBER       = 11;
    static const uint32_t THIRD_BONUS_TANK_NUMBER        = 18;

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
        GAME_OFF
    };

    static const RelativeRect PLAYER1_BIRTH_POS;
    static const RelativeRect PLAYER2_BIRTH_POS;
    static const RelativeRect ENEMY1_BIRTH_POS;
    static const RelativeRect ENEMY2_BIRTH_POS;
    static const RelativeRect ENEMY3_BIRTH_POS;
    static const RelativeRect ENEMY4_BIRTH_POS;
    static const std::array <const RelativeRect*, 4> ENEMY_BIRTH_PLACE_ARR;

    static constexpr double LIVES_X_COLUMNS_SHIFT = 4;
    static constexpr double LIVES_Y_ROWS_SHIFT    = 4.9;

    static const size_t LIVES_COLUMNS_NUMBER = 3;
    static const size_t ENEMIES_ROWS_NUMBER  = 7;
    static const size_t PLAYERS_ROWS_NUMBER  = 2;

    std::vector <SDL_Rect> m_EnemyBirthPlaceVc;

    State *m_pGlobalGameState;
    std::list <pSharedTank> m_TanksList;
    std::list <pSharedTankShell> m_TankShellsList;
    std::vector <Animation> m_AnimTextureVc;
    std::vector <Texture>   m_TextureVc;
    std::vector <SDL_Rect>  m_EnemyImagesPlaceVc;
    std::vector <SDL_Rect>  m_Player1ImagesPlaceVc;
    std::vector <SDL_Rect>  m_Player2ImagesPlaceVc;
    Renderer m_Renderer;
    pSharedMap m_pMap;
    NumOfPlayers m_NumOfPlayers;
    uint32_t m_nNumOfEnemies;
    uint32_t m_nNumOfEnemiesOnMap;
    uint32_t m_nNumOfEnemiesAlive;
    uint32_t m_nPlayer1Lives;
    uint32_t m_nPlayer2Lives;
    bool m_fPlayer1Alive;
    bool m_fPlayer2Alive;
    GameState m_GameState;
    Random_uint32_t m_Rand1_4;

    SDL_Rect RelativeToBase( const RelativeRect &relativeRect );
    void calcLiveRectSize();
    void calcLiveRectSize( std::vector <SDL_Rect> &nImagePlaceVc, TexturePurpose texturePurpose );
    void livesAndEnemies();
};

typedef std::shared_ptr <GameEngine> pSharedGameEngine;

#endif // GAMEENGINE_H
