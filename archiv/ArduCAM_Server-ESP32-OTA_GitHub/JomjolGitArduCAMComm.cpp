#include "JomjolGitArduCAMComm.h"


// Constructor
GitArduCAMCommLibrary::ArduCAMCommClass::ArduCAMCommClass(int _pin, int _num_led, int _brightness, int _CS, WebServer *_server)
{
//  LEDBandComm = new LEDBandCommLibrary::LEDBandCommClass(_pin, _num_led, _brightness);
  light = new Adafruit_NeoPixel(_num_led, _pin, NEO_GRBW + NEO_KHZ800);
  brightness = _brightness;


  buffer[bufferSize] = {0xFF};
  temp = 0, temp_last = 0;
  i = 0;
  is_header = false;
  CS = _CS;

  myCAM = new ArduCAM(OV2640, CS);

  server = _server;
  Serial.println("ArduCAMCommClass - Setup Done");
}



void GitArduCAMCommLibrary::ArduCAMCommClass::camCaptureMITHeader() {
  WiFiClient client = server->client();
  uint32_t len  = myCAM->read_fifo_length();
  if (len >= MAX_FIFO_SIZE) //8M
  {
    Serial.println(F("Over size."));
  }
  if (len == 0 ) //0 kb
  {
    Serial.println(F("Size is 0."));
  }
  myCAM->CS_LOW();
  myCAM->set_fifo_burst();
  if (!client.connected()) return;
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: image/jpeg\r\n";
  response += "Content-len: " + String(len) + "\r\n\r\n";
  server->sendContent(response);
  i = 0;
  while ( len-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    //Read JPEG data from FIFO
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {
      buffer[i++] = temp;  //save the last  0XD9
      //Write the remain bytes in the buffer
      if (!client.connected()) break;
      client.write(&buffer[0], i);
      is_header = false;
      i = 0;
      myCAM->CS_HIGH();
      break;
    }
    if (is_header == true)
    {
      //Write image data to buffer if not full
      if (i < bufferSize)
        buffer[i++] = temp;
      else
      {
        //Write bufferSize bytes image data to file
        if (!client.connected()) break;
        client.write(&buffer[0], bufferSize);
        i = 0;
        buffer[i++] = temp;
      }
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      buffer[i++] = temp_last;
      buffer[i++] = temp;
    }
  }
}



void GitArduCAMCommLibrary::ArduCAMCommClass::serverCaptureMITHeader() {
  myCAM->flush_fifo();
  myCAM->clear_fifo_flag();
  myCAM->start_capture();

  Serial.println(F("CAM Capturing"));
  int total_time = 0;
  total_time = millis();
  while (!myCAM->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  total_time = millis() - total_time;
  Serial.print(F("capture total_time used (in miliseconds):"));
  Serial.println(total_time, DEC);
  total_time = 0;
  Serial.println(F("CAM Capture Done."));
  total_time = millis();
  camCaptureMITHeader();
  total_time = millis() - total_time;
  Serial.print(F("send total_time used (in miliseconds):"));
  Serial.println(total_time, DEC);
  Serial.println(F("CAM send Done."));
}

void GitArduCAMCommLibrary::ArduCAMCommClass::serverCaptureWithLigth()
{
  LightOn();
  Serial.println("Licht an");

  myCAM->OV2640_set_JPEG_size(5);
  Serial.println("Groesse auf 5 gesetzt ql=5");
  delay(5000);
  serverCaptureMITHeader();
  
  LightOff();
  Serial.println("Licht aus");
}



void GitArduCAMCommLibrary::ArduCAMCommClass::setup()
{
  uint8_t vid, pid;
  uint8_t temp;
  Wire.begin();
  Serial.begin(115200);
  Serial.println(F("ArduCAM Start!"));
  // set the CS as an output:
  pinMode(CS, OUTPUT);
  // initialize SPI:
  SPI.begin();
  SPI.setFrequency(4000000); //4MHz
  //Check if the ArduCAM SPI bus is OK
  myCAM->write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM->read_reg(ARDUCHIP_TEST1);
  if (temp != 0x55) {
    Serial.println(F("SPI1 interface Error!"));
    while (1);
  }
  myCAM->wrSensorReg8_8(0xff, 0x01);
  myCAM->rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  myCAM->rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
  if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
    Serial.println(F("Can't find OV2640 module!"));
  else
    Serial.println(F("OV2640 detected."));

  myCAM->set_format(JPEG);
  myCAM->InitCAM();
  myCAM->OV2640_set_JPEG_size(OV2640_320x240);

  myCAM->clear_fifo_flag();

//  LEDBandComm->setup();								// funktioniert am Anfang nicht !!!!
  light->setBrightness(brightness);
  light->begin();
  light->show(); // Initialize all pixels to 'off'
//  colorWipe(strip.Color(0, 0, 0, 0), 0); // Aus
}


void GitArduCAMCommLibrary::ArduCAMCommClass::LightOn()
{
  Serial.println("LEDBandCommClass - LightOn");
  light->show(); // Initialize all pixels to 'off'
  colorWipe(light->Color(255, 255, 255, 255), 0); // White
}

void GitArduCAMCommLibrary::ArduCAMCommClass::LightOff()
{
	Serial.println("LEDBandCommClass - LightOff");
  colorWipe(light->Color(0, 0, 0, 0), 0); // Aus
}


void GitArduCAMCommLibrary::ArduCAMCommClass::colorWipe(uint32_t c, uint8_t wait)
{
  for(uint16_t i=0; i<light->numPixels(); i++) {
    light->setPixelColor(i, c);
    light->show();
    delay(wait);
  }
}
