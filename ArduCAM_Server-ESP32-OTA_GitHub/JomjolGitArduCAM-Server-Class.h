#include "JomjolGitServerClass.h"
#include "JomjolGitArduCAMComm.h"

#ifndef _ArduCAM_Server_Library
#define _ArduCAM_Server_Library

namespace GitArduCAMServerLibrary
{

  class ArduCAMServerClass: public GitArduCAMCommLibrary::ArduCAMCommClass, public GitServerLibrary::ServerClass
  {
    private:
      void handleCommand(String _param, String _value, String _modus);
      void doCaptureWithLigth();
      void doLightOn();
      void doLightOff();


    public:
      ArduCAMServerClass(int _pin, int _num_led, int _brightness, int _CS) : GitArduCAMCommLibrary::ArduCAMCommClass(_pin, _num_led, _brightness, _CS, this), GitServerLibrary::ServerClass() {} ;

      void setup();
  };
}

#endif
