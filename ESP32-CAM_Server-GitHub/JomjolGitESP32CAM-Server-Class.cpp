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
  String str_delay = "";
  int int_delay;

  str_delay = arg("delay");
  Serial.print("delay:   "); Serial.println(str_delay);

  if (str_delay == "")
    int_delay = 2500;
  else
    int_delay = str_delay.toInt();
  
  Serial.println("ArduCAM-Server-Class - doCaptureWithLigth");
  LightOn();
  Serial.print("sleep "); Serial.println(int_delay);
  delay(int_delay);
  doCapture();
  LightOff();
}

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doCaptureWithFlashLight()
{
  String str_delay = "";
  int int_delay;

  str_delay = arg("delay");
  Serial.print("delay:   "); Serial.println(str_delay);

  if (str_delay == "")
    int_delay = 2500;
  else
    int_delay = str_delay.toInt();
  
  Serial.println("ESP32CAM-Server-Class - doCaptureWithFlashLigth");
  FlashOn();
  Serial.print("sleep "); Serial.println(int_delay);
  delay(int_delay);
  Serial.println("Start doCapture");
  doCapture();
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

  //Reset Timer
  timerWrite(timer, 0); //reset timer (feed watchdog)
  lastWatchDogReset = millis();
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

void GitESP32CAMServerLibrary::ESP32CAMServerClass::doVersion()
{
  Serial.println("Version");
  send(200, "text/plain", "Version:" __DATE__ " " __TIME__);
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
  on("/lastwatchdogreset", std::bind(&ESP32CAMServerClass::doGetLastWatchDogReset, this));
  on("/version", std::bind(&ESP32CAMServerClass::doVersion, this));
    
  GitServerLibrary::ServerClass::setup();

  // Watchdoch auf 1800s = 30 Minuten
  timer = timerBegin(0, 80, true); //timer 0, div 80
  timerAttachInterrupt(timer, &resetModule, true);
  timerAlarmWrite(timer, 1800000000, false); //set time in us
  timerAlarmEnable(timer); //enable interrupt  
  Serial.println("Watchdog enabled with 30 Minute");

}


void IRAM_ATTR GitESP32CAMServerLibrary::ESP32CAMServerClass::resetModule(){
    ets_printf("reboot\n");
    ESP.restart();;
}      


void GitESP32CAMServerLibrary::ESP32CAMServerClass::doGetLastWatchDogReset() 
{
  unsigned long vergangene_zeit;
  vergangene_zeit = millis() - lastWatchDogReset;
  vergangene_zeit = vergangene_zeit / 1000;
  String zw = "Last WatchDog reset before " + String(vergangene_zeit) + "s";
  
  send(200, "text/plain", zw);
}
