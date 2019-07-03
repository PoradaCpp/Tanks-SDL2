#include <algorithm>
#include <iostream>
#include <cmath>

#include "objectsmanagement.h"
#include "gameengine.h"
#include "tankofplayer.h"
#include "tankofenemy.h"
#include "tankshell.h"
#include "gamepage.h"

const RelativeRect ObjectsManagement::SCORES_TEXT_SIZE( 0, 0, 1.5, 2.5 );

ObjectsManagement::ObjectsManagement( GameEngine *pGameEngine, pSharedMap pMap, ObjectsManagementInitData InitData, Renderer renderer ):
    m_pGameEngine( pGameEngine ), m_BonusesTexture( InitData.m_BonusTextureInitData, renderer ), m_Renderer( renderer ),
    m_PlayersHeart( InitData.m_HeartInitData, renderer ), m_GameOverText( InitData.m_GameOverTextInitData, renderer ),
    m_pMap( pMap ), m_BonusAddAudio( InitData.m_sBonusAddAudioPath ), m_BonusTakenAudio( InitData.m_sBonusTakenAudioPath ),
    m_ScoresTextData( InitData.m_GameOverTextInitData ),
    m_ScoresCounting( InitData.m_BackgroundTextureInitData, InitData.m_ScoresFormTextInitDataVc,
                      InitData.m_AmountsTextInitDataVc, InitData.m_ScoresCountingAudioPath, renderer )
{
    m_ScoresTextData.m_nTopTextureShiftX = 1;
    m_ScoresTextData.m_nTopTextureShiftY = 1;
    m_ScoresTextData.TopTextInitData.m_DestRect    = SCORES_TEXT_SIZE;
    m_ScoresTextData.BottomTextInitData.m_DestRect = SCORES_TEXT_SIZE;
    m_ScoresTextData.TopTextInitData.m_BaseRect    = MapObject::TILES_BASE_RECT;
    m_ScoresTextData.BottomTextInitData.m_BaseRect = MapObject::TILES_BASE_RECT;
    m_ScoresTextData.TopTextInitData.m_TextSurfaceInitData.m_sText    = "0";
    m_ScoresTextData.BottomTextInitData.m_TextSurfaceInitData.m_sText = "0";
}

ObjectsManagement::~ObjectsManagement() {}

void ObjectsManagement::tanksManagement()
{
    std::for_each( m_TanksList.begin(), m_TanksList.end(), [this] ( TankAndTextPair &TankAndText )
    {
        if( m_fEnemyTankStop && CommonTanksProperties::TankOwnerIdentity::ENEMY == TankAndText.first->getOwnerIdentity() &&
            !TankAndText.first->hasNoLives() )
        {
            TankAndText.first->stopMove();
            tankStopProcessing();
        }
        else
        {
            TankAndText.first->changePosition();
            pSharedTankShell pShell = TankAndText.first->tankShot();

            if( pShell )
            {
                m_TankShellsList.push_back( pShell );
            }
        }

        if( m_pBonus && CommonTanksProperties::TankOwnerIdentity::ENEMY != TankAndText.first->getOwnerIdentity() &&
            TankAndText.first->checkTankCollision( m_pBonus->getDestination() ) )
        {
            bonusProcessing( TankAndText.first );
        }
    });

    m_TanksList.remove_if( [this] ( TankAndTextPair &TankAndText )
    {
        if( TankAndText.first->isDestroyed() )
        {
            m_pGameEngine->tanksDecrease( TankAndText.first->getOwnerIdentity() );
            m_pGameEngine->resetPlayersTankType( TankAndText.first->getOwnerIdentity() );
        }
        return TankAndText.first->isDestroyed();
    });
}

void ObjectsManagement::tankShellsManagement()
{
    std::for_each( m_TankShellsList.begin(), m_TankShellsList.end(), [this] ( pSharedTankShell &pShell )
    {
        pShell->changePosition();
        bool fItIsShell = true;
        SDL_Rect ShellRect = pShell->getCollisionPosition(),
                 HeartPos = m_PlayersHeart.getCollisionRect();

        if( !pShell->getExplosion() && !m_PlayersHeart.isDestroying() && SDL_HasIntersection( &ShellRect, &HeartPos ) )
        {
            m_PlayersHeart.destroy();
            pShell->setExplosion();
        }

        if( !pShell->getExplosion() && m_pMap->checkCollision( pShell->getCollisionPosition(), fItIsShell ) )
        {
            pShell->setExplosion();

            if( !m_pMap->checkBorderCollision( pShell->getCollisionPosition() ))
            {
                m_pMap->destroy( pShell->getCollisionPosition(), pShell->getDestoyingVolume(), pShell->getMoveDirection() );
            }
        }

        std::for_each( m_TanksList.begin(), m_TanksList.end(), [&pShell, this] ( TankAndTextPair &TankAndText )
        {
            if( !TankAndText.first->hasNoLives() && !pShell->getExplosion() &&
                TankAndText.first->checkTankShellCollision( pShell->getCollisionPosition(), pShell->getOwnerIdentity() ) )
            {
                if( TankAndText.first->hasNoLives() )
                {
                    scoresProcessing( TankAndText, pShell );
                }
                if( TankAndText.first->isBonus() )
                {
                    TankAndText.first->resetBonus();
                    createBonus();
                }
                pShell->setExplosion();
            }
        });

        std::for_each( m_TankShellsList.begin(), m_TankShellsList.end(), [&pShell] ( pSharedTankShell &pAnotherShell )
        {
            if( pShell->getOwnerIdentity() != pAnotherShell->getOwnerIdentity() && !pShell->getExplosion() &&
                !pAnotherShell->getExplosion() )
            {
                SDL_Rect ShellRect = pShell->getCollisionPosition();
                SDL_Rect AnotherShellRect = pAnotherShell->getCollisionPosition();

                if( SDL_HasIntersection( &ShellRect, &AnotherShellRect ))
                {
                    pShell->setExplosion();
                    pAnotherShell->setExplosion();
                }
            }
        });
    });

    m_TankShellsList.remove_if( [] ( pSharedTankShell &pShell )
    {
        return pShell->isExplosed();
    });
}

void ObjectsManagement::createBonus()
{
    m_pBonus = std::make_shared <Bonus> ( m_BonusesTexture, m_BonusAddAudio, m_BonusTakenAudio,
                                          static_cast <Bonus::BonusType> ( m_RandBonus() ) );
    SDL_Rect MapBorderRect = m_pMap->getMapBordersRect();
    SDL_Rect BonusBorderRect = m_pBonus->getDestination();
    int nX = MapBorderRect.x + ( ( MapBorderRect.w - BonusBorderRect.w ) * static_cast <int> ( m_Rand0_100() ) ) / 100;
    int nY = MapBorderRect.y + lround( MapBorderRect.h * BONUS_BIRTH_BORDER ) +
             lround( ( MapBorderRect.h * BONUS_BIRTH_RANGE * m_Rand0_100() ) / 100 );
    m_pBonus->setPosition( nX, nY );
}

void ObjectsManagement::init()
{
    m_PlayersHeart.init();
    m_ScoresCounting.setStageNumber( m_pMap->getStageNumber() );
    m_ScoresCounting.setHighScore( m_pGameEngine->m_nHighScore );
    m_ScoresCounting.resetScoresCounter();
    m_ScoresCounting.resetStageScores();
    m_nEnemyTankStopTime = 0;
    m_fEnemyTankStop     = false;
    m_nBaseDefenseOnTime = 0;
    m_fBaseDefenseOn     = false;
}

void ObjectsManagement::addTank( pSharedTank pTank )
{
    m_TanksList.push_back({ pTank, nullptr });
}

void ObjectsManagement::disableBonus()
{
    m_pBonus.reset();
}

bool ObjectsManagement::heartIsDestroyed() const
{
    return m_PlayersHeart.isDestroyed();
}

void ObjectsManagement::gameEndClear()
{
    m_pMap->clear();
    m_TanksList.clear();
    m_TankShellsList.clear();
    m_pBonus.reset();
}

void ObjectsManagement::destroyPlayersTanks()
{
    std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( TankAndTextPair &TankAndText )
    {
        if( CommonTanksProperties::TankOwnerIdentity::ENEMY != TankAndText.first->getOwnerIdentity() )
        {
            TankAndText.first->resetLives();
        }
    });
}

void ObjectsManagement::destroyEnemiesTanks()
{
    std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( TankAndTextPair &TankAndText )
    {
        if( CommonTanksProperties::TankOwnerIdentity::ENEMY == TankAndText.first->getOwnerIdentity() )
        {
            TankAndText.first->resetLives();
        }
    });
}

void ObjectsManagement::tankStopProcessing()
{
    if( !m_nEnemyTankStopTime )
    {
        m_nEnemyTankStopTime = SDL_GetTicks();
    }
    else if( SDL_GetTicks() - m_nEnemyTankStopTime >= ENEMY_TANK_STOP_TIME )
    {
        m_nEnemyTankStopTime = 0;
        m_fEnemyTankStop = false;
    }
}

void ObjectsManagement::tankUpgrade( pSharedTank pTank )
{
    CommonTanksProperties::TankOwnerIdentity tankOwner = pTank->getOwnerIdentity();

    switch( pTank->getTankType() )
    {
    case CommonTanksProperties::TankType::ORDINARY_TANK:
        pTank->attachProperties( std::make_shared <PlayersTankProperties> ( tankOwner, CommonTanksProperties::TankType::BTR ));
        m_pGameEngine->setPlayersTankType( tankOwner, CommonTanksProperties::TankType::BTR );
        break;
    case CommonTanksProperties::TankType::BTR:
        pTank->attachProperties( std::make_shared <PlayersTankProperties> ( tankOwner,
                                                                            CommonTanksProperties::TankType::QUICK_SHOOTING_BTR ));
        m_pGameEngine->setPlayersTankType( tankOwner, CommonTanksProperties::TankType::QUICK_SHOOTING_BTR );
        break;
    case CommonTanksProperties::TankType::QUICK_SHOOTING_BTR:
        pTank->attachProperties( std::make_shared <PlayersTankProperties> ( tankOwner, CommonTanksProperties::TankType::HEAVY_TANK ));
        m_pGameEngine->setPlayersTankType( tankOwner, CommonTanksProperties::TankType::HEAVY_TANK );
        break;
    case CommonTanksProperties::TankType::HEAVY_TANK:
        break;
    }
}

void ObjectsManagement::playersBaseDefense()
{
    if( !m_nBaseDefenseOnTime )
    {
        m_pMap->setBaseGraniteDefense();
        m_nBaseDefenseOnTime = SDL_GetTicks();
    }
    else if( SDL_GetTicks() - m_nBaseDefenseOnTime >= BASE_PROTECTION_ON_TIME )
    {
        m_pMap->resetBaseGraniteDefense();
        m_nBaseDefenseOnTime = 0;
        m_fBaseDefenseOn = false;
    }
}

void ObjectsManagement::scoresProcessing( TankAndTextPair &TankAndText, const pSharedTankShell &pShell )
{
    if( CommonTanksProperties::TankOwnerIdentity::ENEMY == TankAndText.first->getOwnerIdentity() )
    {
        scoresTextCreating( TankAndText );
        m_ScoresCounting.scoresProcessing( TankAndText.first, pShell );
    }
}

void ObjectsManagement::scoresTextCreating( TankAndTextPair &TankAndText )
{
    SDL_Rect TankPos = TankAndText.first->getFullObjectPosition();

    TankAndText.second = std::make_shared <Text3D>( m_ScoresTextData, m_Renderer );
    TankAndText.second->setPosition( TankPos.x, TankPos.y );
    TankAndText.second->setText( std::to_string( TankAndText.first->getDestroyingScores() ) );
}

void ObjectsManagement::bonusScoresTextCreating()
{
    if( !m_nBonusScoresShowTime && m_pBonus )
    {
        m_nBonusScoresShowTime = SDL_GetTicks();
        SDL_Rect BonusPos = m_pBonus->getDestination();

        m_pBonusScoresText = std::make_shared <Text3D>( m_ScoresTextData, m_Renderer );
        m_pBonusScoresText->setPosition( BonusPos.x, BonusPos.y );
        m_pBonusScoresText->setText( BONUS_SCORES_TEXT );
    }
    else if( SDL_GetTicks() - m_nBonusScoresShowTime >= BONUS_SCORES_SHOW_TIME )
    {
        m_pBonusScoresText.reset();
        m_nBonusScoresShowTime = 0;
        m_fBonusScoresShow = false;
    }
}

void ObjectsManagement::resetScores()
{
    m_ScoresCounting.resetScores();
}

void ObjectsManagement::gameOverTextRender()
{
    m_GameOverText.render();
}

void ObjectsManagement::finalScoresCounting()
{
    m_ScoresCounting.scoresCounting();
}

void ObjectsManagement::finalScoresRender()
{
    m_ScoresCounting.render();
}

uint32_t ObjectsManagement::getCurrentScore() const
{
    return m_ScoresCounting.getCurrentScore();
}

bool ObjectsManagement::checkTankCollision( SDL_Rect &checkingRect ) const
{
    for( const TankAndTextPair &TankAndText: m_TanksList )
    {
        if( TankAndText.first->checkTankCollision( checkingRect ) )
        {
            return true;
        }
    }
    return false;
}

bool ObjectsManagement::checkHeartCollision( SDL_Rect &checkingRect ) const
{
    SDL_Rect HeartRect = m_PlayersHeart.getCollisionRect();

    return ( SDL_HasIntersection( &checkingRect, &HeartRect ) && !m_PlayersHeart.isDestroyed() );
}

void ObjectsManagement::mainProcess()
{
    if( m_fBaseDefenseOn )
    {
        playersBaseDefense();
    }
    if( m_fBonusScoresShow )
    {
        bonusScoresTextCreating();
    }
    tanksManagement();
    tankShellsManagement();
}

void ObjectsManagement::bonusProcessing( pSharedTank pTank )
{
    if( m_pBonus )
    {
        switch( m_pBonus->getBonusType() )
        {
        case Bonus::BonusType::DEFENCE:
            pTank->setDefence();
            break;
        case Bonus::BonusType::EXTRA_LIFE:
            m_pGameEngine->livesIncrease( pTank->getOwnerIdentity() );
            break;
        case Bonus::BonusType::TANK_UPGRADE:
            tankUpgrade( pTank );
            break;
        case Bonus::BonusType::FORTIFICATION:
            m_fBaseDefenseOn = true;
            break;
        case Bonus::BonusType::ENEMY_EXPLOSING:
            destroyEnemiesTanks();
            break;
        case Bonus::BonusType::ENEMY_TEMP_STOP:
            m_fEnemyTankStop = true;
            break;
        }
        bonusScoresTextCreating();
        m_ScoresCounting.addToScores( pTank->getOwnerIdentity(), BONUS_SCORES );
        m_fBonusScoresShow = true;
        m_pBonus->playTakingSound();
        m_pBonus.reset();
    }
}

void ObjectsManagement::resize()
{
    std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( TankAndTextPair &TankAndText )
    {
        TankAndText.first->changeSize();

        if( TankAndText.second )
        {
            TankAndText.second->changeSize();
        }
    });

    std::for_each( m_TankShellsList.begin(), m_TankShellsList.end(), [] ( pSharedTankShell &pShell )
    {
        pShell->changeSize();
    });

    m_PlayersHeart.resize();
    m_GameOverText.changeSize();
    m_ScoresCounting.changeSize();

    if( m_pBonus )
    {
        m_pBonus->changeSize();
    }
    if( m_pBonusScoresText )
    {
        m_pBonusScoresText->changeSize();
    }
}

void ObjectsManagement::render()
{
    std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( TankAndTextPair &TankAndText )
    {
        TankAndText.first->render();

        if( TankAndText.second )
        {
            TankAndText.second->render();
        }
    });

    std::for_each( m_TankShellsList.begin(), m_TankShellsList.end(), [] ( pSharedTankShell &pShell )
    {
        pShell->render();
    });

    m_PlayersHeart.render();

    if( m_pBonus )
    {
        m_pBonus->render();
    }
    if( m_pBonusScoresText )
    {
        m_pBonusScoresText->render();
    }
}

void ObjectsManagement::setPause()
{
    std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( TankAndTextPair &TankAndText )
    {
        TankAndText.first->stopMove();
    });
}
