#include <algorithm>

#include "tankofenemy.h"
#include "tankshell.h"

TankOfEnemy::TankOfEnemy(Animation TankMoveAnim, Animation TankExplosionAnim, Animation BombExplosionAnim, pSharedMap pMap,
                            RelativeRect RelativeBirthPos, GameEngine *pGameEngine, MoveDirection moveDirection ):
    Tank( TankMoveAnim, TankExplosionAnim, BombExplosionAnim, pMap, RelativeBirthPos, pGameEngine, moveDirection ),
    m_nDeadLockChangeDirectionTime( 0 ) {}

TankOfEnemy::~TankOfEnemy() {}

void TankOfEnemy::bonusAnimControl()
{
    if( m_pProperties->m_fBonus )
    {
        m_nAnimBegin = m_pProperties->m_nBonusTankAnimTilesBegin;
        m_nAnimEnd = m_pProperties->m_nBonusTankAnimTilesEnd;
    }
    else if( CommonTanksProperties::TankType::HEAVY_TANK == m_pProperties->m_TankType )
    {
        m_nAnimBegin = m_pProperties->m_nAnimTilesBegin + CommonTanksProperties::ENEMY_HEAVY_TANK_ANIM_LENGTH *
                ( CommonTanksProperties::ENEMY_HEAVY_TANKS_LIVES - m_pProperties->m_nNumberOfLives );
        m_nAnimEnd = m_nAnimBegin + CommonTanksProperties::ENEMY_HEAVY_TANK_ANIM_LENGTH - 1;
    }
    else
    {
        m_nAnimBegin = m_pProperties->m_nAnimTilesBegin;
        m_nAnimEnd = m_pProperties->m_nAnimTilesEnd;
    }
}

void TankOfEnemy::newRandomDirection()
{
    if( !m_nDeadLockChangeDirectionTime )
    {
        m_nDeadLockChangeDirectionTime = SDL_GetTicks();
    }
    else if( SDL_GetTicks() - m_nDeadLockChangeDirectionTime >= CommonTanksProperties::ENEMY_DIRECTION_CHANGE_MIN_TIMEOUT )
    {
        m_pProperties->m_pMoveAlgorithm->changeDirection();
        m_nDeadLockChangeDirectionTime = 0;
    }
}

void TankOfEnemy::changePosition()
{
    if( m_pProperties->m_nNumberOfLives )
    {
        bonusAnimControl();

        if( !newPositionProcessing() )
        {
            newRandomDirection();
        }
    }
    else
    {
        tankExplosing();
    }
}