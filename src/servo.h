#include <Arduino.h>

int frequence = 50;
int resolution = 14;
int servoMin = 395;
int servoMax = 2020;

int speed = 5;

int servoPin[] = {35, 36, 40, 39, 38, 37, 41, 42};
int servoDir[] = {0, 1, 1, 0, 1, 0, 1, 0};
int servoOffset[] = {15, 3, -11, -13, -6, 6, 6, -4};
int servoCurentPos[] = {120, 120, 120, 120, 140, 140, 140, 140};
int servoMoveToPos[] = {120, 120, 120, 120, 140, 140, 140, 140};
bool isServoStill[] = {true, true, true, true, true, true, true, true};

void setServo(int servoNumb, int pos)
{
    if (servoDir[servoNumb])
    {
        pos = map(pos, 0, 180, 180, 0);
    }

    if (pos > 165)
    {
        pos = 165;
    }

    else if (pos < 15)
    {
        pos = 15;
    }

    pos -= servoOffset[servoNumb];

    ledcWrite(servoNumb, (pos * (servoMax - servoMin) / 180) + 395);
}

void moveServo1Step()
{
    for (int i = 0; i < sizeof(servoPin) / sizeof(servoPin[0]); i++)
    {
        isServoStill[i] = false;

        if (servoCurentPos[i] > servoMoveToPos[i])
        {
            servoCurentPos[i]--;
        }

        else if (servoCurentPos[i] < servoMoveToPos[i])
        {
            servoCurentPos[i]++;
        }

        else
        {
            isServoStill[i] = true;
            continue;
        }
    }

    for (int i = 0; i < sizeof(servoPin) / sizeof(servoPin[0]); i++)
    {
        setServo(i, servoCurentPos[i]);
    }
}

void startupServos()
{
    for (int i = 0; i < sizeof(servoPin) / sizeof(servoPin[0]); i++)
    {
        ledcSetup(i, frequence, resolution);
        ledcAttachPin(servoPin[i], i);
    }

    for (int i = 0; i < 4; i++)
    {
        setServo(i, 120);
        delay(100);
    }

    for (int i = 4; i < 8; i++)
    {
        setServo(i, 140);
        delay(100);
    }
}