#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#include "arduino.h"
#include "JomjolGitArduCAM-Server-Class.h"

const char* ssid = "WLAN-SSID";
const char* password = "WLAN-PASSWORD";
const char* host = "Wasser";

GitArduCAMServerLibrary::ArduCAMServerClass ArduCAMServer(D3, 10, 50, 16); // Pin, Anzahl LEDs, Brightness, CS

void setup() {
  Serial.begin(115200);
  Serial.println("gestartet - setup");
  WifiReConnect();
  OTA_setup();

  
  ArduCAMServer.setup();
}


void loop() {
  ArduCAMServer.loop();
  OTA_loop();
}


void WifiReConnect()
{
  #define INT_LED     2         // Led in NodeMCU at pin GPIO16 (D0).
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
}
