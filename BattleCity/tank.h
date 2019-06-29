#ifndef TANK_H
#define TANK_H

#include <array>
#include <vector>

#include "tankproperties.h"
#include "map.h"

class GameEngine;
class TankShell;
typedef std::shared_ptr <TankShell> pSharedTankShell;
class Tank;
typedef std::shared_ptr <Tank> pSharedTank;

class Tank: public DisplayedObject
{
public:
    friend class GameEngine;

    Tank( Animation TankMoveAnim, Animation TankExplosionAnim, Animation ShellExplosionAnim, std::vector <AudioChunk> audioVc,
          pSharedMap pMap, RelativeRect RelativeBirthPos, GameEngine *pGameEngine, CommonTanksProperties::MoveDirection moveDirection );
    ~Tank() override;

    void changeSize() override;
    void render() override;

    virtual void attachProperties( pSharedTankProperties pProperties );
    virtual void changePosition();
    virtual pSharedTankShell tankShot();
    virtual bool isNoLives();
    virtual bool isDestroyed();
    virtual bool isBonus();
    virtual bool checkTankShellCollision( const SDL_Rect &Rect, CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity );
    virtual bool checkTankCollision( const SDL_Rect &Rect );

    CommonTanksProperties::TankOwnerIdentity getOwnerIdentity() const;
    void setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect = { 0, 0, 0, 0 } );
    void stopMove();

protected:
    Animation m_TankMoveAnim;
    Animation m_TankExplosionAnim;
    Animation m_ShellExplosionAnim;
    std::vector <AudioChunk> m_AudioVc;
    pSharedTankProperties m_pProperties;
    pSharedMap m_pMap;
    SDL_Rect m_AnimCurPos;
    SDL_Rect *m_pRealCurPos;
    SDL_Rect m_RealSizeRectHoriz;
    SDL_Rect m_RealSizeRectVert;
    SDL_Point m_TankCenter;
    SDL_Point m_ShellPosition;
    uint32_t m_nAnimBegin;
    uint32_t m_nAnimEnd;
    CommonTanksProperties::MoveDirection m_MoveDirection;
    bool m_fMove;
    bool m_fBirth;
    bool m_fDestroyed;
    uint32_t m_nShotTime;
    uint32_t m_nDestroyingTime;
    GameEngine *m_pGameEngine;

    void countRealTankSize();
    CommonTanksProperties::MoveDirection determineDirection( SDL_Rect &NewPos );
    void allowNewPosition( SDL_Rect &NewPos );
    SDL_Rect calcPossiblePos( SDL_Rect &NewPos, CommonTanksProperties::MoveDirection NewDirection );
    bool alignmentAfterTurn( SDL_Rect &PossiblePos );
    bool alignmentAfterResize( SDL_Rect &PossiblePos );
    bool checkNewPosition( SDL_Rect &NewPos );
    bool newPositionProcessing();
    void calcShellPosition();
    void tankExplosing();
};



#endif // TANK_H
