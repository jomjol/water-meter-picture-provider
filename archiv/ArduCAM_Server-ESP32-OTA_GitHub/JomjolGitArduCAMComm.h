#ifndef _ArduCAM_Comm_Library
#define _ArduCAM_Comm_Library

#include <WiFiUdp.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
//#include <ESP8266mDNS.h>
#include <Wire.h>
#include "ArduCAM.h"
#include <SPI.h>
#include "memorysaver.h"

#include <Adafruit_NeoPixel.h>



const size_t bufferSize = 4096;

namespace GitArduCAMCommLibrary
{
  class ArduCAMCommClass
  {
    private:

      uint8_t buffer[bufferSize] = {0xFF};
      uint8_t temp, temp_last;
      int i;
      bool is_header;
      int CS;

      WebServer *server;
      ArduCAM *myCAM;

      void serverCaptureMITHeader();
      void camCaptureMITHeader();
	  
	  
      Adafruit_NeoPixel *light; 
      int brightness;
      void colorWipe(uint32_t c, uint8_t wait);

	  

    public:
      ArduCAMCommClass(int _pin, int _num_led, int _brightness, int _CS, WebServer *_server);
      virtual void setup();
      
      void serverCaptureWithLigth();
	  
      void LightOn();
      void LightOff();
      
  };
}
#endif
