#ifndef _Server_Library
#define _Server_Library

#include <WebServer.h>

namespace GitServerLibrary
{
  class ServerClass: public WebServer
  {
    private:
      virtual void doCommand();
      virtual void doGetRuntime();
	    virtual void doReset();
      virtual void handleCommand(String _param, String _value, String _modus);

      virtual void handleRoot();

    public:
      ServerClass(uint16_t _port = 80) : WebServer(_port){startzeit = millis();};

      unsigned long startzeit;

      virtual void setup();
      virtual void loop();
  };
}

#endif
