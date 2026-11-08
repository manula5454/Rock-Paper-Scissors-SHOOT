#include "esp_camera.h"
#include <WebServer.h>
#include <WiFi.h>

const char* ssid = "සුළඟ වගේ";
const char* password = "diL22731";

  WiFiServer streamServer (81);

 void handleStream(WiFiClient client){

  unsigned long timeout = millis();

  while (client.connected() && !client.available()){
    if (millis() - timeout > 3000) {
      client.stop();
      return;
    }
    delay(1);
  }

  while (client.available()){
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
  }

  client.print(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
    "Cashe-Control: no-cache\r\n"
    "Connection: close\r\n\r\n"
  );

  while (client.connected()){
    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb) {
      Serial.println("Camera capture failed");
      break;
    }

    client.print("--frame\r\n");
    client.print("Content-type: image/jpeg\r\n");
    client.printf("Content-Length: %u\r\n\r\n", fb->len);

    client.write(fb->buf, fb->len);
    client.print("\r\n");

    esp_camera_fb_return(fb);
  }

  client.stop();
  Serial.println("Stream disconnected");
 }

#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 21
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 19
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 4
#define V_SYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

unsigned long lastReport = 0;
int frames = 0;

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config = {};
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
  config.pin_vsync = V_SYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000; //20MHz
  config.pixel_format = PIXFORMAT_JPEG; // no jpeg to get faster frames
  config.frame_size = FRAMESIZE_QVGA; //160 x 120 pixels
  config.jpeg_quality = 12;
  config.fb_count = 2; //2 frame buffers for overlapping captures
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.fb_location = CAMERA_FB_IN_PSRAM;

   WiFi.setSleep(false);

  if(!psramFound()) {
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  esp_err_t err = esp_camera_init(&config);
  if(err != ESP_OK){
    Serial.printf("Camera Intilisation failed: 0x%x\n", err);
    return;
  }
  Serial.println("Camera successfully started!!");

  //sensor_t *s = esp_camera_sensor_get();
  //s->set_exposure_ctrl(s, 0); // auto exposure is turned off to get faster framses
  //s->set_aec_value(s, 200);
  //s->set_gain_ctrl(s, 0);
  Serial.println("Camera Ready!!!");

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wifi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("Camera IP address: ");
  Serial.println(WiFi.localIP());

  streamServer.begin();

  Serial.println("Camera streaming server started");
  Serial.println("Open https://IP:81/stream in browser");

}

void loop() {

  WiFiClient client = streamServer.available();

  if (client) {
    Serial.println("Viewer connected!");
    handleStream(client);
  }

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    return;
  }
  esp_camera_fb_return(fb);
  frames++;

  if (millis() - lastReport >= 1000){
    Serial.printf("fps: %d\n", frames);
    frames = 0;
    lastReport = millis();
  }
}


