#include <cmath>

#include "tankproperties.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief TankKeyboardMoveAlgo::TankKeyboardMoveAlgo
/// \param playerNumber
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TankKeyboardMoveAlgo::TankKeyboardMoveAlgo( CommonTanksProperties::TankOwnerIdentity playerNumber,
                                            CommonTanksProperties::TankType tankType ):
    MoveAlgorithm(), m_dRelativeTankSpeed( 0 ), m_nTankSpeed( 0 )
{
    if( CommonTanksProperties::TankType::ORDINARY_TANK == tankType || CommonTanksProperties::TankType::HEAVY_TANK == tankType )
    {
        m_dRelativeTankSpeed = CommonTanksProperties::ORDINARY_TANK_RELATIVE_SPEED;
    }
    else
    {
        m_dRelativeTankSpeed = CommonTanksProperties::BTR_RELATIVE_SPEED;
    }
    CommonTanksProperties::TankOwnerIdentity::PLAYER1 == playerNumber ? InitPlayer1() : InitPlayer2();
}

TankKeyboardMoveAlgo::~TankKeyboardMoveAlgo() {}

void TankKeyboardMoveAlgo::move( SDL_Rect &CurPos )
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );

    if( currentKeyStates[ Up ] )
    {
        CurPos.y -= m_nTankSpeed;
    }
    if( currentKeyStates[ Down ] )
    {
        CurPos.y += m_nTankSpeed;
    }
    if( currentKeyStates[ Right ] )
    {
        CurPos.x += m_nTankSpeed;
    }
    if( currentKeyStates[ Left ] )
    {
        CurPos.x -= m_nTankSpeed;
    }
}

void TankKeyboardMoveAlgo::setDisplayWidth( uint32_t nDisplayWidth )
{
    m_nDisplayWidth = nDisplayWidth;
    m_nTankSpeed = static_cast <uint32_t> ( lround( m_dRelativeTankSpeed * m_nDisplayWidth / 100 ) );
}

void TankKeyboardMoveAlgo::InitPlayer1( SDL_Scancode KeyDown, SDL_Scancode KeyUp, SDL_Scancode KeyRight, SDL_Scancode KeyLeft )
{
    Down  = KeyDown;
    Up    = KeyUp;
    Right = KeyRight;
    Left  = KeyLeft;
}

void TankKeyboardMoveAlgo::InitPlayer2( SDL_Scancode KeyDown, SDL_Scancode KeyUp, SDL_Scancode KeyRight, SDL_Scancode KeyLeft )
{
    Down  = KeyDown;
    Up    = KeyUp;
    Right = KeyRight;
    Left  = KeyLeft;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief TankRandomMoveAlgo::TankRandomMoveAlgo
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TankRandomMoveAlgo::TankRandomMoveAlgo( CommonTanksProperties::TankType tankType ): MoveAlgorithm(),
    m_RandomDirection( static_cast <uint32_t> ( MoveDirection::UP ), static_cast <uint32_t> ( MoveDirection::RIGHT ) ),
    m_RandomMoveTimeout( CommonTanksProperties::ENEMY_DIRECTION_CHANGE_MIN_TIMEOUT,
                         CommonTanksProperties::ENEMY_DIRECTION_CHANGE_MAX_TIMEOUT ),
    m_nDirectionChangeTimeout( 0 ), m_nCurrentTimeoutValue( 0 ), m_dRelativeTankSpeed( 0 ), m_nTankSpeed( 0 )
{
    if( CommonTanksProperties::TankType::ORDINARY_TANK == tankType || CommonTanksProperties::TankType::HEAVY_TANK == tankType )
    {
        m_dRelativeTankSpeed = CommonTanksProperties::ORDINARY_TANK_RELATIVE_SPEED;
    }
    else
    {
        m_dRelativeTankSpeed = CommonTanksProperties::BTR_RELATIVE_SPEED;
    }
}

TankRandomMoveAlgo::~TankRandomMoveAlgo() {}

void TankRandomMoveAlgo::move( SDL_Rect &CurPos )
{
    m_nDirectionChangeTimeout = m_RandomMoveTimeout();

    if( ++m_nCurrentTimeoutValue > m_nDirectionChangeTimeout )
    {
        m_MoveDirection = static_cast <MoveDirection>( m_RandomDirection() );
        m_nCurrentTimeoutValue = 0;
    }

    switch( m_MoveDirection )
    {
    case MoveDirection::UP:
        CurPos.y += m_nTankSpeed;
        break;

    case MoveDirection::DOWN:
        CurPos.y -= m_nTankSpeed;
        break;

    case MoveDirection::RIGHT:
        CurPos.x += m_nTankSpeed;
        break;

    case MoveDirection::LEFT:
        CurPos.x -= m_nTankSpeed;
        break;
    }
}

void TankRandomMoveAlgo::changeDirection()
{
    m_MoveDirection = static_cast <MoveDirection>( m_RandomDirection() );
}

void TankRandomMoveAlgo::setDisplayWidth( uint32_t nDisplayWidth )
{
    m_nDisplayWidth = nDisplayWidth;
    m_nTankSpeed = static_cast <uint32_t> ( lround( m_dRelativeTankSpeed * m_nDisplayWidth / 100 ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief TankProperties::TankProperties
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TankProperties::TankProperties() {}

TankProperties::~TankProperties() {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief PlayersTankProperties::PlayersTankProperties
/// \param playerNumber
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PlayersTankProperties::PlayersTankProperties( CommonTanksProperties::TankOwnerIdentity playerNumber,
                                              CommonTanksProperties::TankType PlayersTankType ): TankProperties()
{
    m_pMoveAlgorithm = std::make_shared <TankKeyboardMoveAlgo> ( playerNumber, PlayersTankType );
    Shot = CommonTanksProperties::TankOwnerIdentity::PLAYER1 == playerNumber ? SDL_SCANCODE_KP_0 : SDL_SCANCODE_SPACE;

    m_TankType               = PlayersTankType;
    m_TankOwnerIdentity      = playerNumber;
    m_nNumberOfLives         = CommonTanksProperties::PLAYERS_TANKS_LIVES;
    m_nBirthAnimTilesBegin   = CommonTanksProperties::PLAYERS_TANKS_BIRTH_ANIM_BEGIN;
    m_nBirthAnimTilesEnd     = CommonTanksProperties::PLAYERS_TANKS_BIRTH_ANIM_END;
    m_nDefenseAnimTilesBegin = CommonTanksProperties::PLAYERS_TANKS_DEFENSE_ANIM_BEGIN;
    m_nDefenseAnimTilesEnd   = CommonTanksProperties::PLAYERS_TANKS_DEFENSE_ANIM_END;

    switch( PlayersTankType )
    {
    case CommonTanksProperties::TankType::ORDINARY_TANK:
        initAsOrdinaryTank();
        break;

    case CommonTanksProperties::TankType::BTR:
        initAsBTR();
        break;

    case CommonTanksProperties::TankType::QUICK_SHOOTING_BTR:
        initAsQuickShootingBTR();
        break;

    case CommonTanksProperties::TankType::HEAVY_TANK:
        initAsHeavyTank();
        break;
    }
}

PlayersTankProperties::~PlayersTankProperties() {}

void PlayersTankProperties::initAsOrdinaryTank()
{
    m_RelativeTankSize         = CommonTanksProperties::ORDINARY_TANK;
    m_RelativeTankShellPos     = CommonTanksProperties::ORDINARY_TANK_SHELL;
    m_dRelativeXDestroyingForce = CommonTanksProperties::PLAYERS_TANK_X_DESTROYING_FORCE;
    m_nAnimTilesBegin          = CommonTanksProperties::PLAYERS_ORDINARY_TANK_ANIM_BEGIN;
    m_nAnimTilesEnd            = CommonTanksProperties::PLAYERS_ORDINARY_TANK_ANIM_END;
    m_nShootPause              = CommonTanksProperties::PLAYER_ORDINARY_SHOT_PAUSE;
}

void PlayersTankProperties::initAsBTR()
{
    m_RelativeTankSize         = CommonTanksProperties::BTR;
    m_RelativeTankShellPos     = CommonTanksProperties::BTR_SHELL;
    m_dRelativeXDestroyingForce = CommonTanksProperties::PLAYERS_TANK_X_DESTROYING_FORCE;
    m_nAnimTilesBegin          = CommonTanksProperties::PLAYERS_BTR_ANIM_BEGIN;
    m_nAnimTilesEnd            = CommonTanksProperties::PLAYERS_BTR_ANIM_END;
    m_nShootPause              = CommonTanksProperties::PLAYER_ORDINARY_SHOT_PAUSE;
}

void PlayersTankProperties::initAsQuickShootingBTR()
{
    m_RelativeTankSize         = CommonTanksProperties::BTR;
    m_RelativeTankShellPos     = CommonTanksProperties::BTR_SHELL;
    m_dRelativeXDestroyingForce = CommonTanksProperties::PLAYERS_TANK_X_DESTROYING_FORCE;
    m_nAnimTilesBegin          = CommonTanksProperties::PLAYERS_BTR_ANIM_BEGIN;
    m_nAnimTilesEnd            = CommonTanksProperties::PLAYERS_BTR_ANIM_END;
    m_nShootPause              = CommonTanksProperties::PLAYER_QUICK_SHOT_PAUSE;
}

void PlayersTankProperties::initAsHeavyTank()
{
    m_RelativeTankSize         = CommonTanksProperties::HEAVY_TANK;
    m_RelativeTankShellPos     = CommonTanksProperties::HEAVY_TANK_SHELL;
    m_dRelativeXDestroyingForce = CommonTanksProperties::PLAYERS_HEAVY_TANK_X_DESTROYING_FORCE;
    m_nAnimTilesBegin          = CommonTanksProperties::PLAYERS_HEAVY_TANK_ANIM_BEGIN;
    m_nAnimTilesEnd            = CommonTanksProperties::PLAYERS_HEAVY_TANK_ANIM_END;
    m_nShootPause              = CommonTanksProperties::PLAYER_ORDINARY_SHOT_PAUSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief EnemyTankProperties::EnemyTankProperties
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EnemyTankProperties::EnemyTankProperties( CommonTanksProperties::TankType EnemyTankType, bool fBonus ): TankProperties()
{    
    m_TankType                 = EnemyTankType;
    m_TankOwnerIdentity        = CommonTanksProperties::TankOwnerIdentity::ENEMY;
    m_pMoveAlgorithm           = std::make_shared <TankRandomMoveAlgo> ( EnemyTankType );
    m_nBonusTankAnimTilesBegin = 0;
    m_nBonusTankAnimTilesEnd   = 0;
    m_nBirthAnimTilesBegin     = CommonTanksProperties::ENEMY_TANKS_BIRTH_ANIM_BEGIN;
    m_nBirthAnimTilesEnd       = CommonTanksProperties::ENEMY_TANKS_BIRTH_ANIM_END;
    m_fBonus                   = fBonus;

    switch( EnemyTankType )
    {
    case CommonTanksProperties::TankType::ORDINARY_TANK:
        initAsOrdinaryTank();

        if( fBonus )
        {
            m_nBonusTankAnimTilesBegin = CommonTanksProperties::ENEMY_BONUS_ORDINARY_TANK_ANIM_BEGIN;
            m_nBonusTankAnimTilesEnd   = CommonTanksProperties::ENEMY_BONUS_ORDINARY_TANK_ANIM_END;
        }
        break;

    case CommonTanksProperties::TankType::BTR:
        initAsBTR();

        if( fBonus )
        {
            m_nBonusTankAnimTilesBegin = CommonTanksProperties::ENEMY_BONUS_BTR_ANIM_BEGIN;
            m_nBonusTankAnimTilesEnd   = CommonTanksProperties::ENEMY_BONUS_BTR_ANIM_END;
        }
        break;

    case CommonTanksProperties::TankType::QUICK_SHOOTING_BTR:
        initAsQuickShootingBTR();

        if( fBonus )
        {
            m_nBonusTankAnimTilesBegin = CommonTanksProperties::ENEMY_BONUS_BTR_ANIM_BEGIN;
            m_nBonusTankAnimTilesEnd   = CommonTanksProperties::ENEMY_BONUS_BTR_ANIM_END;
        }
        break;

    case CommonTanksProperties::TankType::HEAVY_TANK:
        initAsHeavyTank();

        if( fBonus )
        {
            m_nBonusTankAnimTilesBegin = CommonTanksProperties::ENEMY_BONUS_HEAVY_TANK_ANIM_BEGIN;
            m_nBonusTankAnimTilesEnd   = CommonTanksProperties::ENEMY_BONUS_HEAVY_TANK_ANIM_END;
        }
        break;
    }
}

EnemyTankProperties::~EnemyTankProperties() {}

void EnemyTankProperties::initAsOrdinaryTank()
{
    m_RelativeTankSize         = CommonTanksProperties::ORDINARY_TANK;
    m_RelativeTankShellPos     = CommonTanksProperties::ORDINARY_TANK_SHELL;
    m_dRelativeXDestroyingForce = CommonTanksProperties::ENEMY_TANK_X_DESTROYING_FORCE;
    m_nNumberOfLives           = CommonTanksProperties::ENEMY_LIGHT_TANKS_LIVES;
    m_nAnimTilesBegin          = CommonTanksProperties::ENEMY_ORDINARY_TANK_ANIM_BEGIN;
    m_nAnimTilesEnd            = CommonTanksProperties::ENEMY_ORDINARY_TANK_ANIM_END;
    m_nShootPause              = CommonTanksProperties::ENEMY_ORDINARY_SHOT_PAUSE;
}

void EnemyTankProperties::initAsBTR()
{
    m_RelativeTankSize         = CommonTanksProperties::BTR;
    m_RelativeTankShellPos     = CommonTanksProperties::BTR_SHELL;
    m_dRelativeXDestroyingForce = CommonTanksProperties::ENEMY_TANK_X_DESTROYING_FORCE;
    m_nNumberOfLives           = CommonTanksProperties::ENEMY_LIGHT_TANKS_LIVES;
    m_nAnimTilesBegin          = CommonTanksProperties::ENEMY_BTR_ANIM_BEGIN;
    m_nAnimTilesEnd            = CommonTanksProperties::ENEMY_BTR_ANIM_END;
    m_nShootPause              = CommonTanksProperties::ENEMY_ORDINARY_SHOT_PAUSE;
}

void EnemyTankProperties::initAsQuickShootingBTR()
{
    m_RelativeTankSize         = CommonTanksProperties::BTR;
    m_RelativeTankShellPos     = CommonTanksProperties::BTR_SHELL;
    m_dRelativeXDestroyingForce = CommonTanksProperties::ENEMY_TANK_X_DESTROYING_FORCE;
    m_nNumberOfLives           = CommonTanksProperties::ENEMY_LIGHT_TANKS_LIVES;
    m_nAnimTilesBegin          = CommonTanksProperties::ENEMY_BTR_ANIM_BEGIN;
    m_nAnimTilesEnd            = CommonTanksProperties::ENEMY_BTR_ANIM_END;
    m_nShootPause              = CommonTanksProperties::ENEMY_QUICK_SHOT_PAUSE;
}

void EnemyTankProperties::initAsHeavyTank()
{
    m_RelativeTankSize         = CommonTanksProperties::HEAVY_TANK;
    m_RelativeTankShellPos     = CommonTanksProperties::HEAVY_TANK_SHELL;
    m_dRelativeXDestroyingForce = CommonTanksProperties::ENEMY_HEAVY_TANK_X_DESTROYING_FORCE;
    m_nNumberOfLives           = CommonTanksProperties::ENEMY_HEAVY_TANKS_LIVES;
    m_nAnimTilesBegin          = CommonTanksProperties::ENEMY_HEAVY_TANK_ANIM_BEGIN;
    m_nAnimTilesEnd            = CommonTanksProperties::ENEMY_HEAVY_TANK_ANIM_END;
    m_nShootPause              = CommonTanksProperties::ENEMY_ORDINARY_SHOT_PAUSE;
}
