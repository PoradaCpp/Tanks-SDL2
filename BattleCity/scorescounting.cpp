#include <algorithm>

#include "scorescounting.h"
#include "tankshell.h"

ScoresCounting::ScoresCounting( ImgTextureInitData BackgroundTextureInitData, std::vector <Text3DInitData> ScoresFormTextInitDataVc,
                                std::vector <Text3DInitData> AmountsTextInitDataVc, std::string sScoresCountingAudioPath,
                                Renderer renderer ): m_BackgroundTexture( BackgroundTextureInitData, renderer ),
    m_ScoresCountingAudio( sScoresCountingAudioPath )

{
    std::for_each( ScoresFormTextInitDataVc.begin(), ScoresFormTextInitDataVc.end(), [this, renderer] ( Text3DInitData &InitData )
    {
        m_ScoresFormTextVc.push_back( std::make_shared <Text3D> ( InitData, renderer ) );
    });

    std::for_each( AmountsTextInitDataVc.begin(), AmountsTextInitDataVc.end(), [this, renderer] ( Text3DInitData &InitData )
    {
        m_AmountsTextVc.push_back( std::make_shared <Text3D> ( InitData, renderer ) );
    });

    std::for_each( m_AmountsTextVc.begin() + static_cast <size_t> ( PointsAndAmounts::PLAIN_TANKS_SCORE ), m_AmountsTextVc.end(),
                   [] ( pSharedText3D &pText3D )
    {
        pText3D->setAlignment( TextTexture::TextAlignment::RIGHT_SIDE );
    });
}

ScoresCounting::~ScoresCounting() {}

void ScoresCounting::render()
{
    m_BackgroundTexture.render();

    std::for_each( m_ScoresFormTextVc.begin(), m_ScoresFormTextVc.end(), [] ( pSharedText3D &pText3D )
    {
        if( pText3D )
        {
            pText3D->render();
        }
    });

    std::for_each( m_AmountsTextVc.begin(), m_AmountsTextVc.end(), [] ( pSharedText3D &pText3D )
    {
        if( pText3D )
        {
            pText3D->render();
        }
    });
}

void ScoresCounting::changeSize()
{
    m_BackgroundTexture.changeSize();

    std::for_each( m_ScoresFormTextVc.begin(), m_ScoresFormTextVc.end(), [] ( pSharedText3D &pText3D )
    {
        if( pText3D )
        {
            pText3D->changeSize();
        }
    });

    std::for_each( m_AmountsTextVc.begin(), m_AmountsTextVc.end(), [] ( pSharedText3D &pText3D )
    {
        if( pText3D )
        {
            pText3D->changeSize();
        }
    });
}

void ScoresCounting::scoresProcessing( const pSharedTank &pTank , const pSharedTankShell &pShell )
{
    if( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == pShell->getOwnerIdentity() )
    {
        m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::PLAYER_1_SCORE ) ) += pTank->getDestroyingScores();
    }
    else if( CommonTanksProperties::TankOwnerIdentity::PLAYER2 == pShell->getOwnerIdentity() )
    {
        m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::PLAYER_2_SCORE ) ) += pTank->getDestroyingScores();
    }

    switch( pTank->getTankType() )
    {
    case CommonTanksProperties::TankType::ORDINARY_TANK:
        ++m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::AMOUNT_OF_PLAIN_TANKS ) );
        m_PointsAndAmountsArr.at(   static_cast <size_t> ( PointsAndAmounts::PLAIN_TANKS_SCORE    ) ) += pTank->getDestroyingScores();
        break;

    case CommonTanksProperties::TankType::BTR:
        ++m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::AMOUNT_OF_BTR ) );
        m_PointsAndAmountsArr.at(   static_cast <size_t> ( PointsAndAmounts::BTR_SCORE    ) ) += pTank->getDestroyingScores();
        break;

    case CommonTanksProperties::TankType::QUICK_SHOOTING_BTR:
        ++m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::AMOUNT_OF_QS_BTR ) );
        m_PointsAndAmountsArr.at(   static_cast <size_t> ( PointsAndAmounts::QS_BTR_SCORE    ) ) += pTank->getDestroyingScores();
        break;

    case CommonTanksProperties::TankType::HEAVY_TANK:
        ++m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::AMOUNT_OF_HEAVY_TANKS ) );
        m_PointsAndAmountsArr.at(   static_cast <size_t> ( PointsAndAmounts::HEAVY_TANKS_SCORE    ) ) += pTank->getDestroyingScores();
        break;
    }
}

void ScoresCounting::addToScores( CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity, uint32_t nScores )
{
    if( CommonTanksProperties::TankOwnerIdentity::PLAYER1 == tankOwnerIdentity )
    {
        m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::PLAYER_1_SCORE ) ) += nScores;
    }
    else if( CommonTanksProperties::TankOwnerIdentity::PLAYER2 == tankOwnerIdentity )
    {
        m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::PLAYER_2_SCORE ) ) += nScores;
    }
}

void ScoresCounting::resetScores()
{
    std::for_each( m_PointsAndAmountsArr.begin() + static_cast <size_t> ( PointsAndAmounts::PLAYER_1_SCORE ),
                   m_PointsAndAmountsArr.end(), [] ( uint32_t &nPoints )
    {
        nPoints = 0;
    });

    std::for_each( m_AmountsTextVc.begin() + static_cast <size_t> ( PointsAndAmounts::PLAYER_1_SCORE ),
                   m_AmountsTextVc.end(), [] ( pSharedText3D &pText3D )
    {
        if( pText3D )
        {
            pText3D->setText( "0" );
        }
    });
}

void ScoresCounting::resetStageScores()
{
    std::for_each( m_PointsAndAmountsArr.begin() + static_cast <size_t> ( PointsAndAmounts::PLAIN_TANKS_SCORE ),
                   m_PointsAndAmountsArr.end(), [] ( uint32_t &nPoints )
    {
        nPoints = 0;
    });

    std::for_each( m_AmountsTextVc.begin() + static_cast <size_t> ( PointsAndAmounts::PLAIN_TANKS_SCORE ),
                   m_AmountsTextVc.end(), [] ( pSharedText3D &pText3D )
    {
        if( pText3D )
        {
            pText3D->setText( "0" );
        }
    });
}

void ScoresCounting::resetScoresCounter()
{
    m_nCurrentScorePos  = 0;
    m_nScoreForPiece    = 0;
    m_nQuantity         = 0;
    m_nScoreCounter     = 0;
}

void ScoresCounting::scoresCounting()
{
    if( !m_nCurrentScorePos )
    {
        for( size_t i = 0, nVcSize = m_AmountsTextVc.size(), nArrSize = m_PointsAndAmountsArr.size(); i < nVcSize && i < nArrSize &&
             i < static_cast <size_t> ( PointsAndAmounts::PLAIN_TANKS_SCORE ) ; ++i )
        {
            m_AmountsTextVc.at( i )->setText( std::to_string( m_PointsAndAmountsArr.at( i ) ) );
            ++m_nCurrentScorePos;
        }
    }
    else if( m_nCurrentScorePos + 1 < m_AmountsTextVc.size() && m_nCurrentScorePos + 1 < m_PointsAndAmountsArr.size() )
    {
        if( !m_fScoreCountingPause )
        {
            m_nScoreCountingTime = SDL_GetTicks();
            m_fScoreCountingPause = true;
        }

        if( !m_nScoreForPiece )
        {
            m_nScoreForPiece = m_PointsAndAmountsArr.at( m_nCurrentScorePos );
            m_nQuantity = m_PointsAndAmountsArr.at( m_nCurrentScorePos + 1 );

            if( m_nQuantity )
            {
                m_nScoreForPiece /= m_nQuantity;
            }
        }
        if( SDL_GetTicks() - m_nScoreCountingTime >= SCORE_COUNTING_DELAY_TIME )
        {
            if( m_nScoreCounter++ < m_nQuantity )
            {
                m_AmountsTextVc.at( m_nCurrentScorePos + 1 )->setText( std::to_string( m_nScoreCounter                    ) );
                m_AmountsTextVc.at( m_nCurrentScorePos     )->setText( std::to_string( m_nScoreCounter * m_nScoreForPiece ) );
                m_nScoreCountingTime = 0;
                m_fScoreCountingPause = false;
                m_ScoresCountingAudio.play();
            }
            else
            {
                m_nCurrentScorePos += 2;
                m_nScoreForPiece    = 0;
                m_nQuantity         = 0;
                m_nScoreCounter     = 0;
            }
        }
    }
}

void ScoresCounting::setStageNumber( uint32_t nStageNumber )
{
    m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::STAGE_NUMBER ) ) = nStageNumber;
}

void ScoresCounting::setHighScore( uint32_t nHighScore )
{
    m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::HIGH_SCORE ) ) = nHighScore;
}

uint32_t ScoresCounting::getCurrentScore() const
{
    return ( m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::PLAYER_1_SCORE ) ) +
             m_PointsAndAmountsArr.at( static_cast <size_t> ( PointsAndAmounts::PLAYER_2_SCORE ) ) );
}
