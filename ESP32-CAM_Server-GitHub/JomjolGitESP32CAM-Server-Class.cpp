#include "JomjolGitESP32CAM-Server-Class.h"


void GitESP32CAMServerLibrary::ESP32CAMServerClass::handleRoot() 
{
  String zw;
  zw = "Hello from ESP32-CAM!\n";
  zw = zw + "Flash-Light is on       GPIO" + String(flashPIN) + " - recommended illumination!\n";
  zw = zw + "WS2812b-LED Strip is on GPIO" + String(NeoPixelPIN) + " - if connected\n";
  zw = zw + "Watchdog enabled with 120s";
  
  send(200, "text/plain", zw);
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doCaptureWithLigth()
{
  Serial.println("ArduCAM-Server-Class - doCaptureWithLigth");
  LightOn();
  delay(5000);
  doCapture();
  LightOff();
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doCaptureWithFlashLight()
{
  Serial.println("ESP32CAM-Server-Class - doCaptureWithFlashLigth");
  FlashOn();
  Serial.println("sleep 5000");
  delay(5000);
  Serial.println("Start doCapture");
  this->doCapture();
  FlashOff();
}
 

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doCapture()
{
  Serial.println("ESP32CAM-Server-Class - doCapture");
  String str_resolution = "";
  framesize_t res;
  String str_quality = "";
  uint8_t quality;

  str_resolution = arg("size");
  str_resolution.toUpperCase();
  str_quality = arg("quality");
  Serial.print("size:    "); Serial.println(str_resolution);
  Serial.print("quality: "); Serial.println(str_quality);

  res = FRAMESIZE_SVGA;
  if (str_resolution == "QVGA")
    res = FRAMESIZE_QVGA;       // 320x240
  if (str_resolution == "VGA")
    res = FRAMESIZE_VGA;      // 640x480
  if (str_resolution == "SVGA")
    res = FRAMESIZE_SVGA;     // 800x600
  if (str_resolution == "XGA")
    res = FRAMESIZE_XGA;      // 1024x768
  if (str_resolution == "SXGA")
    res = FRAMESIZE_SXGA;     // 1280x1024
  if (str_resolution == "UXGA")
    res = FRAMESIZE_UXGA;     // 1600x1200

  if (str_quality == "")
    quality = 10;
  else
    quality = str_quality.toInt();
    
  if (quality > 63)
    quality = 63;
  if (quality < 0)
    quality = 0;
  
  serverCapture(quality, res);
}


void GitESP32CAMServerLibrary::ESP32CAMServerClass::doLightOn()
{
  Serial.println("ArduCAM-Server-Class - Ligth On");
  LightOn();
  send(200, "text/plain", "Light On");
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doLightOff()
{
  Serial.println("ESP32-CAM-Server-Class - Ligth Off");
  LightOff();
  send(200, "text/plain", "Light Off");
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doFlashOn()
{
  Serial.println("GPIO4-High");
  FlashOn();
  send(200, "text/plain", "Light On");
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doFlashOff()
{
  Serial.println("GPIO4-Low");
  FlashOff();
  send(200, "text/plain", "Light On");
}


void GitESP32CAMServerLibrary::ESP32CAMServerClass::setup()
{
  GitESP32CAMCommLibrary::ESP32CAMCommClass::setup();
  
  on("/capture", std::bind(&ESP32CAMServerClass::doCapture, this));
  on("/capture_with_light", std::bind(&ESP32CAMServerClass::doCaptureWithLigth, this));
  on("/capture_with_flashlight", std::bind(&ESP32CAMServerClass::doCaptureWithFlashLight, this));
  on("/lighton", std::bind(&ESP32CAMServerClass::doLightOn, this));
  on("/lightoff", std::bind(&ESP32CAMServerClass::doLightOff, this));
  on("/flashon", std::bind(&ESP32CAMServerClass::doFlashOn, this));
  on("/flashoff", std::bind(&ESP32CAMServerClass::doFlashOff, this));
  
  GitServerLibrary::ServerClass::setup();
}
