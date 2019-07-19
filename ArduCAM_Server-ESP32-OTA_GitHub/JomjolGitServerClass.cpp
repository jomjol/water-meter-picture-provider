#include "JomjolGitServerClass.h"


void GitServerLibrary::ServerClass::setup()
{
  on("/", std::bind(&ServerClass::handleRoot, this));
  on("/command.html", std::bind(&ServerClass::doCommand, this));
  on("/reset", std::bind(&ServerClass::doReset, this));
  
  begin();
}

void GitServerLibrary::ServerClass::handleRoot() 
{
  send(200, "text/plain", "hello from esp8266!");
}


void GitServerLibrary::ServerClass::loop()
{
  handleClient();
}

void GitServerLibrary::ServerClass::doReset()
{
  String message = "Reset in 10 Sekunden";
  send(200, "text/plain", message); 

  for (int i = 0; i < 10; ++i)
  {
	delay(1000);
	delay (0);
  }
  ESP.restart();
}


void GitServerLibrary::ServerClass::doCommand()
{
  String param = "";
  String param_value = "";
  String modus = "";

  param = arg("param");
  param_value = arg("value");
  modus = arg("modus");

  handleCommand(param, param_value, modus);
}


void GitServerLibrary::ServerClass::handleCommand(String _param, String _value, String _modus)
{
  String message = "";

  message+="param = " + _param + "\n";
  message+="value = " + _value + "\n";
  message+="modus = " + _modus + "\n";
  send(200, "text/plain", message); 
}
