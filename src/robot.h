#include "servo.h"

bool isWalking = false;
bool isForw = true;

int walkLoopPos = 0;
int lastWalkLoopPos = 7;
int walkLastServo[] = {1, 3};
int walkServoOrder[][2] = {{4, 6}, {0, 2}, {4, 6}, {5, 7}, {0, 2}, {1, 3}, {5, 7}, {1, 3}};
int walkBServoPos[] = {90, 40, 140, 90, 120, 40, 140, 120};
int walkFServoPos[] = {90, 120, 140, 90, 40, 120, 140, 40};

void lower()
{
    for (int i = 0; i < 4; i++)
    {
        servoMoveToPos[i] = 90;
    }

    for (int i = 4; i < 8; i++)
    {
        servoMoveToPos[i] = 0;
    }
}

void raise()
{
    for (int i = 0; i < 4; i++)
    {
        servoMoveToPos[i] = 90;
    }

    for (int i = 4; i < 8; i++)
    {
        servoMoveToPos[i] = 140;
    }
}

int getWalkServoPos()
{
    if (isForw)
    {
        return walkFServoPos[walkLoopPos];
    }

    else
    {
        return walkBServoPos[walkLoopPos];
    }
}

bool isAllServoStill()
{
    for (int i = 0; i < 8; i++)
    {
        if (!isServoStill[i])
        {
            return false;
        }
    }

    return true;
}

void Walking()
{
    if (walkLoopPos > 7)
    {
        walkLoopPos = 0;
    }

    int activeServo1 = walkServoOrder[walkLoopPos][0];
    int activeServo2 = walkServoOrder[walkLoopPos][1];

    if (isAllServoStill())
    {
        servoMoveToPos[activeServo1] = getWalkServoPos();
        servoMoveToPos[activeServo2] = getWalkServoPos();

        walkLoopPos++;
    }
}

void toggleWalking()
{
    isWalking = !isWalking;
}

void toggleForw()
{
    isForw = !isForw;
}

void (*customBtnArry[4])() = {lower, raise, toggleWalking, toggleForw};
