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
                        ObjectsManagementInitData objectsManagementInitData, Renderer renderer, pSharedMap pMap ):
    m_Renderer( renderer ), m_pMap( pMap )
{
    animInit( AnimInitDataVc, renderer );
    livesAndEnemiesInit( LivesAndEnemiesInitDataVc, renderer );
    soundsInit();

    m_pObjectsManagement = std::make_shared <ObjectsManagement> ( this, pMap, objectsManagementInitData, renderer );
}

GameEngine::~GameEngine() {}

void GameEngine::animInit( std::vector <AnimationInitData> AnimInitDataVc, Renderer renderer )
{
    m_AnimTextureVc.reserve( AnimInitDataVc.size() );
    std::for_each( AnimInitDataVc.begin(), AnimInitDataVc.end(), [this, renderer] ( AnimationInitData InitData )
    {
        m_AnimTextureVc.emplace_back( InitData, renderer );
    });
}

void GameEngine::livesAndEnemiesInit(std::vector <ImgTextureInitData> LivesAndEnemiesInitDataVc , Renderer renderer)
{
    m_LivesAndEnemiesTextureVc.reserve( LivesAndEnemiesInitDataVc.size() + 1 );
    std::for_each( LivesAndEnemiesInitDataVc.begin(), LivesAndEnemiesInitDataVc.end(), [this, renderer] ( ImgTextureInitData InitData )
    {
        m_LivesAndEnemiesTextureVc.emplace_back( InitData, renderer );
    });

    m_EnemyImagesPlaceVc.resize  ( MAX_NUM_OF_ENEMIES, { 0, 0, 0, 0 });
    m_Player1ImagesPlaceVc.resize( MAX_PLAYERS_LIVES,  { 0, 0, 0, 0 });
    m_Player2ImagesPlaceVc.resize( MAX_PLAYERS_LIVES,  { 0, 0, 0, 0 });
    calcLiveRectSize();
}

void GameEngine::soundsInit()
{
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
}


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

    for( uint32_t i = 0, nArrSize = m_Player1ImagesPlaceVc.size(); i < nArrSize && i < m_nPlayer1Lives; ++i )
    {
        m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( TexturePurpose::TANKS_OF_PLAYER1_LIFE )).render( nullptr,
                                                                                                      &m_Player1ImagesPlaceVc.at(i) );
    }

    for( uint32_t i = 0, nArrSize = m_Player2ImagesPlaceVc.size(); i < nArrSize && i < m_nPlayer2Lives; ++i )
    {
        m_LivesAndEnemiesTextureVc.at( static_cast <size_t>( TexturePurpose::TANKS_OF_PLAYER2_LIFE )).render( nullptr,
                                                                                                      &m_Player2ImagesPlaceVc.at(i) );
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

        if( m_pObjectsManagement->checkTankCollision( BirthPlace ) )
        {
            pBirthPlace = nullptr;
        }

        if( pBirthPlace )
        {
            auto pTank = std::make_shared<TankOfPlayer> ( m_AnimTextureVc.at( static_cast<size_t>( AnimPurpose::TANK_MOVE_ANIM      )),
                                                          m_AnimTextureVc.at( static_cast<size_t>( AnimPurpose::TANK_EXPLOSION_ANIM )),
                                                          m_AnimTextureVc.at( static_cast<size_t>( AnimPurpose::BOMB_EXPLOSION_ANIM )),
                                                          m_PlayersAudioVc, m_pMap, *pBirthPlace, m_pObjectsManagement );

            pTank->attachProperties( std::make_shared <PlayersTankProperties>
                                     ( tankOwner, ( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == tankOwner ?
                                                  m_Pl1DefTankType : m_Pl2DefTankType ) ));
            m_pObjectsManagement->addTank( pTank );
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
            m_pObjectsManagement->disableBonus();
        }

        for( size_t i = 0, nVcSize = m_EnemyBirthPlaceVc.size(); i < nVcSize && !pBirthPlace; ++i )
        {
            pBirthPlace = ENEMY_BIRTH_PLACE_ARR.at( i );
            BirthPlace = m_EnemyBirthPlaceVc.at( i );

            if( m_pObjectsManagement->checkTankCollision( BirthPlace ) )
            {
                pBirthPlace = nullptr;
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

            auto pTank = std::make_shared<TankOfEnemy> ( m_AnimTextureVc.at( static_cast<size_t> ( AnimPurpose::TANK_MOVE_ANIM      )),
                                                         m_AnimTextureVc.at( static_cast<size_t> ( AnimPurpose::TANK_EXPLOSION_ANIM )),
                                                         m_AnimTextureVc.at( static_cast<size_t> ( AnimPurpose::BOMB_EXPLOSION_ANIM )),
                                                         m_EnemyAudioVc, m_pMap, *pBirthPlace, m_pObjectsManagement, moveDirection );

            pTank->attachProperties( std::make_shared <EnemyTankProperties>( static_cast <CommonTanksProperties::TankType>
                                                                             ( m_Rand1_4() ), fBonusTank ));
            m_pObjectsManagement->addTank( pTank );
            ++m_nNumOfEnemiesAlive;
            ++m_nNumOfEnemies;
        }
    }
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

void GameEngine::livesIncrease( CommonTanksProperties::TankOwnerIdentity tankOwner )
{
    if( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == tankOwner && m_nPlayer1Lives < MAX_PLAYERS_LIVES )
    {
        ++m_nPlayer1Lives;
    }
    else if( CommonTanksProperties::TankOwnerIdentity::PLAYER2 == tankOwner && m_nPlayer2Lives < MAX_PLAYERS_LIVES )
    {
        ++m_nPlayer2Lives;
    }
}

void GameEngine::setPlayersTankType( CommonTanksProperties::TankOwnerIdentity tankOwner, CommonTanksProperties::TankType tankType )
{
    if( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == tankOwner )
    {
        m_Pl1DefTankType = tankType;
    }
    else if( CommonTanksProperties::TankOwnerIdentity::PLAYER2 == tankOwner )
    {
        m_Pl2DefTankType = tankType;
    }
}

void GameEngine::resetPlayersTankType( CommonTanksProperties::TankOwnerIdentity tankOwner )
{
    if( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == tankOwner )
    {
        m_Pl1DefTankType = CommonTanksProperties::TankType::ORDINARY_TANK;
    }
    else if( CommonTanksProperties::TankOwnerIdentity::PLAYER2 == tankOwner )
    {
        m_Pl2DefTankType = CommonTanksProperties::TankType::ORDINARY_TANK;
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
        m_nHighScore = m_pGamePage->getHighScore();

        if( NumOfPlayers::TWO_PLAYERS == m_NumOfPlayers )
        {
            if( m_CurrentState != GameState::GAME_WON )
            {
                m_nPlayer1Lives = START_PLAYERS_LIVES;
                m_nPlayer2Lives = START_PLAYERS_LIVES;
            }
            m_nNumOfEnemiesOnMap = NUM_OF_ENEMIES_ON_MAP_2PLAYERS;
        }
        else if( NumOfPlayers::ONE_PLAYER == m_NumOfPlayers )
        {
            if( m_CurrentState != GameState::GAME_WON )
            {
                m_nPlayer1Lives = START_PLAYERS_LIVES;
                m_nPlayer2Lives = 0;
            }
            m_nNumOfEnemiesOnMap = NUM_OF_ENEMIES_ON_MAP_1PLAYER;
        }
        if( m_pMap->isMapIndexDefault() )
        {
            m_pMap->loadMap();
        }
        m_CurrentState = GameState::GAME_ON;
        m_nNumOfEnemies = 0;
        m_nNumOfEnemiesAlive = 0;
        m_pObjectsManagement->init();
    }
}

void GameEngine::gameAction()
{
    switch( m_CurrentState )
    {
    case GameState::GAME_ON:
        playersBirth( CommonTanksProperties::TankOwnerIdentity::PLAYER1, m_nPlayer1Lives, m_fPlayer1Alive );
        playersBirth( CommonTanksProperties::TankOwnerIdentity::PLAYER2, m_nPlayer2Lives, m_fPlayer2Alive );
        enemiesBirth();
        m_pObjectsManagement->mainProcess();

        if( m_pObjectsManagement->heartIsDestroyed() || ( !m_nPlayer1Lives && !m_nPlayer2Lives && !m_fPlayer1Alive && !m_fPlayer2Alive ))
        {
            m_CurrentState = GameState::GAME_OFF;
            m_nGameStateChangingTime = SDL_GetTicks();
        }
        else if( !m_nNumOfEnemiesAlive && MAX_NUM_OF_ENEMIES == m_nNumOfEnemies )
        {
            m_nGameStateChangingTime = SDL_GetTicks();
            m_CurrentState = GameState::GAME_WON;
        }
        break;

    case GameState::GAME_PAUSED:
        if( m_nGameStateChangingTime )
        {
            m_nGameStateChangingTime = SDL_GetTicks();
        }
        break;

    case GameState::GAME_OFF:
        m_fPlayer1Alive = false;
        m_fPlayer2Alive = false;
        m_nPlayer1Lives = 0;
        m_nPlayer2Lives = 0;

        m_pObjectsManagement->destroyPlayersTanks();
        m_pObjectsManagement->mainProcess();
        m_pObjectsManagement->resetScores();
        m_pGamePage->setCurrentScore( 0 );

        if( SDL_GetTicks() - m_nGameStateChangingTime >= GAME_OVER_TIMEOUT )
        {
            m_nGameStateChangingTime = 0;
            m_pObjectsManagement->gameEndClear();
            m_pGamePage->changeState( CurrentState::START_PAGE );
            m_pMap->resetCurrentMapIndex();
            resetPlayersTankType( CommonTanksProperties::TankOwnerIdentity::PLAYER1 );
            resetPlayersTankType( CommonTanksProperties::TankOwnerIdentity::PLAYER2 );
        }
        break;

    case GameState::GAME_WON:
        m_pObjectsManagement->mainProcess();
        m_pObjectsManagement->finalScoresCounting();

        if( SDL_GetTicks() - m_nGameStateChangingTime >= GAME_WON_TIMEOUT )
        {
            m_nGameStateChangingTime = 0;
            m_pObjectsManagement->gameEndClear();
            m_fPlayer1Alive = false;
            m_fPlayer2Alive = false;

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
                resetPlayersTankType( CommonTanksProperties::TankOwnerIdentity::PLAYER1 );
                resetPlayersTankType( CommonTanksProperties::TankOwnerIdentity::PLAYER2 );
                m_pGamePage->setCurrentScore( m_pObjectsManagement->getCurrentScore() );
                m_pObjectsManagement->resetScores();
            }
        }
        break;
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

    m_pObjectsManagement->resize();
    calcBirthPlacesRectSize();
    calcLiveRectSize();
}

void GameEngine::render()
{
    livesAndEnemies();
    m_pMap->renderLowerLayer();
    m_pObjectsManagement->render();
    m_pMap->renderUpperLayer();

    if( (( GameState::GAME_PAUSED == m_CurrentState   && GameState::GAME_WON == m_PreviousState ) ||
           GameState::GAME_WON    == m_CurrentState ) && m_nGameStateChangingTime )
    {
        m_pObjectsManagement->finalScoresRender();
    }
    else if((( GameState::GAME_PAUSED == m_CurrentState   && GameState::GAME_OFF == m_PreviousState ) ||
               GameState::GAME_OFF    == m_CurrentState ) && m_nGameStateChangingTime )
    {
        m_pObjectsManagement->gameOverTextRender();
    }
}

void GameEngine::setPause()
{
    if( GameState::GAME_PAUSED != m_CurrentState )
    {
        m_PreviousState = m_CurrentState;
        m_CurrentState = GameState::GAME_PAUSED;
    }

    if( GameState::GAME_PAUSED == m_CurrentState )
    {
        m_pObjectsManagement->setPause();
    }
}

void GameEngine::resetPause()
{
    m_CurrentState = m_PreviousState;
}

bool GameEngine::isGameOn()
{
    return GameState::GAME_OFF != m_CurrentState;
}

bool GameEngine::isGamePaused()
{
    return GameState::GAME_PAUSED == m_CurrentState;
}
