#include "JomjolGitServerClass.h"
#include "JomjolGitESP32CamComm.h"

#ifndef _ESP32CAM_Server_Library
#define _ESP32CAM_Server_Library

namespace GitESP32CAMServerLibrary
{

  class ESP32CAMServerClass: public GitESP32CAMCommLibrary::ESP32CAMCommClass, public GitServerLibrary::ServerClass
  {
    private:
      void handleCommand(String _param, String _value, String _modus);
      void doCaptureWithLigth();
      void doCapture();
      void doLightOn();
      void doLightOff();


    public:
      ESP32CAMServerClass(int _pin, int _num_led, int _brightness, int _CS) : GitESP32CAMCommLibrary::ESP32CAMCommClass(_pin, _num_led, _brightness, _CS, this), GitServerLibrary::ServerClass() {} ;

      void setup();
  };
}

#endif
