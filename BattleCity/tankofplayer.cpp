#include <algorithm>

#include "tankofplayer.h"
#include "tankshell.h"

TankOfPlayer::TankOfPlayer(Animation TankMoveAnim, Animation TankExplosionAnim, Animation BombExplosionAnim, pSharedMap pMap,
                            RelativeRect RelativeBirthPos, GameEngine *pGameEngine, CommonTanksProperties::MoveDirection moveDirection ):
    Tank( TankMoveAnim, TankExplosionAnim, BombExplosionAnim, pMap, RelativeBirthPos, pGameEngine, moveDirection ),
    m_TankDefenseAnim( TankMoveAnim ), m_DefenseRelativeRect( 0, 0, 0, 0 ), m_nDefenseTime( 0 ), m_fDefense( true ) {}

TankOfPlayer::~TankOfPlayer() {}

void TankOfPlayer::changeSize()
{
    Tank::changeSize();
    m_TankDefenseAnim.changeSize();
}

void TankOfPlayer::render()
{
    Tank::render();        

    if( !m_fBirth && m_fDefense )
    {
        if( !m_nDefenseTime )
        {
            m_nDefenseTime = SDL_GetTicks();
        }
        else if( SDL_GetTicks() - m_nDefenseTime <= DEFENSE_ON_TIME )
        {
            m_TankDefenseAnim.startAnimation( m_pProperties->m_nDefenseAnimTilesBegin, m_pProperties->m_nDefenseAnimTilesEnd );
            m_TankDefenseAnim.render( static_cast <double> ( m_MoveDirection ) );
        }
        else
        {
            m_fDefense = false;
        }
    }
}

void TankOfPlayer::attachProperties( pSharedTankProperties pProperties )
{
    Tank::attachProperties( pProperties );
    m_DefenseRelativeRect = m_TankMoveAnim.getRelativeDestination();
    double dWidth  = m_DefenseRelativeRect.m_dWidth  * m_pProperties->m_RelativeTankSize.m_dWidth  / 100;
    double dHeight = m_DefenseRelativeRect.m_dHeight * m_pProperties->m_RelativeTankSize.m_dHeight / 100;

    m_DefenseRelativeRect.m_dX     += ( dWidth  - m_DefenseRelativeRect.m_dWidth  ) / 2;
    m_DefenseRelativeRect.m_dY     += ( dHeight - m_DefenseRelativeRect.m_dHeight ) / 2;
    m_DefenseRelativeRect.m_dWidth  = dWidth;
    m_DefenseRelativeRect.m_dHeight = dHeight;

    m_TankDefenseAnim.setRelativeDestination( m_DefenseRelativeRect );
}

void TankOfPlayer::changePosition()
{
    Tank::changePosition();
    m_TankDefenseAnim.setPosition( m_RealSizeRectHoriz.x, m_RealSizeRectHoriz.y );
}

pSharedTankShell TankOfPlayer::tankShot()
{
    if( m_pProperties->m_nNumberOfLives )
    {
        if( m_pProperties->m_nNumberOfLives )
        {
            const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );

            if( !m_nShotTime )
            {
                m_nShotTime = SDL_GetTicks();
            }
            else if( currentKeyStates[ m_pProperties->Shot ] && SDL_GetTicks() - m_nShotTime >= ( m_pProperties->m_nShootPause ) )
            {
                m_nShotTime = 0;
                auto Shell = std::make_shared <TankShell> ( m_ShellExplosionAnim, m_pProperties->m_RelativeTankShellPos,
                                                            m_MoveDirection, m_pProperties->m_TankOwnerIdentity,
                                                            m_pProperties->m_dRelativeXDestroyingForce );

                Shell->setPosition( m_ShellPosition.x, m_ShellPosition.y );

                return Shell;
            }
        }
    }

    return pSharedTankShell();
}

bool TankOfPlayer::checkTankShellCollision( const SDL_Rect &Rect, CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity )
{
    if( ( SDL_HasIntersection( &Rect, m_pRealCurPos ) ) && (( m_pProperties->m_TankOwnerIdentity != tankOwnerIdentity ) &&
          (( CommonTanksProperties::TankOwnerIdentity::ENEMY == tankOwnerIdentity ) ||
           ( CommonTanksProperties::TankOwnerIdentity::ENEMY == m_pProperties->m_TankOwnerIdentity )) ) )
    {
        if( !m_fDefense && m_pProperties->m_nNumberOfLives )
        {
            --m_pProperties->m_nNumberOfLives;
        }
        return true;
    }
    return false;
}
