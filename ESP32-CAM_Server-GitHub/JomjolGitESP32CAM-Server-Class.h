#include "JomjolGitServerClass.h"
#include "JomjolGitESP32CamComm.h"

#ifndef _ESP32CAM_Server_Library
#define _ESP32CAM_Server_Library

namespace GitESP32CAMServerLibrary
{

  class ESP32CAMServerClass: public GitESP32CAMCommLibrary::ESP32CAMCommClass, public GitServerLibrary::ServerClass
  {
    private:
      void doCaptureWithLigth();
      void doCaptureWithFlashLight();
      void doCapture();
      void doLightOn();
      void doLightOff();
      void doFlashOn();
      void doFlashOff();
      
      void doGetLastWatchDogReset();
            
      void handleRoot(); 
      
    public:
      ESP32CAMServerClass(int _pin, int _num_led, int _brightness, int _CS, int _flashPIN) : GitESP32CAMCommLibrary::ESP32CAMCommClass(_pin, _num_led, _brightness, _CS, _flashPIN, this), GitServerLibrary::ServerClass() {} ;

      void setup();

      hw_timer_t *timer;
      unsigned long lastWatchDogReset;

      static void IRAM_ATTR resetModule();
  };
}

#endif
