#include "tankshell.h"

TankShell::TankShell( Animation ShellExplosionAnim, AudioChunk shellExplosionSound, RelativeRect RelativePos,
                      CommonTanksProperties::MoveDirection moveDirection, CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity,
                      double dRelativeXDestroyingForce ):
    m_ShellAnim( ShellExplosionAnim ), m_ExplosionSound( shellExplosionSound ), m_CurPos({ 0, 0, 0, 0 }),
    m_CollisionSize({ 0, 0, 0, 0 }), m_HorizShellDestroyingVolume({ 0, 0, 0, 0 }), m_VertShellDestroyingVolume({ 0, 0, 0, 0 }),
    m_dRelativeXDestroyingForce( dRelativeXDestroyingForce ), m_MoveDirection( moveDirection ),
    m_TankOwnerIdentity( tankOwnerIdentity), m_fExplosion( false ), m_fExplosed( false )
{
    m_ShellAnim.setAnimFrames( CommonTanksProperties::SHELL_ANIM_BEGIN );
    m_ShellAnim.setRelativeDestination( RelativePos );
    recalcPosition();
    calcDestroyingVolume();
    m_nTankShellSpeed = static_cast <uint32_t> ( lround( CommonTanksProperties::SHELL_RELATIVE_SPEED *
                                                         m_ShellAnim.getPageWidth() / 100 ) );
}

TankShell::~TankShell() {}

void TankShell::recalcPosition()
{
    m_CurPos = m_ShellAnim.getDestination();
    m_CollisionSize.w = lround( m_CurPos.w * CommonTanksProperties::SHELL_SIZE_SCALE / 100.0 );
    m_CollisionSize.h = lround( m_CurPos.h * CommonTanksProperties::SHELL_SIZE_SCALE / 100.0 );
    m_CollisionSize.x = m_CurPos.x + ( m_CurPos.w - m_CollisionSize.w ) / 2;
    m_CollisionSize.y = m_CurPos.y + ( m_CurPos.h - m_CollisionSize.h ) / 2;
}

void TankShell::calcDestroyingVolume()
{
    m_HorizShellDestroyingVolume.w = lround( m_CurPos.w * 100.0 / CommonTanksProperties::TANK_SHELL_SCALE );
    m_HorizShellDestroyingVolume.h = lround( m_ShellAnim.getPageWidth() * m_dRelativeXDestroyingForce / 100 );

    m_VertShellDestroyingVolume.w = lround( m_ShellAnim.getPageWidth() * m_dRelativeXDestroyingForce / 100 );
    m_VertShellDestroyingVolume.h = lround( m_CurPos.h * 100.0 / CommonTanksProperties::TANK_SHELL_SCALE );
}

void TankShell::changeSize()
{
    m_ShellAnim.changeSize();
    recalcPosition();
    calcDestroyingVolume();
    m_nTankShellSpeed = static_cast <uint32_t> ( lround( CommonTanksProperties::SHELL_RELATIVE_SPEED *
                                                         m_ShellAnim.getPageWidth() / 100 ) );
}

void TankShell::render()
{
    m_ShellAnim.render();
}

void TankShell::setExplosion()
{
    if( !m_fExplosion )
    {
        m_fExplosion = true;
        if(CommonTanksProperties::TankOwnerIdentity::ENEMY != m_TankOwnerIdentity )
        {
            m_ExplosionSound.play();
        }
        int nWidth  = lround( m_CurPos.w * 100 / CommonTanksProperties::TANK_SHELL_SCALE );
        int nHeight = lround( m_CurPos.h * 100 / CommonTanksProperties::TANK_SHELL_SCALE );
        m_CurPos.x += ( m_CurPos.w - nWidth ) / 2;
        m_CurPos.y += ( m_CurPos.h - nHeight ) / 2;
        m_CurPos.w = nWidth;
        m_CurPos.h = nHeight;

        m_ShellAnim.setDestination( m_CurPos );
    }
}

bool TankShell::getExplosion()
{
    return m_fExplosion;
}

bool TankShell::isExplosed()
{
    return m_fExplosed;
}

void TankShell::changePosition()
{
    if( !m_fExplosion )
    {
        switch( m_MoveDirection )
        {
        case CommonTanksProperties::MoveDirection::UP:
            m_CurPos.y        -= m_nTankShellSpeed;
            m_CollisionSize.y -= m_nTankShellSpeed;
            m_HorizShellDestroyingVolume.x = m_CurPos.x + ( m_CurPos.w - m_HorizShellDestroyingVolume.w ) / 2;
            m_HorizShellDestroyingVolume.y = m_CurPos.y + ( m_CurPos.h - m_HorizShellDestroyingVolume.h ) / 2;
            break;

        case CommonTanksProperties::MoveDirection::DOWN:
            m_CurPos.y        += m_nTankShellSpeed;
            m_CollisionSize.y += m_nTankShellSpeed;
            m_HorizShellDestroyingVolume.x = m_CurPos.x + ( m_CurPos.w - m_HorizShellDestroyingVolume.w ) / 2;
            m_HorizShellDestroyingVolume.y = m_CurPos.y + ( m_CurPos.h - m_HorizShellDestroyingVolume.h ) / 2;
            break;

        case CommonTanksProperties::MoveDirection::RIGHT:
            m_CurPos.x        -= m_nTankShellSpeed;
            m_CollisionSize.x -= m_nTankShellSpeed;
            m_VertShellDestroyingVolume.x = m_CurPos.x + ( m_CurPos.w - m_VertShellDestroyingVolume.w ) / 2;
            m_VertShellDestroyingVolume.y = m_CurPos.y + ( m_CurPos.h - m_VertShellDestroyingVolume.h ) / 2;
            break;

        case CommonTanksProperties::MoveDirection::LEFT:
            m_CurPos.x        += m_nTankShellSpeed;
            m_CollisionSize.x += m_nTankShellSpeed;
            m_VertShellDestroyingVolume.x = m_CurPos.x + ( m_CurPos.w - m_VertShellDestroyingVolume.w ) / 2;
            m_VertShellDestroyingVolume.y = m_CurPos.y + ( m_CurPos.h - m_VertShellDestroyingVolume.h ) / 2;
            break;
        }
        m_ShellAnim.setPosition( m_CurPos.x, m_CurPos.y );
    }
    else if( m_ShellAnim.singleAnimation( 0 ) )
    {
        m_fExplosed = true;
    }
}

void TankShell::setPosition( int nX , int nY )
{
    m_ShellAnim.setPosition( nX, nY );
    recalcPosition();
}

SDL_Rect TankShell::getPosition() const
{
    return m_CurPos;
}

SDL_Rect TankShell::getCollisionPosition() const
{
    return m_CollisionSize;
}

SDL_Rect TankShell::getDestoyingVolume() const
{
    if( CommonTanksProperties::MoveDirection::UP == m_MoveDirection || CommonTanksProperties::MoveDirection::DOWN == m_MoveDirection )
    {
        return m_HorizShellDestroyingVolume;
    }
    else
    {
        return m_VertShellDestroyingVolume;
    }
}

CommonTanksProperties::TankOwnerIdentity TankShell::getOwnerIdentity() const
{
    return m_TankOwnerIdentity;
}

CommonTanksProperties::MoveDirection TankShell::getMoveDirection() const
{
    return m_MoveDirection;
}
