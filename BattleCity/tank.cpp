#include <cmath>
#include <algorithm>

#include "tank.h"
#include "gameengine.h"
#include "tankshell.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Tank::COLLISION_INFO
/// \brief Object with data about real or "solid" part of TAnk
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Tank::Tank( Animation TankMoveAnim, Animation TankExplosionAnim, Animation ShellExplosionAnim, pSharedMap pMap,
            RelativeRect RelativeBirthPos, GameEngine *pGameEngine, MoveDirection moveDirection ): DisplayedObject(),
    m_TankMoveAnim( TankMoveAnim ), m_TankExplosionAnim( TankExplosionAnim ), m_ShellExplosionAnim( ShellExplosionAnim ),
    m_pProperties( nullptr), m_pMap( pMap), m_AnimCurPos({ 0, 0, 0, 0 }), m_pRealCurPos( nullptr ),
    m_RealSizeRectHoriz({ 0, 0, 0, 0 }), m_RealSizeRectVert({ 0, 0, 0, 0 }), m_TankCenter({ 0, 0 }), m_ShellPosition({ 0, 0 }),
    m_nAnimBegin( 0 ), m_nAnimEnd( 0 ), m_MoveDirection( moveDirection ), m_fMove( false ), m_fBirth( true ), m_fDestroyed( false ),
    m_nDestroyingTime( 0 ), m_pGameEngine( pGameEngine )
{
    m_TankMoveAnim.setRelativeDestination( RelativeBirthPos );
    m_AnimCurPos = m_TankMoveAnim.getDestination();
}

Tank::~Tank() {}

void Tank::countRealTankSize()
{
    m_RealSizeRectHoriz = { 0, 0, lround( m_AnimCurPos.w * m_pProperties->m_RelativeTankSize.m_dWidth / 100 ),
                             lround( m_AnimCurPos.h * m_pProperties->m_RelativeTankSize.m_dHeight / 100 )};
    m_RealSizeRectHoriz.x = m_AnimCurPos.x + ( m_AnimCurPos.w - m_RealSizeRectHoriz.w ) / 2;
    m_RealSizeRectHoriz.y = m_AnimCurPos.y + ( m_AnimCurPos.h - m_RealSizeRectHoriz.h ) / 2;

    m_RealSizeRectVert.w = m_RealSizeRectHoriz.h;
    m_RealSizeRectVert.h = m_RealSizeRectHoriz.w;
    m_RealSizeRectVert.x = m_RealSizeRectHoriz.x + ( m_RealSizeRectHoriz.w - m_RealSizeRectVert.w ) / 2;
    m_RealSizeRectVert.y = m_RealSizeRectHoriz.y + ( m_RealSizeRectHoriz.h - m_RealSizeRectVert.h ) / 2;

    if( MoveDirection::UP == m_MoveDirection || MoveDirection::DOWN == m_MoveDirection )
    {
        m_pRealCurPos = &m_RealSizeRectVert;
    }
    else
    {
        m_pRealCurPos = &m_RealSizeRectHoriz;
    }
    m_TankCenter.x = m_pRealCurPos->x + m_pRealCurPos->w / 2;
    m_TankCenter.y = m_pRealCurPos->y + m_pRealCurPos->h / 2;
}

Tank::MoveDirection Tank::determineDirection( SDL_Rect &NewPos )
{
    MoveDirection NewDirection( m_MoveDirection );

    if( NewPos.x != m_pRealCurPos->x || NewPos.y != m_pRealCurPos->y )
    {
        if(m_pRealCurPos->x > NewPos.x )
        {
            NewDirection = MoveDirection::RIGHT;
        }
        else if( m_pRealCurPos->x < NewPos.x )
        {
            NewDirection = MoveDirection::LEFT;
        }
        else if( m_pRealCurPos->y > NewPos.y )
        {
            NewDirection = MoveDirection::UP;;
        }
        else if( m_pRealCurPos->y < NewPos.y )
        {
            NewDirection = MoveDirection::DOWN;
        }
    }
    return NewDirection;
}

void Tank::allowNewPosition( SDL_Rect &NewPos )
{
    if( MoveDirection::RIGHT == m_MoveDirection || MoveDirection::LEFT == m_MoveDirection )
    {
        int nDx = NewPos.x - m_RealSizeRectHoriz.x;
        int nDy = NewPos.y - m_RealSizeRectHoriz.y;
        m_AnimCurPos.x += nDx;
        m_AnimCurPos.y += nDy;
        m_RealSizeRectVert.x += nDx;
        m_RealSizeRectVert.y += nDy;
        m_RealSizeRectHoriz = NewPos;
        m_pRealCurPos = &m_RealSizeRectHoriz;
    }
    else
    {
        int nDx = NewPos.x - m_RealSizeRectVert.x;
        int nDy = NewPos.y - m_RealSizeRectVert.y;
        m_AnimCurPos.x += nDx;
        m_AnimCurPos.y += nDy;
        m_RealSizeRectHoriz.x += nDx;
        m_RealSizeRectHoriz.y += nDy;
        m_RealSizeRectVert = NewPos;
        m_pRealCurPos = &m_RealSizeRectVert;
    }
    m_TankCenter.x = m_pRealCurPos->x + m_pRealCurPos->w / 2;
    m_TankCenter.y = m_pRealCurPos->y + m_pRealCurPos->h / 2;
}

SDL_Rect Tank::calcPossiblePos( SDL_Rect &NewPos, MoveDirection NewDirection )
{
    SDL_Rect PossiblePos;

    if( MoveDirection::RIGHT == NewDirection || MoveDirection::LEFT == NewDirection )
    {
        int nDx = NewPos.x - m_pRealCurPos->x;
        PossiblePos = m_RealSizeRectHoriz;
        PossiblePos.x += nDx;
    }
    else
    {
        int nDy = NewPos.y - m_pRealCurPos->y;
        PossiblePos = m_RealSizeRectVert;
        PossiblePos.y += nDy;
    }
    return PossiblePos;
}

bool Tank::alignmentAfterTurn( SDL_Rect &PossiblePos )
{
    SDL_Rect PossiblePos2 = PossiblePos;
    int nEndPos = abs( lround( 1.1 * PossiblePos.w - PossiblePos.h ) );
    int nBeginX = PossiblePos.x;
    int nBeginY = PossiblePos.y;
    bool fEnd( false );

    for( int i = 1; i <= nEndPos && !fEnd; ++i )
    {
        for( int j = -i; j <= i && !fEnd; j += i * 2 )
        {
            PossiblePos.x = nBeginX;
            PossiblePos.x += j;
            PossiblePos2.y = nBeginY;
            PossiblePos2.y += j;

            for( int k = - i ; k <= i && !fEnd; ++k )
            {
                PossiblePos.y = nBeginY;
                PossiblePos.y += k;
                PossiblePos2.x = nBeginX;
                PossiblePos2.x += k;

                if( !m_pMap->checkCollision( PossiblePos ) && !checkTankCollision( PossiblePos ) )
                {
                    fEnd = true;
                    break;
                }
                if( !m_pMap->checkCollision( PossiblePos2 ) && !checkTankCollision( PossiblePos2 ) )
                {
                    fEnd = true;
                    PossiblePos = PossiblePos2;
                    break;
                }
            }
        }
    }
    return fEnd;
}

bool Tank::alignmentAfterResize (SDL_Rect &PossiblePos)
{
    SDL_Rect PossiblePos2 = PossiblePos;
    int nEndPos = ( PossiblePos.w > PossiblePos.h ? PossiblePos.w : PossiblePos.h );
    int nBeginX = PossiblePos.x;
    int nBeginY = PossiblePos.y;
    bool fEnd( false );

    for( int i = 1; i <= nEndPos && !fEnd; ++i )
    {
        for( int j = -i; j <= i && !fEnd; j += i * 2 )
        {
            PossiblePos.x = nBeginX;
            PossiblePos.x += j;
            PossiblePos2.y = nBeginY;
            PossiblePos2.y += j;

            for( int k = - i ; k <= i && !fEnd; ++k )
            {
                PossiblePos.y = nBeginY;
                PossiblePos.y += k;
                PossiblePos2.x = nBeginX;
                PossiblePos2.x += k;

                if( !m_pMap->checkCollision( PossiblePos ) && !checkTankCollision( PossiblePos ) )
                {
                    fEnd = true;
                    break;
                }
                if( !m_pMap->checkCollision( PossiblePos2 ) && !checkTankCollision( PossiblePos2 ) )
                {
                    fEnd = true;
                    PossiblePos = PossiblePos2;
                    break;
                }
            }
        }
    }
    return fEnd;
}

bool Tank::checkTankCollision()
{
    for( size_t i = 0, nVectorSize = m_pGameEngine->m_TanksVc.size(); i < nVectorSize; ++i )
    {
        if( m_pGameEngine->m_TanksVc.at(i).get() != this &&
            SDL_HasIntersection( m_pRealCurPos,m_pGameEngine->m_TanksVc.at(i)->m_pRealCurPos ))
        {
            return true;
        }
    }
    return false;
}

bool Tank::checkTankCollision( const SDL_Rect &Rect )
{
    for( size_t i = 0, nVectorSize = m_pGameEngine->m_TanksVc.size(); i < nVectorSize; ++i )
    {
        if( m_pGameEngine->m_TanksVc.at(i).get() != this &&
            SDL_HasIntersection( &Rect, m_pGameEngine->m_TanksVc.at(i)->m_pRealCurPos ))
        {
            return true;
        }
    }
    return false;
}

bool Tank::checkNewPosition( SDL_Rect &NewPos )
{
    m_fMove = true;
    bool fLooseMove = false;
    m_TankMoveAnim.startAnimation( m_nAnimBegin, m_nAnimEnd );

    MoveDirection NewDirection = determineDirection( NewPos );
    SDL_Rect PossiblePos = calcPossiblePos( NewPos, NewDirection );

    if( NewDirection != m_MoveDirection )
    {
        m_MoveDirection = NewDirection;

        if( m_pMap->checkCollision( PossiblePos ) || checkTankCollision( PossiblePos ) )
        {
            if( alignmentAfterTurn( PossiblePos ) )
            {
                allowNewPosition( PossiblePos );
                fLooseMove = true;
            }
        }
    }
    else if( !m_pMap->checkCollision( PossiblePos ) && !checkTankCollision( PossiblePos ) )
    {
        allowNewPosition( PossiblePos );
        fLooseMove = true;
    }
    return fLooseMove;
}

bool Tank::newPositionProcessing()
{
    bool fLooseMove = false;
    SDL_Rect NewPos = *m_pRealCurPos;
    m_pProperties->m_pMoveAlgorithm->move( NewPos );

    if( m_fBirth )
    {
        if( m_TankMoveAnim.singleAnimation( m_pProperties->m_nBirthAnimTilesBegin, m_pProperties->m_nBirthAnimTilesEnd ) )
        {
            m_fBirth = false;
            m_TankMoveAnim.setAnimFrames( m_nAnimBegin, m_nAnimEnd );
        }
    }
    else if( !SDL_RectEquals( m_pRealCurPos, &NewPos ))
    {
        if( checkNewPosition( NewPos ) )
        {
            fLooseMove = true;
            m_TankMoveAnim.setPosition( m_AnimCurPos.x, m_AnimCurPos.y );
            calcShellPosition();
        }
    }
    else
    {
        m_fMove = false;
        m_TankMoveAnim.stopAnimation();
    }
    return fLooseMove;
}

void Tank::calcShellPosition()
{
    m_ShellPosition = m_TankCenter;

    switch( m_MoveDirection )
    {
    case MoveDirection::UP:
        m_ShellPosition.y -= m_AnimCurPos.h * ( CommonTanksProperties::TANK_SHELL_SCALE -
                                                m_pProperties->m_RelativeTankShellPos.m_dX ) / 100;
        m_ShellPosition.x -= m_AnimCurPos.w * ( m_pProperties->m_RelativeTankShellPos.m_dY ) / 100;
        break;

    case MoveDirection::DOWN:
        m_ShellPosition.y -= m_AnimCurPos.h * m_pProperties->m_RelativeTankShellPos.m_dX / 100;
        m_ShellPosition.x -= m_AnimCurPos.w * ( m_pProperties->m_RelativeTankShellPos.m_dY ) / 100;
        break;

    case MoveDirection::RIGHT:
        m_ShellPosition.y -= m_AnimCurPos.h * ( m_pProperties->m_RelativeTankShellPos.m_dY ) / 100;
        m_ShellPosition.x -= m_AnimCurPos.w * ( CommonTanksProperties::TANK_SHELL_SCALE -
                                                m_pProperties->m_RelativeTankShellPos.m_dX ) / 100;
        break;

    case MoveDirection::LEFT:
        m_ShellPosition.y -= m_AnimCurPos.h * ( m_pProperties->m_RelativeTankShellPos.m_dY ) / 100;
        m_ShellPosition.x -= m_AnimCurPos.w * m_pProperties->m_RelativeTankShellPos.m_dX / 100;

        break;
    }
}

void Tank::tankExplosing()
{
    if( !m_nDestroyingTime )
    {
        m_nDestroyingTime = SDL_GetTicks();

        SDL_Rect ExplosionRect = m_AnimCurPos;
        ExplosionRect.w = lround( m_AnimCurPos.w * 100.0 / CommonTanksProperties::SHELL_SIZE_SCALE );
        ExplosionRect.h = lround( m_AnimCurPos.h * 100.0 / CommonTanksProperties::SHELL_SIZE_SCALE );
        ExplosionRect.x = m_AnimCurPos.x + ( m_AnimCurPos.w - ExplosionRect.w ) / 2;
        ExplosionRect.y = m_AnimCurPos.y + ( m_AnimCurPos.h - ExplosionRect.h ) / 2;

        m_TankExplosionAnim.setDestination( ExplosionRect );
        m_TankMoveAnim.stopAnimation();
    }
    if( m_TankExplosionAnim.singleAnimation() )
    {
        m_fDestroyed = true;
    }
}

void Tank::changeSize()
{
    m_TankMoveAnim.changeSize();
    SDL_Rect NewPos = m_TankMoveAnim.getDestination();

    if( alignmentAfterResize( NewPos ) )
    {
        m_AnimCurPos = NewPos;
    }

    countRealTankSize();
    m_pProperties->m_pMoveAlgorithm->setDisplayWidth( static_cast <uint32_t> ( m_TankMoveAnim.getPageWidth() ) );
}

void Tank::render()
{
    if( m_pProperties->m_nNumberOfLives || SDL_GetTicks() <= m_nDestroyingTime + CommonTanksProperties::DESTROYING_PAUSE )
    {
        m_TankMoveAnim.render( static_cast <double> ( m_MoveDirection ) );
    }

    if( !m_pProperties->m_nNumberOfLives )
    {
        m_TankExplosionAnim.render();
    }
    //SDL_SetRenderDrawColor( m_pMap->m_Renderer, Map::GREY.r, Map::GREY.g, Map::GREY.b, Map::GREY.a );
    //SDL_RenderDrawRect( m_pMap->m_Renderer, m_pRealCurPos );
}

void Tank::attachProperties( pSharedTankProperties pProperties )
{
    m_pProperties = pProperties;
    countRealTankSize();
    m_pProperties->m_pMoveAlgorithm->setDisplayWidth( static_cast <uint32_t> ( m_TankMoveAnim.getPageWidth() ) );
    RelativeRect RelativePos = m_TankMoveAnim.getRelativeDestination();
    m_pProperties->m_RelativeTankShellPos.m_dWidth  *= RelativePos.m_dWidth  / 100;
    m_pProperties->m_RelativeTankShellPos.m_dHeight *= RelativePos.m_dHeight / 100;
    calcShellPosition();
    m_nAnimBegin = m_pProperties->m_nAnimTilesBegin;
    m_nAnimEnd = m_pProperties->m_nAnimTilesEnd;
}

void Tank::changePosition()
{
    if( m_pProperties->m_nNumberOfLives )
    {
        newPositionProcessing();
    }
    else
    {
        tankExplosing();
    }
}

pSharedTankShell Tank::tankShot()
{
    if( m_pProperties->m_nNumberOfLives )
    {
        if( !m_nShotTime )
        {
            m_nShotTime = SDL_GetTicks();
        }
        else if( SDL_GetTicks() - m_nShotTime >= ( m_pProperties->m_nShootPause ) )
        {
            m_nShotTime = 0;
            auto Shell = std::make_shared <TankShell> ( m_ShellExplosionAnim, m_pProperties->m_RelativeTankShellPos, m_MoveDirection,
                                                        m_pProperties->m_TankOwnerIdentity,
                                                        m_pProperties->m_dRelativeXDestroyingForce );

            Shell->setPosition( m_ShellPosition.x, m_ShellPosition.y );

            return Shell;
        }
    }

    return pSharedTankShell();
}

bool Tank::isNoLives()
{
    return 0 == m_pProperties->m_nNumberOfLives;
}

bool Tank::isDestroyed()
{
    return m_fDestroyed;
}

bool Tank::checkTankShellCollision( const SDL_Rect &Rect, CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity )
{
    if( ( SDL_HasIntersection( &Rect, m_pRealCurPos ) ) && (( m_pProperties->m_TankOwnerIdentity != tankOwnerIdentity ) &&
          (( CommonTanksProperties::TankOwnerIdentity::ENEMY == tankOwnerIdentity ) ||
           ( CommonTanksProperties::TankOwnerIdentity::ENEMY == m_pProperties->m_TankOwnerIdentity )) ) )
    {
        if( m_pProperties->m_fBonus )
        {
            m_pProperties->m_fBonus = false;
        }
        else if( m_pProperties->m_nNumberOfLives )
        {
            --m_pProperties->m_nNumberOfLives;
        }
        return true;
    }
    return false;
}

CommonTanksProperties::TankOwnerIdentity Tank::getOwnerIdentity() const
{
    return m_pProperties->m_TankOwnerIdentity;
}

void Tank::setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect )
{
    m_TankMoveAnim.setRelativeDestination( RelativeDestRect, RelativeBaseRect );
}

