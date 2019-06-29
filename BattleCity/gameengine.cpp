#include <algorithm>
#include <iostream>
#include <cmath>

#include "gameengine.h"
#include "tankofplayer.h"
#include "tankofenemy.h"
#include "tankshell.h"
#include "gamepage.h"

const RelativeRect GameEngine::PLAYER1_BIRTH_POS = { 31, 85,  5.7, 10 };
const RelativeRect GameEngine::PLAYER2_BIRTH_POS = { 47, 85,  5.7, 10 };
const RelativeRect GameEngine::ENEMY1_BIRTH_POS  = { 2,  2.5, 5.7, 10 };
const RelativeRect GameEngine::ENEMY2_BIRTH_POS  = { 27, 2.5, 5.7, 10 };
const RelativeRect GameEngine::ENEMY3_BIRTH_POS  = { 51, 2.5, 5.7, 10 };
const RelativeRect GameEngine::ENEMY4_BIRTH_POS  = { 76, 2.5, 5.7, 10 };


const std::array <const RelativeRect*, 4> GameEngine::ENEMY_BIRTH_PLACE_ARR{ &ENEMY1_BIRTH_POS, &ENEMY2_BIRTH_POS,
                                                                             &ENEMY3_BIRTH_POS, &ENEMY4_BIRTH_POS };

GameEngine::GameEngine( std::vector <AnimationInitData> AnimInitDataVc, std::vector <ImgTextureInitData> LivesAndEnemiesInitDataVc,
                        Text3DInitData GameOverTextInitData, AnimationInitData HeartAnimInitData,
                        ImgTextureInitData BonusTextureInitData, std::string sAudioChunkPath, Renderer renderer, pSharedMap pMap ):
    m_GameOverText( GameOverTextInitData, renderer ), m_BonusesTexture( BonusTextureInitData, renderer ),
    m_PlayersHeart( HeartAnimInitData, sAudioChunkPath, renderer ), m_Renderer( renderer ), m_pMap( pMap )
{
    m_AnimTextureVc.reserve( AnimInitDataVc.size() );
    std::for_each( AnimInitDataVc.begin(), AnimInitDataVc.end(), [this, renderer] ( AnimationInitData InitData )
    {
        m_AnimTextureVc.emplace_back( InitData, renderer );
    });

    m_LivesAndEnemiesTextureVc.reserve( LivesAndEnemiesInitDataVc.size() + 1 );
    std::for_each( LivesAndEnemiesInitDataVc.begin(), LivesAndEnemiesInitDataVc.end(), [this, renderer] ( ImgTextureInitData InitData )
    {
        m_LivesAndEnemiesTextureVc.emplace_back( InitData, renderer );
    });

    m_PlayersAudioVc.reserve( CommonTanksProperties::PLAYERS_TANK_SOUNDS_PATHS.size() );

    for( std::string sAudioChunkPath: CommonTanksProperties::PLAYERS_TANK_SOUNDS_PATHS )
    {
        m_PlayersAudioVc.emplace_back( sAudioChunkPath );
    }

    m_EnemyAudioVc.reserve( CommonTanksProperties::ENEMY_TANK_SOUNDS_PATHS.size() );

    for( std::string sAudioChunkPath: CommonTanksProperties::ENEMY_TANK_SOUNDS_PATHS )
    {
        m_EnemyAudioVc.emplace_back( sAudioChunkPath );
    }

    m_EnemyImagesPlaceVc.resize  ( MAX_NUM_OF_ENEMIES,  { 0, 0, 0, 0 });
    m_Player1ImagesPlaceVc.resize( START_PLAYERS_LIVES, { 0, 0, 0, 0 });
    m_Player2ImagesPlaceVc.resize( START_PLAYERS_LIVES, { 0, 0, 0, 0 });
    calcLiveRectSize();
}

GameEngine::~GameEngine() {}

SDL_Rect GameEngine::RelativeToBase( const RelativeRect &relativeRect )
{
    int nPageWidth( 0 ), nPageHeight( 0 );
    RelativeRect m_RelativeBaseRect;
    SDL_Rect BaseRect = m_pMap->getMapBordersRect();
    SDL_Rect DestRect;

    if( !m_AnimTextureVc.empty() )
    {
        nPageWidth = m_AnimTextureVc.front().getPageWidth();
        nPageHeight = m_AnimTextureVc.front().getPageHeight();
        m_RelativeBaseRect = m_AnimTextureVc.front().getRelativeBase();
    }

    RelativeRect RelativeDestRect = relativeRect;

    RelativeRect RelativeBaseRect( ( BaseRect.x * 100.0 ) / nPageWidth, ( BaseRect.y * 100.0 ) / nPageHeight,
                                   ( BaseRect.w * 100.0 ) / nPageWidth, ( BaseRect.h * 100.0 ) / nPageHeight );

    double dXScale  = RelativeBaseRect.m_dWidth  / m_RelativeBaseRect.m_dWidth;
    double dYScale  = RelativeBaseRect.m_dHeight / m_RelativeBaseRect.m_dHeight;
    double dXCenter = RelativeBaseRect.m_dX      + RelativeBaseRect.m_dWidth / 2;
    double dYCenter = RelativeBaseRect.m_dY      + RelativeBaseRect.m_dHeight / 2;

    RelativeDestRect.m_dWidth *= dXScale;
    RelativeDestRect.m_dHeight *= dYScale;
    RelativeDestRect.m_dX = dXCenter - ( dXCenter - RelativeDestRect.m_dX ) * dXScale;
    RelativeDestRect.m_dY = dYCenter - ( dYCenter - RelativeDestRect.m_dY ) * dYScale;

    DestRect.h = lround( ( RelativeDestRect.m_dHeight * nPageHeight ) / 100 );
    DestRect.w = lround( ( RelativeDestRect.m_dWidth  * nPageWidth  ) / 100 );

    DestRect.x = lround( (( RelativeDestRect.m_dX + ( RelativeDestRect.m_dWidth  - ( DestRect.w * 100.0 ) / nPageWidth  )
                            / 2 ) * nPageWidth ) / 100 );
    DestRect.y = lround( (( RelativeDestRect.m_dY + ( RelativeDestRect.m_dHeight - ( DestRect.h * 100.0 ) / nPageHeight )
                            / 2 ) * nPageHeight ) / 100 );

    return DestRect;
}

void GameEngine::calcLiveRectSize()
{
    calcLiveRectSize( m_Player1ImagesPlaceVc, TexturePurpose::TANKS_OF_PLAYER1_LIFE );
    calcLiveRectSize( m_Player2ImagesPlaceVc, TexturePurpose::TANKS_OF_PLAYER2_LIFE );
    calcLiveRectSize( m_EnemyImagesPlaceVc, TexturePurpose::REMAIN_ENEMY_IMAGE );
}

void GameEngine::calcLiveRectSize( std::vector <SDL_Rect> &nImagePlaceVc, TexturePurpose texturePurpose )
{
    SDL_Rect rect = m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( texturePurpose ) ).getDestination();

    SDL_Rect UpperLeftRect = m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( TexturePurpose::REMAIN_ENEMY_IMAGE  ) ).getDestination();
    SDL_Rect DownRightRect = m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( TexturePurpose::CREATED_ENEMY_IMAGE ) ).getDestination();

    int nDx = lround( ( DownRightRect.x - UpperLeftRect.x ) / LIVES_X_COLUMNS_DIV );
    int nDy = lround( ( DownRightRect.y - UpperLeftRect.y ) / LIVES_Y_ROWS_DIV );
    int nRow = 0, nColumn = 0, nTemp = 0;

    for( uint32_t i = 0, nArrSize = nImagePlaceVc.size(); i < nArrSize; ++i )
    {
        nImagePlaceVc.at( i ) = rect;
        nTemp = i / LIVES_COLUMNS_NUMBER;

        if( nTemp != nRow )
        {
            nRow = nTemp;
            nColumn = 0;
        }
        nImagePlaceVc.at( i ).x += nColumn * nDx;
        nImagePlaceVc.at( i ).y += nRow * nDy;
        ++nColumn;
    }
}

void GameEngine::livesAndEnemies()
{
    for( uint32_t i = 0, nArrSize = m_EnemyImagesPlaceVc.size(); i < nArrSize; ++i )
    {
        if( i < MAX_NUM_OF_ENEMIES - m_nNumOfEnemies )
        {
            m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( TexturePurpose::CREATED_ENEMY_IMAGE ) ).render( nullptr,
                                                                                                  &m_EnemyImagesPlaceVc.at(i) );
        }
        else if( i < MAX_NUM_OF_ENEMIES - m_nNumOfEnemies + m_nNumOfEnemiesAlive  )
        {
            m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( TexturePurpose::REMAIN_ENEMY_IMAGE ) ).render( nullptr,
                                                                                                 &m_EnemyImagesPlaceVc.at(i) );
        }
        else
        {
            break;
        }
    }

    for( uint32_t i = 0, nArrSize = m_Player1ImagesPlaceVc.size(); i < nArrSize; ++i )
    {
        if( i < m_nPlayer1Lives )
        {
            m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( TexturePurpose::TANKS_OF_PLAYER1_LIFE )).render( nullptr,
                                                                                                   &m_Player1ImagesPlaceVc.at(i) );
        }
        else
        {
            break;
        }
    }

    for( uint32_t i = 0, nArrSize = m_Player2ImagesPlaceVc.size(); i < nArrSize; ++i )
    {
        if( i < m_nPlayer2Lives )
        {
            m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( TexturePurpose::TANKS_OF_PLAYER2_LIFE )).render( nullptr,
                                                                                                   &m_Player2ImagesPlaceVc.at(i) );
        }
        else
        {
            break;
        }
    }
}

void GameEngine::calcBirthPlacesRectSize()
{
    m_EnemyBirthPlaceVc.clear();

    std::for_each( ENEMY_BIRTH_PLACE_ARR.begin(), ENEMY_BIRTH_PLACE_ARR.end(), [this] ( const RelativeRect *pRelativeRect )
    {
        m_EnemyBirthPlaceVc.push_back( RelativeToBase( *pRelativeRect ) );
    });

    m_Player1BirthPlace = RelativeToBase( PLAYER1_BIRTH_POS );
    m_Player2BirthPlace = RelativeToBase( PLAYER2_BIRTH_POS );
}

void GameEngine::playersBirth( const CommonTanksProperties::TankOwnerIdentity &tankOwner, const uint32_t &nCurrentLives,
                               bool &fPlayerAlive )
{
    if( nCurrentLives && !fPlayerAlive )
    {
        const RelativeRect *pBirthPlace = CommonTanksProperties::TankOwnerIdentity::PLAYER1 == tankOwner ?
                                          &PLAYER1_BIRTH_POS : &PLAYER2_BIRTH_POS;
        SDL_Rect BirthPlace             = CommonTanksProperties::TankOwnerIdentity::PLAYER1 == tankOwner ?
                                          m_Player1BirthPlace : m_Player2BirthPlace;

        for( pSharedTank &pTank: m_TanksList )
        {
            if( SDL_HasIntersection( &( pTank->m_AnimCurPos ), &BirthPlace ) )
            {
                pBirthPlace = nullptr;
                break;
            }
        }

        if( pBirthPlace )
        {
            m_TanksList.push_back( std::make_shared <TankOfPlayer> (
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_MOVE_ANIM      ) ),
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_EXPLOSION_ANIM ) ),
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::BOMB_EXPLOSION_ANIM ) ),
                                                      m_PlayersAudioVc, m_pMap, *pBirthPlace, this ));

            m_TanksList.back()->attachProperties( std::make_shared <PlayersTankProperties>
                                                  ( tankOwner, CommonTanksProperties::TankType::ORDINARY_TANK ));
            fPlayerAlive = true;
        }
    }
}

void GameEngine::enemiesBirth()
{
    if( m_nNumOfEnemiesAlive < m_nNumOfEnemiesOnMap && m_nNumOfEnemies < MAX_NUM_OF_ENEMIES )
    {
        bool fBonusTank( false );
        const RelativeRect *pBirthPlace( nullptr );
        SDL_Rect BirthPlace;

        if( FIRST_BONUS_TANK_NUMBER == m_nNumOfEnemies ||  SECOND_BONUS_TANK_NUMBER == m_nNumOfEnemies ||
            THIRD_BONUS_TANK_NUMBER == m_nNumOfEnemies )
        {
            fBonusTank = true;
            m_pBonus.reset();
        }

        for( size_t i = 0, nVcSize = m_EnemyBirthPlaceVc.size(); i < nVcSize && !pBirthPlace; ++i )
        {
            pBirthPlace = ENEMY_BIRTH_PLACE_ARR.at( i );
            BirthPlace = m_EnemyBirthPlaceVc.at( i );

            for( pSharedTank &pTank: m_TanksList )
            {
                if( SDL_HasIntersection( &( pTank->m_AnimCurPos ), &BirthPlace ) )
                {
                    pBirthPlace = nullptr;
                    break;
                }
            }
        }

        if( pBirthPlace )
        {
            CommonTanksProperties::MoveDirection moveDirection = CommonTanksProperties::MoveDirection::UP;

            switch( m_Rand1_4() )
            {
            case 1:
                moveDirection = CommonTanksProperties::MoveDirection::UP;
                break;
            case 2:
                moveDirection = CommonTanksProperties::MoveDirection::DOWN;
                break;
            case 3:
                moveDirection = CommonTanksProperties::MoveDirection::LEFT;
                break;
            case 4:
                moveDirection = CommonTanksProperties::MoveDirection::RIGHT;
                break;
            }

            m_TanksList.push_back( std::make_shared <TankOfEnemy> (
                                                  m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_MOVE_ANIM      ) ),
                                                  m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_EXPLOSION_ANIM ) ),
                                                  m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::BOMB_EXPLOSION_ANIM ) ),
                                                  m_EnemyAudioVc, m_pMap, *pBirthPlace, this, moveDirection ));

            m_TanksList.back()->attachProperties( std::make_shared <EnemyTankProperties>( static_cast <CommonTanksProperties::TankType>
                                                                                        ( m_Rand1_4() ), fBonusTank ));
            ++m_nNumOfEnemiesAlive;
            ++m_nNumOfEnemies;
        }
    }
}

void GameEngine::tanksManagement()
{
    std::for_each( m_TanksList.begin(), m_TanksList.end(), [this] ( pSharedTank &pTank )
    {
        pTank->changePosition();
        auto pShell = pTank->tankShot();

        if( pShell )
        {
            m_TankShellsList.push_back( pShell );
        }
    });

    m_TanksList.remove_if( [this] ( pSharedTank &pTank )
    {
        if( pTank->isDestroyed() )
        {
            if( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == pTank->getOwnerIdentity() && m_fPlayer1Alive )
            {
                m_fPlayer1Alive = false;
                --m_nPlayer1Lives;
            }
            else if( CommonTanksProperties::TankOwnerIdentity::PLAYER2 == pTank->getOwnerIdentity() && m_fPlayer2Alive )
            {
                m_fPlayer2Alive = false;
                --m_nPlayer2Lives;
            }
            else if( m_nNumOfEnemiesAlive )
            {
                --m_nNumOfEnemiesAlive;
            }
        }
        return pTank->isDestroyed();

    });
}

void GameEngine::tankShellsManagement()
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

        std::for_each( m_TanksList.begin(), m_TanksList.end(), [&pShell, this] ( pSharedTank &pTank )
        {
            if( !pTank->isNoLives() && !pShell->getExplosion() &&
                pTank->checkTankShellCollision( pShell->getCollisionPosition(), pShell->getOwnerIdentity() ) )
            {
                if( pTank->isBonus() )
                {
                    m_pBonus = std::make_shared <Bonus> ( m_BonusesTexture, static_cast <Bonus::BonusType> ( m_RandBonus() ) );
                    SDL_Rect MapBorderRect = m_pMap->getMapBordersRect();
                    int nX = MapBorderRect.x + ( MapBorderRect.w * static_cast <int> ( m_Rand0_100() ) ) / 100;
                    int nY = MapBorderRect.y + lround( MapBorderRect.h * BONUS_BIRTH_BORDER ) +
                             lround( ( MapBorderRect.h * BONUS_BIRTH_RANGE * m_Rand0_100() ) / 100 );
                    m_pBonus->setPosition( nX, nY );
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

void GameEngine::tanksDecrease( CommonTanksProperties::TankOwnerIdentity tankOwner )
{
    if( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == tankOwner && m_fPlayer1Alive )
    {
        m_fPlayer1Alive = false;
        --m_nPlayer1Lives;
    }
    else if( CommonTanksProperties::TankOwnerIdentity::PLAYER2 == tankOwner && m_fPlayer2Alive )
    {
        m_fPlayer2Alive = false;
        --m_nPlayer2Lives;
    }
    else if( m_nNumOfEnemiesAlive )
    {
        --m_nNumOfEnemiesAlive;
    }
}

void GameEngine::attachGamePage( GamePage *pGamePage )
{
    m_pGamePage = pGamePage;
}

void GameEngine::gameStartInit()
{    
    if( GameState::GAME_OFF == m_CurrentState || GameState::GAME_WON == m_CurrentState )
    {
        m_NumOfPlayers = m_pGamePage->getNumOfPlayers();

        if( NumOfPlayers::TWO_PLAYERS == m_NumOfPlayers )
        {
            if( m_CurrentState != GameState::GAME_WON )
            {
                m_nPlayer1Lives      = START_PLAYERS_LIVES;
                m_nPlayer2Lives      = START_PLAYERS_LIVES;
            }
            m_nNumOfEnemiesOnMap = NUM_OF_ENEMIES_ON_MAP_2PLAYERS;
        }
        else if( NumOfPlayers::ONE_PLAYER == m_NumOfPlayers )
        {
            if( m_CurrentState != GameState::GAME_WON )
            {
                m_nPlayer1Lives      = START_PLAYERS_LIVES;
                m_nPlayer2Lives      = 0;
            }
            m_nNumOfEnemiesOnMap = NUM_OF_ENEMIES_ON_MAP_1PLAYER;
        }
        m_CurrentState = GameState::GAME_ON;
        m_nNumOfEnemies = 0;
        m_nNumOfEnemiesAlive = 0;
        m_PlayersHeart.init();
    }
}

void GameEngine::gameAction()
{    
    if( GameState::GAME_ON == m_CurrentState )
    {
        playersBirth( CommonTanksProperties::TankOwnerIdentity::PLAYER1, m_nPlayer1Lives, m_fPlayer1Alive );
        playersBirth( CommonTanksProperties::TankOwnerIdentity::PLAYER2, m_nPlayer2Lives, m_fPlayer2Alive );
        enemiesBirth();
        tanksManagement();
        tankShellsManagement();

        if( m_PlayersHeart.isDestroyed() || ( !m_nPlayer1Lives && !m_nPlayer2Lives && !m_fPlayer1Alive && !m_fPlayer2Alive ))
        {
            m_CurrentState = GameState::GAME_OFF;
            m_nGameStateChangingTime = SDL_GetTicks();
        }
        else if( !m_nNumOfEnemiesAlive && MAX_NUM_OF_ENEMIES == m_nNumOfEnemies )
        {
            m_nGameStateChangingTime = SDL_GetTicks();
            m_CurrentState = GameState::GAME_WON;
        }
    }
    else if( GameState::GAME_OFF == m_CurrentState )
    {
        m_fPlayer1Alive = false;
        m_fPlayer2Alive = false;
        m_nPlayer1Lives = 0;
        m_nPlayer2Lives = 0;

        std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( pSharedTank &pTank )
        {
            if( CommonTanksProperties::TankOwnerIdentity::ENEMY != pTank->getOwnerIdentity() )
            {
                pTank->m_pProperties->m_nNumberOfLives = 0;
            }
        });

        tanksManagement();
        tankShellsManagement();

        if( SDL_GetTicks() - m_nGameStateChangingTime >= GAME_OVER_TIMEOUT )
        {
            m_nGameStateChangingTime = 0;
            m_pMap->clear();
            m_TanksList.clear();
            m_TankShellsList.clear();
            m_pBonus.reset();
            m_pGamePage->changeState( CurrentState::START_PAGE );
        }
    }
    else if( GameState::GAME_WON == m_CurrentState )
    {
        tanksManagement();
        tankShellsManagement();

        if( SDL_GetTicks() - m_nGameStateChangingTime >= GAME_WON_TIMEOUT )
        {
            m_nGameStateChangingTime = 0;
            m_pMap->clear();
            m_TanksList.clear();
            m_TankShellsList.clear();
            m_fPlayer1Alive = false;
            m_fPlayer2Alive = false;
            m_pBonus.reset();

            if( m_pMap->isMapsStillPresent() )
            {
                m_pMap->loadMap();
                gameStartInit();
                m_pGamePage->playStartSound();
            }
            else
            {
                m_pMap->resetCurrentMapIndex();
                m_pGamePage->changeState( CurrentState::START_PAGE );
                m_CurrentState = GameState::GAME_OFF;
            }
        }
    }
}

void GameEngine::resize()
{
    std::for_each( m_AnimTextureVc.begin(), m_AnimTextureVc.end(), [] ( Animation &animation )
    {
        animation.changeSize();
    });

    std::for_each( m_LivesAndEnemiesTextureVc.begin(), m_LivesAndEnemiesTextureVc.end(), [] ( Texture &texture )
    {
        texture.changeSize();
    });

    std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( pSharedTank &pTank )
    {
        pTank->changeSize();
    });

    std::for_each( m_TankShellsList.begin(), m_TankShellsList.end(), [] ( pSharedTankShell &pShell )
    {
        pShell->changeSize();
    });

    m_PlayersHeart.resize();
    calcBirthPlacesRectSize();
    calcLiveRectSize();
    m_GameOverText.changeSize();
}

void GameEngine::render()
{
    livesAndEnemies();

    std::for_each( m_TankShellsList.begin(), m_TankShellsList.end(), [] ( pSharedTankShell &pShell )
    {
        pShell->render();
    });

    std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( pSharedTank &pTank )
    {
        pTank->render();
    });

    m_PlayersHeart.render();

    if( GameState::GAME_OFF == m_CurrentState && m_nGameStateChangingTime )
    {
        m_GameOverText.render();
    }

    if( m_pBonus )
    {
        m_pBonus->render();
    }
}

void GameEngine::setStartOrPause()
{
    if( GameState::GAME_PAUSED != m_CurrentState )
    {
        m_PreviousState = m_CurrentState;
        m_CurrentState = GameState::GAME_PAUSED;
    }
    else
    {
        m_CurrentState = m_PreviousState;
    }

    if( GameState::GAME_PAUSED == m_CurrentState )
    {
        std::for_each( m_TanksList.begin(), m_TanksList.end(), [] ( pSharedTank &pTank )
        {
            pTank->stopMove();
        });
    }
}

bool GameEngine::isGameOn()
{
    return GameState::GAME_OFF != m_CurrentState;
}
