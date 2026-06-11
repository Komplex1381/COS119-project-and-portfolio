#pragma once
#include "GameCharacter.h"
#include "Map.h"
#include <windows.h>
#include "Helper.h"

enum class GhostState 
{
    CHASE,
    SCATTER,
    EATEN,
    FRIGHTENED
};

class Ghost :  public GameCharacter
{
private:
    GhostState mState;
    int mColor;

public:
    Ghost(int startRow, int startCol, double speed, int tileChar, int color);
    virtual ~Ghost() override = default;

    void draw() const override;
    void updateAI(const Map& gameMap, int pacmanX, int pacmanY);
    void setState(GhostState newState) 
    {
        mState = newState;
    }
    /*GhostState state() 
    {
        return mState;
    }*/

};

double calculateDistance(double x1, double y1, double x2, double y2);
