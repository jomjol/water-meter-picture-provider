#include "JomjolGitESP32CAMComm.h"


// Constructor
GitESP32CAMCommLibrary::ESP32CAMCommClass::ESP32CAMCommClass(int _pin, int _num_led, int _brightness, int _CS, int _flashPIN, int _flashChannel, int _flashFreq, int _flashResolution, WebServer *_server)
{
  light = new Adafruit_NeoPixel(_num_led, _pin, NEO_GRBW + NEO_KHZ800);
  brightness = _brightness;
  flashPIN = _flashPIN;
  NeoPixelPIN = _pin;
  // for ledc module
  flashChannel = _flashChannel;
  flashFreq = _flashFreq;
  flashResolution = _flashResolution;

  pinMode(flashPIN, OUTPUT);         // Initialize the LED_BUILTIN pin as an output

  server = _server;
  Serial.println("ESP32CAMCommClass - Setup Done");
}

size_t GitESP32CAMCommLibrary::ESP32CAMCommClass::jpg_encode_stream(void * arg, size_t index, const void* data, size_t len){
    jpg_chunking_t *j = (jpg_chunking_t *)arg;

    if(!index){
        j->len = 0;
    }
    if(j->client.write((const char *)data, len) != ESP_OK){
        return 0;
    }
    j->len += len;
    return len;
}

void GitESP32CAMCommLibrary::ESP32CAMCommClass::camCaptureMITHeader() {
    WiFiClient client = server->client();
    if (!client.connected()) return;
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: image/jpeg\r\n";
    response += "Content-Disposition: inline; filename=capture.jpg\r\n\r\n";
    server->sendContent(response);

    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    int64_t fr_start = esp_timer_get_time();

    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    uint8_t * out_buf;
    bool s;
    bool detected = false;
    int face_id = 0;
  	size_t fb_len = 0;

    if(fb->format == PIXFORMAT_JPEG){
      fb_len = fb->len;
      Serial.println("Pixformat = jpeg");
      client.write((const char *)fb->buf, fb->len);
    }
    else {
      Serial.println("Pixformat geht in else");
      jpg_chunking_t jchunk = {client, 0};
      res = frame2jpg_cb(fb, 80, this->jpg_encode_stream, &jchunk)?ESP_OK:ESP_FAIL;
      fb_len = jchunk.len;
    }

  	esp_camera_fb_return(fb);
	  int64_t fr_end = esp_timer_get_time();
  	Serial.printf("JPG: %uB %ums\n", (uint32_t)(fb_len), (uint32_t)((fr_end - fr_start)/1000));
}

void GitESP32CAMCommLibrary::ESP32CAMCommClass::serverCaptureMITHeader()
{
    Serial.println(F("CAM Capturing"));
    int total_time = 0;
    total_time = millis();
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

void GitESP32CAMCommLibrary::ESP32CAMCommClass::serverCaptureWithFlashLight()
{
    FlashOn((1 << flashResolution) - 1);
    Serial.println("Flash an");
    delay(5000);
    serverCapture();
    FlashOff();
    Serial.println("Flash aus");
}

void GitESP32CAMCommLibrary::ESP32CAMCommClass::serverCaptureWithLigth()
{
    LightOn();
    Serial.println("Licht an");
    delay(5000);
    serverCapture();
    LightOff();
    Serial.println("Licht aus");
}

void GitESP32CAMCommLibrary::ESP32CAMCommClass::serverCapture(uint8_t _quality, framesize_t _size)
{
    sensor_t * s = esp_camera_sensor_get();
    if (fr_quality != _quality)
    {
      fr_quality = _quality;
      s->set_quality(s, fr_quality);
    }

    if (fr_size != _size)
    {
      fr_size = _size;
      s->set_framesize(s, fr_size);
    }
    serverCaptureMITHeader();
}


void GitESP32CAMCommLibrary::ESP32CAMCommClass::setup()
{
    Serial.println("LEDC setup");
    ledcSetup(flashChannel, flashFreq, flashResolution);
    ledcAttachPin(flashPIN, flashChannel);

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    if (psramFound())
    {
      config.frame_size = FRAMESIZE_UXGA;
      config.jpeg_quality = 10;
      config.fb_count = 2;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.jpeg_quality = 12;
      config.fb_count = 1;
    }

#if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
#endif

	  // camera init
	  esp_err_t err = esp_camera_init(&config);
	  if (err != ESP_OK) {
  		Serial.printf("Camera init failed with error 0x%x", err);
  		return;
	  }

	  sensor_t * s = esp_camera_sensor_get();
	  //initial sensors are flipped vertically and colors are a bit saturated
	  if (s->id.PID == OV3660_PID) {
      s->set_vflip(s, 1);       //flip it back
      s->set_brightness(s, 1);  //up the blightness just a bit
      s->set_saturation(s, -2); //lower the saturation
    }
	  //drop down frame size for higher initial frame rate
    fr_size = FRAMESIZE_SVGA;
    s->set_framesize(s, fr_size);
    fr_quality = 10;
    s->set_quality(s, fr_quality);
    s->set_lenc(s, 1);

  	#if defined(CAMERA_MODEL_M5STACK_WIDE)
  	  s->set_vflip(s, 1);
  	  s->set_hmirror(s, 1);
	  #endif

    light->setBrightness(brightness);
    light->begin();
    light->show(); // Initialize all pixels to 'off'
}

void GitESP32CAMCommLibrary::ESP32CAMCommClass::FlashOn(int intensity)
{
  Serial.println("FlashOn");
  int max = (1 << flashResolution) - 1;
  if (intensity > max)
  {
    intensity = max;
  }
  ledcWrite(flashChannel, intensity);
}

void GitESP32CAMCommLibrary::ESP32CAMCommClass::FlashOff()
{
  Serial.println("FlashOff");
  ledcWrite(flashChannel, LOW);
}

void GitESP32CAMCommLibrary::ESP32CAMCommClass::LightOn()
{
  Serial.println("LEDBandCommClass - LightOn");
  light->show(); // Initialize all pixels to 'off'
  colorWipe(light->Color(255, 255, 255, 255), 0); // White
}

void GitESP32CAMCommLibrary::ESP32CAMCommClass::LightOff()
{
  Serial.println("LEDBandCommClass - LightOff");
  colorWipe(light->Color(0, 0, 0, 0), 0); // Aus
}


void GitESP32CAMCommLibrary::ESP32CAMCommClass::colorWipe(uint32_t c, uint8_t wait)
{
  for(uint16_t i=0; i<light->numPixels(); i++) {
    light->setPixelColor(i, c);
    light->show();
    delay(wait);
  }
}
