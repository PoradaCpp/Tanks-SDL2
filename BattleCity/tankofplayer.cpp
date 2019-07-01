#include <algorithm>

#include "tankofplayer.h"
#include "tankshell.h"

TankOfPlayer::TankOfPlayer( Animation TankMoveAnim, Animation TankExplosionAnim, Animation BombExplosionAnim,
                            std::vector<AudioChunk> audioVc, pSharedMap pMap, RelativeRect RelativeBirthPos,
                            std::shared_ptr<ObjectsManagement> pObjectManagement, CommonTanksProperties::MoveDirection moveDirection ):
    Tank( TankMoveAnim, TankExplosionAnim, BombExplosionAnim, audioVc, pMap, RelativeBirthPos, pObjectManagement, moveDirection ),
    m_TankDefenseAnim( TankMoveAnim ) {}

TankOfPlayer::~TankOfPlayer()
{
    m_AudioVc.at( static_cast <size_t> ( TankProperties::Sounds::MOVE ) ).stop();
    m_AudioVc.at( static_cast <size_t> ( TankProperties::Sounds::ENGINE_WORK ) ).stop();
}

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
        else if( SDL_GetTicks() - m_nDefenseTime <= DEFENSE_ON_TIME && m_pProperties->m_nNumberOfLives)
        {
            m_TankDefenseAnim.startAnimation( m_pProperties->m_nDefenseAnimTilesBegin, m_pProperties->m_nDefenseAnimTilesEnd );
            m_TankDefenseAnim.render( static_cast <double> ( m_MoveDirection ) );
        }
        else
        {
            m_nDefenseTime = 0;
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
    m_AudioVc.at( static_cast <size_t> ( TankProperties::Sounds::ENGINE_WORK ) ).play( AudioChunk::INFINITELY_PLAYING );
}

void TankOfPlayer::changePosition()
{
    Tank::changePosition();
    m_TankDefenseAnim.setPosition( m_RealSizeRectHoriz.x, m_RealSizeRectHoriz.y );
}

bool TankOfPlayer::shotCondition()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );

    return currentKeyStates[ m_pProperties->Shot ];
}
