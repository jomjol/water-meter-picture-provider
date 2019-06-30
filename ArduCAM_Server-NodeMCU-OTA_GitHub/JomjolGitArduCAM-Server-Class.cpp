#include "JomjolGitArduCAM-Server-Class.h"

void GitArduCAMServerLibrary::ArduCAMServerClass::handleCommand(String _param, String _value, String _modus)
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

void GitArduCAMServerLibrary::ArduCAMServerClass::doCaptureWithLigth()
{
  Serial.println("ArduCAM-Server-Class - doCaptureWithLigth");
  serverCaptureWithLigth();
}

void GitArduCAMServerLibrary::ArduCAMServerClass::doLightOn()
{
  Serial.println("ArduCAM-Server-Class - Ligth On");
//  LEDBandComm->LightOn();
  LightOn();
  send(200, "text/plain", "Light On");
}

void GitArduCAMServerLibrary::ArduCAMServerClass::doLightOff()
{
  Serial.println("ArduCAM-Server-Class - Ligth Off");
//  LEDBandComm->LightOff();
  LightOff();
  send(200, "text/plain", "Light Off");
}



void GitArduCAMServerLibrary::ArduCAMServerClass::setup()
{
  GitArduCAMCommLibrary::ArduCAMCommClass::setup();
  
  on("/capture_with_light", std::bind(&ArduCAMServerClass::doCaptureWithLigth, this));
  on("/lighton", std::bind(&ArduCAMServerClass::doLightOn, this));
  on("/lightoff", std::bind(&ArduCAMServerClass::doLightOff, this));
  
  GitServerLibrary::ServerClass::setup();
}
