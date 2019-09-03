#include "JomjolGitESP32CAM-Server-Class.h"

void GitESP32CAMServerLibrary::ESP32CAMServerClass::handleCommand(String _param, String _value, String _modus)
{
  if (_param.equals("humidity"))
  {
    if (_modus.equals("name"))
      send(200, "text/plain", "tbd"); 
    else
      send(200, "text/plain", "tbd"); 
    delay(0);
    return;
  }
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doCaptureWithLigth()
{
  Serial.println("ArduCAM-Server-Class - doCaptureWithLigth");
  serverCaptureWithLigth();
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doCapture()
{
  Serial.println("ArduCAM-Server-Class - doCapture");
  serverCapture();
}


void GitESP32CAMServerLibrary::ESP32CAMServerClass::doLightOn()
{
  Serial.println("ArduCAM-Server-Class - Ligth On");
//  LEDBandComm->LightOn();
  LightOn();
  send(200, "text/plain", "Light On");
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doLightOff()
{
  Serial.println("ArduCAM-Server-Class - Ligth Off");
//  LEDBandComm->LightOff();
  LightOff();
  send(200, "text/plain", "Light Off");
}



void GitESP32CAMServerLibrary::ESP32CAMServerClass::setup()
{
  GitESP32CAMCommLibrary::ESP32CAMCommClass::setup();
  
  on("/capture", std::bind(&ESP32CAMServerClass::doCapture, this));
  on("/capture_with_light", std::bind(&ESP32CAMServerClass::doCaptureWithLigth, this));
  on("/lighton", std::bind(&ESP32CAMServerClass::doLightOn, this));
  on("/lightoff", std::bind(&ESP32CAMServerClass::doLightOff, this));
  
  GitServerLibrary::ServerClass::setup();
}
