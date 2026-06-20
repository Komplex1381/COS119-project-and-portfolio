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
    WORD mColor;
    int mPreviousX = 0;
    int mPreviousY = 0;

public:
    Ghost(int startRow, int startCol, double speed, int tileChar, WORD color);
    virtual ~Ghost() override = default;

    void draw() const override;
    void updateAI(const Map& gameMap, int pacmanX, int pacmanY, Direction pacmanDirection, int blinkyX, int blinkyY);
    void setState(GhostState newState) 
    {
        mState = newState;
    }
    GhostState getState() const
    {
        return mState;
    }
    int getPreviousX() const { return mPreviousX; }
    int getPreviousY() const { return mPreviousY; }
    WORD getColor() const;
    

};

double calculateDistance(double x1, double y1, double x2, double y2);

