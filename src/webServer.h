#ifndef webFunction_H
#define webFunction_H

#include "LittleFS.h"
#include "robot.h"
#include <Arduino.h>
#include <AsyncElegantOTA.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

String argName;
String argValue1;
String argValue2;

const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";
const char *PARAM_INPUT_3 = "state2";

//===web page list========================================================================
const char *pageUrl[] = {"/", "/script.js", "/style.css", "/icon.png"};
const char *pageFileName[] = {"/index.html", "/script.js", "/style.css", "/icon.png"};
const char *pageType[] = {"text/html", "text/javascript", "text/css", "image/png"};

//===Web update Data======================================================================================

static void updateData(AsyncWebServerRequest *request)
{
    request->send(200, "text/plain", "OK");

    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2) && request->hasParam(PARAM_INPUT_3))
    {
        argName = request->getParam(PARAM_INPUT_1)->value();
        argValue1 = request->getParam(PARAM_INPUT_2)->value();
        argValue2 = request->getParam(PARAM_INPUT_3)->value();

        if (argName == "servo")
        {
            servoMoveToPos[argValue1.toInt()] = argValue2.toInt();
        }

        else if (argName == "custom")
        {
            customBtnArry[argValue1.toInt()]();
        }

        else if (argName == "speed")
        {
            speed = map(argValue2.toInt(), 1, 20, 20, 1);
        }
    }
}

void startWebPages()
{
    AsyncElegantOTA.begin(&server); // Start AsyncElegantOTA
    server.begin();
    LittleFS.begin();

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*"); // enable CORS

    server.on("/updateData", HTTP_GET, updateData);

    for (int i = 0; i < sizeof(pageUrl) / sizeof(pageUrl[0]); i++)
    {
        server.on(pageUrl[i], HTTP_GET, [i](AsyncWebServerRequest *request)
                  { request->send(LittleFS, pageFileName[i], pageType[i]); });
    }
}
#endif