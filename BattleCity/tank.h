#ifndef TANK_H
#define TANK_H

#include <array>
#include <vector>

#include "tankproperties.h"
#include "map.h"

class ObjectsManagement;
class TankShell;
typedef std::shared_ptr <TankShell> pSharedTankShell;
class Tank;
typedef std::shared_ptr <Tank> pSharedTank;

class Tank: public DisplayedObject
{
public:
    friend class GameEngine;

    Tank( Animation TankMoveAnim, Animation TankExplosionAnim, Animation ShellExplosionAnim, std::vector <AudioChunk> audioVc,
          pSharedMap pMap, RelativeRect RelativeBirthPos, std::shared_ptr <ObjectsManagement> pObjectsManagement,
          CommonTanksProperties::MoveDirection moveDirection );
    ~Tank() override;

    void changeSize() override;
    void render() override;

    virtual void attachProperties( pSharedTankProperties pProperties );
    virtual void changePosition();
    virtual bool shotCondition();

    pSharedTankShell tankShot();
    bool hasNoLives();
    bool isDestroyed();
    bool isBonus();
    void resetBonus();
    uint32_t getDestroyingScores();
    void setDefence();
    bool checkTankShellCollision( const SDL_Rect &Rect, CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity );
    bool checkTankCollision( const SDL_Rect &checkingRect ) const;
    CommonTanksProperties::TankOwnerIdentity getOwnerIdentity() const;
    CommonTanksProperties::TankType getTankType() const;
    void setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect = { 0, 0, 0, 0 } );
    void stopMove();
    void resetLives();
    SDL_Rect getSolidPartPosition();
    SDL_Rect getFullObjectPosition();

protected:
    Animation m_TankMoveAnim;
    Animation m_TankExplosionAnim;
    Animation m_ShellExplosionAnim;
    std::vector <AudioChunk> m_AudioVc;
    pSharedTankProperties m_pProperties = nullptr;
    pSharedMap m_pMap;
    SDL_Rect m_AnimCurPos               = { 0, 0, 0, 0 };
    SDL_Rect *m_pRealCurPos             = nullptr;
    SDL_Rect m_RealSizeRectHoriz        = { 0, 0, 0, 0 };
    SDL_Rect m_RealSizeRectVert         = { 0, 0, 0, 0 };
    SDL_Point m_TankCenter              = { 0, 0 };
    SDL_Point m_ShellPosition           = { 0, 0 };
    uint32_t m_nAnimBegin               = 0;
    uint32_t m_nAnimEnd                 = 0;
    CommonTanksProperties::MoveDirection m_MoveDirection;
    bool m_fMove                        = false;
    bool m_fBirth                       = true;
    bool m_fDestroyed                   = false;
    bool m_fCheckingCollision           = false;
    bool m_fDefense                     = true;
    uint32_t m_nShotTime                = 0;
    uint32_t m_nDestroyingTime          = 0;
    std::shared_ptr <ObjectsManagement> m_pObjectsManagement;

    void countRealTankSize();
    CommonTanksProperties::MoveDirection determineDirection( SDL_Rect &NewPos );
    void allowNewPosition( SDL_Rect &NewPos );
    SDL_Rect calcPossiblePos( SDL_Rect &NewPos, CommonTanksProperties::MoveDirection NewDirection );
    bool tankAlignment( SDL_Rect &PossiblePos, int nShiftingLimit );
    bool checkNewPosition( SDL_Rect &NewPos );
    bool newPositionProcessing();
    void calcShellPosition();
    void tankExplosing();
};



#endif // TANK_H
