#include <algorithm>
#include <iostream>
#include <cmath>

#include "gameengine.h"
#include "tankofplayer.h"
#include "tankofenemy.h"
#include "tankshell.h"

const RelativeRect GameEngine::PLAYER1_BIRTH_POS = { 31, 85,  5.7, 10 };
const RelativeRect GameEngine::PLAYER2_BIRTH_POS = { 47, 85,  5.7, 10 };
const RelativeRect GameEngine::ENEMY1_BIRTH_POS  = { 2,  2.5, 5.7, 10 };
const RelativeRect GameEngine::ENEMY2_BIRTH_POS  = { 27, 2.5, 5.7, 10 };
const RelativeRect GameEngine::ENEMY3_BIRTH_POS  = { 51, 2.5, 5.7, 10 };
const RelativeRect GameEngine::ENEMY4_BIRTH_POS  = { 76, 2.5, 5.7, 10 };

//std::vector <SDL_Rect> vc1, vc2;

const std::array <const RelativeRect*, 4> GameEngine::ENEMY_BIRTH_PLACE_ARR{ &ENEMY1_BIRTH_POS, &ENEMY2_BIRTH_POS,
                                                                             &ENEMY3_BIRTH_POS, &ENEMY4_BIRTH_POS };

GameEngine::GameEngine( std::shared_ptr <std::vector <pSharedDisplayedObj> > pDisplayedObjVc,
                        std::vector<AnimationInitData> AnimInitDataVc, std::vector<ImgTextureInitData> TextureInitDataVc,
                        Renderer renderer, pSharedMap pMap ):
    m_pGlobalGameState( nullptr), m_pDisplayedObjVc( pDisplayedObjVc ), m_Renderer( renderer ), m_pMap( pMap ),
    m_NumOfPlayers( NumOfPlayers::ONE_PLAYER ), m_nNumOfEnemies( 0 ),
    m_nNumOfEnemiesOnMap( NUM_OF_ENEMIES_ON_MAP_1PLAYER ), m_nNumOfEnemiesAlive( 0 ), m_nPlayer1Lives( START_PLAYERS_LIVES ),
    m_nPlayer2Lives( 0 ), m_fPlayer1Alive( false ), m_fPlayer2Alive( false ), m_GameState( GameState::GAME_OFF ), m_Rand1_4( 1, 4 )
{
    m_TanksVc.reserve( MAX_ACTIVE_TANKS_NUMBER );
    m_AnimTextureVc.reserve( AnimInitDataVc.size() );
    std::for_each( AnimInitDataVc.begin(), AnimInitDataVc.end(), [this, renderer] ( AnimationInitData InitData )
    {
        m_AnimTextureVc.emplace_back( InitData, renderer );
    });

    m_TextureVc.reserve( TextureInitDataVc.size() + 1 );
    std::for_each( TextureInitDataVc.begin(), TextureInitDataVc.end(), [this, renderer] ( ImgTextureInitData InitData )
    {
        m_TextureVc.emplace_back( InitData, renderer );
    });

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
    SDL_Rect rect = m_TextureVc.at( static_cast <size_t>( texturePurpose ) ).getDestination();
    int nDx = lround( m_TextureVc.front().getPageWidth() *  LIVES_X_COLUMNS_SHIFT / 100 );
    int nDy = lround( m_TextureVc.front().getPageHeight() * LIVES_Y_ROWS_SHIFT / 100 );
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
            m_TextureVc.at( static_cast <size_t>( TexturePurpose::CREATED_ENEMY_IMAGE ) ).setPosition( m_EnemyImagesPlaceVc.at( i ).x,
                                                                                                      m_EnemyImagesPlaceVc.at( i ).y );
            m_TextureVc.at( static_cast <size_t>( TexturePurpose::CREATED_ENEMY_IMAGE ) ).render();
        }
        else if( i < MAX_NUM_OF_ENEMIES - m_nNumOfEnemies + m_nNumOfEnemiesAlive  )
        {
            m_TextureVc.at( static_cast <size_t>( TexturePurpose::REMAIN_ENEMY_IMAGE ) ).setPosition( m_EnemyImagesPlaceVc.at( i ).x,
                                                                                                      m_EnemyImagesPlaceVc.at( i ).y );
            m_TextureVc.at( static_cast <size_t>( TexturePurpose::REMAIN_ENEMY_IMAGE ) ).render();
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
            m_TextureVc.at( static_cast <size_t>( TexturePurpose::TANKS_OF_PLAYER1_LIFE )).setPosition( m_Player1ImagesPlaceVc.at(i).x,
                                                                                                        m_Player1ImagesPlaceVc.at(i).y );
            m_TextureVc.at( static_cast <size_t>( TexturePurpose::TANKS_OF_PLAYER1_LIFE )).render();
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
            m_TextureVc.at( static_cast <size_t>( TexturePurpose::TANKS_OF_PLAYER2_LIFE )).setPosition( m_Player2ImagesPlaceVc.at(i).x,
                                                                                                        m_Player2ImagesPlaceVc.at(i).y );
            m_TextureVc.at( static_cast <size_t>( TexturePurpose::TANKS_OF_PLAYER2_LIFE )).render();
        }
        else
        {
            break;
        }
    }

    /*SDL_SetRenderDrawColor( m_Renderer, 0, 0, 255, 255 );
    std::for_each( vc1.begin(), vc1.end(), [this] ( SDL_Rect &rect )
    {
        SDL_RenderDrawRect( m_Renderer, &rect );
    });

    SDL_SetRenderDrawColor( m_Renderer, 255, 0, 0, 255 );
    std::for_each( vc2.begin(), vc2.end(), [this] ( SDL_Rect &rect )
    {
        SDL_RenderFillRect( m_Renderer, &rect );
    });*/
}

void GameEngine::attachGlobalGameState( State *pGlobalGameState )
{
    m_pGlobalGameState = pGlobalGameState;
}

void GameEngine::updateNumOfPlayers()
{
    m_NumOfPlayers = m_pGlobalGameState->getNumOfPlayers();

    if( NumOfPlayers::TWO_PLAYERS == m_NumOfPlayers )
    {
        m_nPlayer2Lives      = START_PLAYERS_LIVES;
        m_nNumOfEnemiesOnMap = NUM_OF_ENEMIES_ON_MAP_2PLAYERS;
    }
    m_GameState = GameState::GAME_ON;
}

void GameEngine::gameAction()
{
    livesAndEnemies();

    if( GameState::GAME_ON == m_GameState )
    {
        if( m_nPlayer1Lives && !m_fPlayer1Alive )
        {
            pSharedTank pTankOfPlayer1 =
                    std::make_shared <TankOfPlayer> ( m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_MOVE_ANIM      ) ),
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_EXPLOSION_ANIM ) ),
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::BOMB_EXPLOSION_ANIM ) ),
                                                      m_pMap, PLAYER1_BIRTH_POS, this );
            pTankOfPlayer1->attachProperties( std::make_shared <PlayersTankProperties>
                                              ( CommonTanksProperties::TankOwnerIdentity::PLAYER1,
                                                CommonTanksProperties::TankType::ORDINARY_TANK ));
            m_TanksVc.push_back( pTankOfPlayer1 );
            m_pDisplayedObjVc->push_back( pTankOfPlayer1 );
            m_fPlayer1Alive = true;
        }

        if( m_nPlayer2Lives && !m_fPlayer2Alive )
        {
            pSharedTank pTankOfPlayer2 =
                    std::make_shared <TankOfPlayer> ( m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_MOVE_ANIM      ) ),
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_EXPLOSION_ANIM ) ),
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::BOMB_EXPLOSION_ANIM ) ),
                                                      m_pMap, PLAYER2_BIRTH_POS, this );
            pTankOfPlayer2->attachProperties( std::make_shared <PlayersTankProperties>
                                              ( CommonTanksProperties::TankOwnerIdentity::PLAYER2 ));
            m_TanksVc.push_back( pTankOfPlayer2 );
            m_pDisplayedObjVc->push_back( pTankOfPlayer2 );
            m_fPlayer2Alive = true;
        }

        if( m_nNumOfEnemiesAlive < m_nNumOfEnemiesOnMap && m_nNumOfEnemies < MAX_NUM_OF_ENEMIES )
        {
            bool fBonusTank( false );
            const RelativeRect *pBirthPlace( nullptr );
            SDL_Rect BirthPlace;

            if( FIRST_BONUS_TANK_NUMBER == m_nNumOfEnemies ||  SECOND_BONUS_TANK_NUMBER == m_nNumOfEnemies ||
                THIRD_BONUS_TANK_NUMBER == m_nNumOfEnemies )
            {
                fBonusTank = true;
            }

            for( size_t i = 0, nVcSize = m_EnemyBirthPlaceVc.size(); i < nVcSize && !pBirthPlace; ++i )
            {
                pBirthPlace = ENEMY_BIRTH_PLACE_ARR.at( i );
                BirthPlace = m_EnemyBirthPlaceVc.at( i );

                for( size_t j = 0, nVectorSize = m_TanksVc.size(); j < nVectorSize; ++j )
                {
                    if( SDL_HasIntersection( &( m_TanksVc.at( j )->m_AnimCurPos ), &BirthPlace ) )
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

                pSharedTank pTankOfEnemy =
                     std::make_shared <TankOfEnemy> ( m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_MOVE_ANIM      ) ),
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::TANK_EXPLOSION_ANIM ) ),
                                                      m_AnimTextureVc.at( static_cast <size_t> ( AnimPurpose::BOMB_EXPLOSION_ANIM ) ),
                                                      m_pMap, *pBirthPlace, this, moveDirection );

                pTankOfEnemy->attachProperties( std::make_shared <EnemyTankProperties>( static_cast <CommonTanksProperties::TankType>
                                                                                        ( m_Rand1_4() ), fBonusTank ));
                m_TanksVc.push_back( pTankOfEnemy );
                m_pDisplayedObjVc->push_back( pTankOfEnemy );
                ++m_nNumOfEnemiesAlive;
                ++m_nNumOfEnemies;
            }
        }

        std::for_each( m_TanksVc.begin(), m_TanksVc.end(), [this] ( pSharedTank &pTank )
        {
            pTank->changePosition();            
            auto pShell = pTank->tankShot();

            if( pShell )
            {
                m_TankShellsVc.push_back( pShell );
            }
        });

        m_TanksVc.erase( std::remove_if( m_TanksVc.begin(), m_TanksVc.end(), [this] ( pSharedTank &pTank )
        {
            if( pTank->isDestroyed() )
            {
                if( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == pTank->getOwnerIdentity() )
                {
                    m_fPlayer1Alive = false;
                    --m_nPlayer1Lives;
                }
                else if( CommonTanksProperties::TankOwnerIdentity::PLAYER2 == pTank->getOwnerIdentity() )
                {
                    m_fPlayer2Alive = false;
                    --m_nPlayer2Lives;
                }
                else
                {
                    --m_nNumOfEnemiesAlive;
                }
            }
            return pTank->isDestroyed();

        }),  m_TanksVc.end() );

        m_TankShellsVc.erase( std::remove_if( m_TankShellsVc.begin(), m_TankShellsVc.end(), [] ( pSharedTankShell &pShell )
        {
            return pShell->isExplosed();
        }),  m_TankShellsVc.end() );

        std::for_each( m_TankShellsVc.begin(), m_TankShellsVc.end(), [this] ( pSharedTankShell &pShell )
        {
            pShell->changePosition();
            pShell->render();

            if( !pShell->getExplosion() && m_pMap->checkCollision( pShell->getCollisionPosition() ) )
            {
                pShell->setExplosion();

                if( !m_pMap->checkBorderCollision( pShell->getCollisionPosition() ))
                {
                    m_pMap->destroy( pShell->getCollisionPosition(), pShell->getDestoyingVolume(), pShell->getMoveDirection() );
                    //std::cout << "x: " << pShell->getDestoyingVolume().x << ", y: " << pShell->getDestoyingVolume().y
                    //          << ", w: " << pShell->getDestoyingVolume().w << ", h: " << pShell->getDestoyingVolume().h << '\n';
                    //vc1.push_back( pShell->getDestoyingVolume() );
                    //vc2.push_back( pShell->getCollisionPosition() );
                }
            }

            std::for_each( m_TanksVc.begin(), m_TanksVc.end(), [&pShell] ( pSharedTank &pTank )
            {
                if( !pTank->isNoLives() && !pShell->getExplosion() &&
                    pTank->checkTankShellCollision( pShell->getCollisionPosition(), pShell->getOwnerIdentity() ) )
                {
                    pShell->setExplosion();
                }
            });

            std::for_each( m_TankShellsVc.begin(), m_TankShellsVc.end(), [&pShell] ( pSharedTankShell &pAnotherShell )
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
    }
}

void GameEngine::resize()
{
    std::for_each( m_AnimTextureVc.begin(), m_AnimTextureVc.end(), [] ( Animation &animation )
    {
        animation.changeSize();
    });

    std::for_each( m_TextureVc.begin(), m_TextureVc.end(), [] ( Texture &texture )
    {
        texture.changeSize();
    });

    m_EnemyBirthPlaceVc.clear();

    std::for_each( ENEMY_BIRTH_PLACE_ARR.begin(), ENEMY_BIRTH_PLACE_ARR.end(), [this] ( const RelativeRect *pRelativeRect )
    {
        m_EnemyBirthPlaceVc.push_back( RelativeToBase( *pRelativeRect ) );
    });

    calcLiveRectSize();
}
