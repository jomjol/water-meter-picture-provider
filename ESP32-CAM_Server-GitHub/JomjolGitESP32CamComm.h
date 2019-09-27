#ifndef _ESP32CAM_Comm_Library
#define _ESP32CAM_Comm_Library

#include <WebServer.h>
#include "esp_camera.h"

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

#include <Adafruit_NeoPixel.h>

typedef struct {
        WiFiClient client;
        size_t len;
} jpg_chunking_t;


namespace GitESP32CAMCommLibrary
{
  class ESP32CAMCommClass
  {
    private:
      WebServer *server;

      void serverCaptureMITHeader();
      void camCaptureMITHeader();
      
      static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len);
	  
      Adafruit_NeoPixel *light; 
      int brightness;
      void colorWipe(uint32_t c, uint8_t wait);

      framesize_t fr_size;
      uint8_t fr_quality;

    public:
      int flashPIN;
      int NeoPixelPIN;

      ESP32CAMCommClass(int _pin, int _num_led, int _brightness, int _CS, int _flashPIN, WebServer *_server);
      virtual void setup();
      
      void serverCaptureWithLigth();
      void serverCaptureWithFlashLight();
      void serverCapture(uint8_t _quality = 10, framesize_t _size = FRAMESIZE_SVGA);
	  
      void LightOn();
      void LightOff();

      void FlashOn();
      void FlashOff();
      
  };
}
#endif
