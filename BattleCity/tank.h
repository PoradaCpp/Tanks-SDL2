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

    enum class MoveDirection
    {
        UP    =   90,
        DOWN  =  -90,
        LEFT  =   180,
        RIGHT =   0
    };

    Tank( Animation TankMoveAnim, Animation TankExplosionAnim, Animation ShellExplosionAnim, pSharedMap pMap,
          RelativeRect RelativeBirthPos, GameEngine *pGameEngine, MoveDirection moveDirection = MoveDirection::UP );
    ~Tank() override;

    void changeSize() override;
    void render() override;

    virtual void attachProperties( pSharedTankProperties pProperties );
    virtual void changePosition();
    virtual pSharedTankShell tankShot();
    virtual bool isNoLives();
    virtual bool isDestroyed();
    virtual bool checkTankShellCollision( const SDL_Rect &Rect, CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity );

    CommonTanksProperties::TankOwnerIdentity getOwnerIdentity() const;
    void setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect = { 0, 0, 0, 0 } );

protected:
    Animation m_TankMoveAnim;
    Animation m_TankExplosionAnim;
    Animation m_ShellExplosionAnim;
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
    MoveDirection m_MoveDirection;
    bool m_fMove;
    bool m_fBirth;
    bool m_fDestroyed;
    uint32_t m_nShotTime;
    uint32_t m_nDestroyingTime;
    GameEngine *m_pGameEngine;

    void countRealTankSize();
    MoveDirection determineDirection( SDL_Rect &NewPos );
    void allowNewPosition( SDL_Rect &NewPos );
    SDL_Rect calcPossiblePos( SDL_Rect &NewPos, MoveDirection NewDirection );
    bool alignmentAfterTurn( SDL_Rect &PossiblePos );
    bool alignmentAfterResize( SDL_Rect &PossiblePos );
    bool checkTankCollision();
    bool checkTankCollision( const SDL_Rect &Rect );
    bool checkNewPosition( SDL_Rect &NewPos );
    bool newPositionProcessing();
    void calcShellPosition();
    void tankExplosing();
};



#endif // TANK_H