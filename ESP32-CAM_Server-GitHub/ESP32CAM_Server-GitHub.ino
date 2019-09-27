#include <WiFi.h>
//#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "esp_system.h"

#include "arduino.h"
#include "JomjolGitESP32CAM-Server-Class.h"


const char* ssid = "SSID";
const char* password = "Password";
const char* host = "Kamera";

#define LEDPin      2          // Pin für Steuerung LED-Leiste 
#define INT_LED     33         // Interne LED zum Blinken bei WiFi-Connect at pin GPIO33 (ESP32-CAM).
#define FLASH_PIN   4          // Intere Flash-LED des ESP32-CAM Moduls

hw_timer_t *timer = NULL;
void IRAM_ATTR resetModule(){
    ets_printf("reboot\n");
    ESP.restart();;
}


GitESP32CAMServerLibrary::ESP32CAMServerClass ESP32CAMServer(LEDPin, 10, 50, 16, FLASH_PIN); // Pin, Anzahl LEDs, Brightness, CS, GPIO_FLASH

void setup() {
  Serial.begin(115200);
  Serial.println("Setup start");
  
  // Setup Watchdogtimer to 2 Minutes
  timer = timerBegin(0, 80, true); //timer 0, div 80
  timerAttachInterrupt(timer, &resetModule, true);
  timerAlarmWrite(timer, 120000000, false); //set time in us
  timerAlarmEnable(timer); //enable interrupt  
  Serial.println("Watchdog enabled");

  WifiReConnect();
  OTA_setup();
  
  ESP32CAMServer.setup();
  Serial.println("Setup done");
}


void loop() {
  ESP32CAMServer.loop();
  OTA_loop();
}


void WifiReConnect()
{
  pinMode(INT_LED, OUTPUT);         // Initialize the LED_BUILTIN pin as an output
  digitalWrite(INT_LED, HIGH);      // turn the LED on.

  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    for (int i=0;i<3;i++){
      digitalWrite(INT_LED, LOW);          // turn the LED on.
      delay(500);         // wait
      digitalWrite(INT_LED, HIGH);         // turn the LED off.
      delay(500);   // wait
      delay(0);                        //to prevent watchdog firing.
    }
  }
}


void OTA_setup()
{
  ArduinoOTA.setHostname(host);
  ArduinoOTA.setPassword(password);
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart(); });
  ArduinoOTA.begin();
}

void OTA_loop()
{
  ArduinoOTA.handle();
  timerWrite(timer, 0); //reset timer (feed watchdog)
}
