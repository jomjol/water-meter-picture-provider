#ifndef _Server_Library
#define _Server_Library

#include <WebServer.h>

namespace GitServerLibrary
{
  class ServerClass: public WebServer
  {
    private:
      virtual void doCommand();
	    virtual void doReset();
      virtual void handleCommand(String _param, String _value, String _modus);

      virtual void handleRoot();

    public:
      ServerClass(uint16_t port = 80) : WebServer(port){};

      virtual void setup();
      virtual void loop();
  };
}

#endif
