#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <ArduinoOTA.h>
#include "esp_system.h"


// #include "arduino.h"
#include "JomjolGitESP32CAM-Server-Class.h"

const char* ssid =     "SSID";
const char* password = "PASSWORD";
const char* updatepassword = "UPDATE";
const char* host = "wasseruhr";
// FIX IP siehe unten

#define LEDPin      2          // Pin für Steuerung LED-Leiste 
#define INT_LED     33         // Interne LED zum Blinken bei WiFi-Connect at pin GPIO33 (ESP32-CAM).
#define FLASH_PIN   4          // Intere Flash-LED des ESP32-CAM Moduls


GitESP32CAMServerLibrary::ESP32CAMServerClass ESP32CAMServer(LEDPin, 10, 50, 16, FLASH_PIN); // Pin, Anzahl LEDs, Brightness, CS, GPIO_FLASH

void setup() {
  Serial.begin(115200);
  Serial.println("Setup start");
  
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
      Serial.print("Attempting to connect to WPA network, SSID: ");
      Serial.println(ssid);
      WiFi.begin(ssid, password);
  
      // wait 5 seconds for connection:
      delay(5000);
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    
//    FIX IP?
//    IPAddress ip(192,168,1,50);   
//    IPAddress gateway(192,168,1,100);   
//    IPAddress subnet(255,255,255,0);   
//    WiFi.config(ip, gateway, subnet);
    
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(host)) {
      Serial.print("MDNS responder started - Host: "); Serial.println(host);
    }

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
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    
  ArduinoOTA.setPassword(updatepassword);
  ArduinoOTA.begin();}

void OTA_loop()
{
  ArduinoOTA.handle();
}
