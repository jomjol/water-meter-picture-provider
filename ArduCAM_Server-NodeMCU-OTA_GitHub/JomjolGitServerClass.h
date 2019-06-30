#ifndef _Server_Library
#define _Server_Library

#include <ESP8266WebServer.h>

namespace GitServerLibrary
{
  class ServerClass: public ESP8266WebServer
  {
    private:
      virtual void doCommand();
	  virtual void doReset();
      virtual void handleCommand(String _param, String _value, String _modus);

      virtual void handleRoot();

    public:
      ServerClass(uint16_t port = 80) : ESP8266WebServer(port){};

      virtual void setup();
      virtual void loop();
  };
}

#endif
