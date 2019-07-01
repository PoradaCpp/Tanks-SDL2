#ifndef TANKPROPERTIES_H
#define TANKPROPERTIES_H

#include <array>

#include "animation.h"
#include "audiochunk.h"
#include "movealgorithm.h"
#include "random_uint32_t.h"

namespace CommonTanksProperties
{
    enum class PlayerNumber
    {
        PLAYER1,
        PLAYER2
    };

    enum class TankOwnerIdentity
    {
        PLAYER1,
        PLAYER2,
        ENEMY
    };

    enum class TankType
    {
        ORDINARY_TANK      = 1,
        BTR                = 2,
        QUICK_SHOOTING_BTR = 3,
        HEAVY_TANK         = 4
    };

    enum class MoveDirection
    {
        UP    =   90,
        DOWN  =  -90,
        LEFT  =   180,
        RIGHT =   0
    };

    const double TANK_SHELL_SCALE = 30;
    const double TANK_SHELL_SHIFT = 45;
    const double BTR_SHELL_SHIFT  = 10;

    const RelativeRect ORDINARY_TANK_SHELL ( TANK_SHELL_SCALE / 2 - TANK_SHELL_SHIFT, TANK_SHELL_SCALE / 2, TANK_SHELL_SCALE,
                                             TANK_SHELL_SCALE );
    const RelativeRect BTR_SHELL           ( TANK_SHELL_SCALE / 2 - BTR_SHELL_SHIFT,  TANK_SHELL_SCALE / 2, TANK_SHELL_SCALE,
                                             TANK_SHELL_SCALE );
    const RelativeRect HEAVY_TANK_SHELL    ( TANK_SHELL_SCALE / 2 - TANK_SHELL_SHIFT, TANK_SHELL_SCALE / 2, TANK_SHELL_SCALE,
                                             TANK_SHELL_SCALE );


    const RelativeRect ORDINARY_TANK ( 0, 0, 97, 71 );
    const RelativeRect BTR           ( 0, 0, 97, 55 );
    const RelativeRect HEAVY_TANK    ( 0, 0, 97, 81 );
    const double QUICK_SHOOTING_BTR_SCALE  = 1.3;


    const uint32_t PLAYERS_TANKS_LIVES                  = 1;
    const uint32_t ENEMY_LIGHT_TANKS_LIVES              = 1;
    const uint32_t ENEMY_HEAVY_TANKS_LIVES              = 3;

    const double ORDINARY_TANK_RELATIVE_SPEED           = 0.125;
    const double BTR_RELATIVE_SPEED                     = 0.25;
    const double SHELL_RELATIVE_SPEED                   = 0.5;

    const uint32_t PLAYERS_TANKS_DEFENSE_ANIM_BEGIN     = 100;
    const uint32_t PLAYERS_TANKS_DEFENSE_ANIM_END       = 103;
    const uint32_t PLAYERS_TANKS_BIRTH_ANIM_BEGIN       = 16;
    const uint32_t PLAYERS_TANKS_BIRTH_ANIM_END         = 31;
    const uint32_t PLAYERS_ORDINARY_TANK_ANIM_BEGIN     = 32;
    const uint32_t PLAYERS_ORDINARY_TANK_ANIM_END       = 35;
    const uint32_t PLAYERS_BTR_ANIM_BEGIN               = 52;
    const uint32_t PLAYERS_BTR_ANIM_END                 = 55;
    const uint32_t PLAYERS_HEAVY_TANK_ANIM_BEGIN        = 72;
    const uint32_t PLAYERS_HEAVY_TANK_ANIM_END          = 75;

    const uint32_t ENEMY_TANKS_BIRTH_ANIM_BEGIN         = 0;
    const uint32_t ENEMY_TANKS_BIRTH_ANIM_END           = 15;
    const uint32_t ENEMY_ORDINARY_TANK_ANIM_BEGIN       = 36;
    const uint32_t ENEMY_ORDINARY_TANK_ANIM_END         = 39;
    const uint32_t ENEMY_BTR_ANIM_BEGIN                 = 56;
    const uint32_t ENEMY_BTR_ANIM_END                   = 59;
    const uint32_t ENEMY_HEAVY_TANK_ANIM_BEGIN          = 76;
    const uint32_t ENEMY_HEAVY_TANK_ANIM_END            = 87;
    const uint32_t ENEMY_HEAVY_TANK_ANIM_LENGTH         = 4;
    const uint32_t ENEMY_BONUS_ORDINARY_TANK_ANIM_BEGIN = 36;
    const uint32_t ENEMY_BONUS_ORDINARY_TANK_ANIM_END   = 51;
    const uint32_t ENEMY_BONUS_BTR_ANIM_BEGIN           = 56;
    const uint32_t ENEMY_BONUS_BTR_ANIM_END             = 71;
    const uint32_t ENEMY_BONUS_HEAVY_TANK_ANIM_BEGIN    = 84;
    const uint32_t ENEMY_BONUS_HEAVY_TANK_ANIM_END      = 99;

    const double PLAYERS_TANK_X_DESTROYING_FORCE        = 0.8;
    const double ENEMY_TANK_X_DESTROYING_FORCE          = 0.7;
    const double PLAYERS_HEAVY_TANK_X_DESTROYING_FORCE  = 1.6;
    const double ENEMY_HEAVY_TANK_X_DESTROYING_FORCE    = 1.4;

    const uint32_t SHELL_ANIM_BEGIN                     = 3;
    const int      SHELL_SIZE_SCALE                     = 50;

    const uint32_t ENEMY_DIRECTION_CHANGE_MIN_TIMEOUT   = 200;
    const uint32_t ENEMY_DIRECTION_CHANGE_MAX_TIMEOUT   = 800;

    const uint32_t ENEMY_ORDINARY_SHOT_PAUSE            = 1000;
    const uint32_t PLAYER_ORDINARY_SHOT_PAUSE           = 800;
    const uint32_t ENEMY_QUICK_SHOT_PAUSE               = 600;
    const uint32_t PLAYER_QUICK_SHOT_PAUSE              = 500;

    const uint32_t ORDINARY_TANK_BONUS                  = 100;
    const uint32_t BTR_BONUS                            = 200;
    const uint32_t QUICK_SHOOTING_BTR_BONUS             = 300;
    const uint32_t HEAVY_TANK_BONUS                     = 400;

    const uint32_t DESTROYING_PAUSE                     = 400;

    const std::array <std::string, 5> PLAYERS_TANK_SOUNDS_PATHS
    ({
         "media/audio/players_engine_work.wav",     // Player's engine work sound
         "media/audio/players_tank_move.wav",       // Player's tank move sound
         "media/audio/players_tank_shot.wav",       // Player's tank shot sound
         "media/audio/players_shell_explosion.wav", // Player's shell explosion sound
         "media/audio/players_tank_explosion.wav"   // Player's tank explosion sound
     });

    const std::array <std::string, 5> ENEMY_TANK_SOUNDS_PATHS
    ({
         "media/audio/enemy_engine_work.wav",       // Enemy engine work sound
         "media/audio/enemy_tank_move.wav",         // Enemy tank move sound
         "media/audio/enemy_tank_shot.wav",         // Enemy tank shot sound
         "media/audio/enemy_shell_explosion.wav",   // Enemy shell explosion sound
         "media/audio/enemy_tank_explosion.wav"     // Enemy tank explosion sound
     });
};

class TankKeyboardMoveAlgo: public MoveAlgorithm
{
public:
    TankKeyboardMoveAlgo( CommonTanksProperties::TankOwnerIdentity playerNumber, CommonTanksProperties::TankType tankType );
    ~TankKeyboardMoveAlgo() override;

    void move( SDL_Rect &CurPos ) override;
    void setDisplayWidth( uint32_t nDisplayWidth ) override;

    void InitPlayer1( SDL_Scancode KeyDown  = SDL_SCANCODE_DOWN, SDL_Scancode KeyUp     = SDL_SCANCODE_UP,
                      SDL_Scancode KeyRight = SDL_SCANCODE_RIGHT, SDL_Scancode KeyLeft  = SDL_SCANCODE_LEFT );

    void InitPlayer2( SDL_Scancode KeyDown  = SDL_SCANCODE_S, SDL_Scancode KeyUp    = SDL_SCANCODE_W,
                      SDL_Scancode KeyRight = SDL_SCANCODE_D, SDL_Scancode KeyLeft  = SDL_SCANCODE_A );

private:
    double m_dRelativeTankSpeed;
    uint32_t m_nTankSpeed;
    SDL_Scancode Down;
    SDL_Scancode Up;
    SDL_Scancode Right;
    SDL_Scancode Left;
};

class TankRandomMoveAlgo: public MoveAlgorithm
{
public:
    TankRandomMoveAlgo( CommonTanksProperties::TankType tankType );
    ~TankRandomMoveAlgo() override;

    void move( SDL_Rect &CurPos ) override;
    void changeDirection() override;
    void setDisplayWidth( uint32_t nDisplayWidth ) override;

private:
    enum class MoveDirection
    {
        UP    = 0,
        DOWN  = 1,
        LEFT  = 2,
        RIGHT = 3
    };

    Random_uint32_t m_RandomDirection;
    Random_uint32_t m_RandomMoveTimeout;
    uint32_t m_nDirectionChangeTimeout;
    uint32_t m_nCurrentTimeoutValue;
    MoveDirection m_MoveDirection;
    double m_dRelativeTankSpeed;
    uint32_t m_nTankSpeed;
};

class TankProperties
{
public:
    friend class Tank;
    friend class TankOfPlayer;
    friend class TankOfEnemy;
    friend class GameEngine;

    TankProperties();
    virtual ~TankProperties();

    virtual void initAsOrdinaryTank()     = 0;
    virtual void initAsBTR()              = 0;
    virtual void initAsQuickShootingBTR() = 0;
    virtual void initAsHeavyTank()        = 0;

protected:
    enum class Sounds
    {
        ENGINE_WORK     = 0,
        MOVE            = 1,
        SHOT            = 2,
        SHELL_EXPLOSION = 3,
        TANK_EXPLOSION  = 4,
        SOUNDS_QUANTITY = 5
    };

    CommonTanksProperties::TankType m_TankType;
    CommonTanksProperties::TankOwnerIdentity m_TankOwnerIdentity;
    pSharedMoveAlgo m_pMoveAlgorithm;
    RelativeRect m_RelativeTankSize;
    RelativeRect m_RelativeTankShellPos;
    double m_dRelativeXDestroyingForce;
    uint32_t m_nNumberOfLives;
    size_t m_nBirthAnimTilesBegin;
    size_t m_nBirthAnimTilesEnd;
    size_t m_nDefenseAnimTilesBegin;
    size_t m_nDefenseAnimTilesEnd;
    size_t m_nBonusTankAnimTilesBegin;
    size_t m_nBonusTankAnimTilesEnd;
    size_t m_nAnimTilesBegin;
    size_t m_nAnimTilesEnd;
    uint32_t m_nShootPause;
    SDL_Scancode Shot;
    bool m_fBonus = false;
};

typedef std::shared_ptr <TankProperties> pSharedTankProperties;

class PlayersTankProperties: public TankProperties
{
public:
    PlayersTankProperties( CommonTanksProperties::TankOwnerIdentity playerNumber = CommonTanksProperties::TankOwnerIdentity::PLAYER1,
                           CommonTanksProperties::TankType PlayersTankType = CommonTanksProperties::TankType::ORDINARY_TANK );
    ~PlayersTankProperties() override;

    void initAsOrdinaryTank() override;
    void initAsBTR() override;
    void initAsQuickShootingBTR() override;
    void initAsHeavyTank() override;
};

class EnemyTankProperties: public TankProperties
{
public:
    EnemyTankProperties( CommonTanksProperties::TankType EnemyTankType = CommonTanksProperties::TankType::ORDINARY_TANK,
                         bool fBonus = false );
    ~EnemyTankProperties() override;

    void initAsOrdinaryTank() override;
    void initAsBTR() override;
    void initAsQuickShootingBTR() override;
    void initAsHeavyTank() override;
};

#endif // TANKPROPERTIES_H
