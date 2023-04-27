#include "webServer.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <WiFi.h>

unsigned long lastDelay;
unsigned long lastWalk;
unsigned long curMills;

int buttonPin = 0;

void setup()
{
    WiFi.begin("HFR Tower", "21471131");

    pinMode(buttonPin, INPUT_PULLUP);

    startupServos();
    startWebPages();

    MDNS.begin("robot");
}

void loop()
{
    curMills = millis();

    if (!digitalRead(buttonPin))
    {
        lower();
    }

    if (curMills - lastDelay >= speed)
    {
        lastDelay = curMills;

        if (isWalking)
        {
            Walking();
        }
        moveServo1Step();
    }
}