#include <esp_now.h>
#include <WiFi.h>

uint8_t remoteMac1[] = {0xE8, 0x6B, 0xEA, 0xD0, 0xAF, 0xA0};
uint8_t remoteMac2[] = {0xE4, 0x65, 0xB8, 0x76, 0xAE, 0x50};

#define rvrx 34
#define rvry 33
#define lvrx 32
#define lvry 35
#define SWITCH_PIN1 13
#define SWITCH_PIN2 12

 typedef struct {
  int left_x;
  int left_y;
  int right_x;
  int right_y;
  bool switchState1;
  bool switchState2;
} JoystickData;

void onDataSent(const uint8_t* mac, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    //Serial.println("Sent data successfully");
  } else {
    //Serial.println("Failed to send data");
  }
}

void setup() {
  pinMode(rvrx, INPUT);
  pinMode(rvry, INPUT);
  pinMode(lvrx, INPUT);
  pinMode(lvry, INPUT);
  pinMode(SWITCH_PIN1, INPUT_PULLUP);
  pinMode(SWITCH_PIN2, INPUT_PULLUP);


  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo1 = {};
  memcpy(peerInfo1.peer_addr, remoteMac1, 6);
  peerInfo1.channel = 0;
  peerInfo1.encrypt = false;

  if (esp_now_add_peer(&peerInfo1) != ESP_OK) {
   // Serial.println("Failed to add peer");
    return;
  }


esp_now_peer_info_t peerInfo2 = {};
  memcpy(peerInfo2.peer_addr, remoteMac2, 6);
  peerInfo2.channel = 0;
  peerInfo2.encrypt = false;

  if (esp_now_add_peer(&peerInfo2) != ESP_OK) {
   // Serial.println("Failed to add peer");
    return;
  }

}

void loop() {
  JoystickData data;

  data.left_x = analogRead(lvrx);
  data.left_y = analogRead(lvry);
  data.right_x = analogRead(rvrx);
  data.right_y = analogRead(rvry);
  data.switchState1 = !digitalRead(SWITCH_PIN1);  
  data.switchState2 = !digitalRead(SWITCH_PIN2);  

  Serial.printf("Left X: %d, Left Y: %d, Right X: %d, Right Y: %d, Switch1: %d,Switch2: %d \n", data.left_x, data.left_y, data.right_x, data.right_y,data.switchState1,data.switchState2);

  esp_err_t result1 = esp_now_send(remoteMac1, (uint8_t*)&data, sizeof(data));
  esp_err_t result2 = esp_now_send(remoteMac2, (uint8_t*)&data, sizeof(data));

  //if (result == ESP_OK) {
   // Serial.println("Data sent successfully");
  //} else {
    //Serial.println("Failed to send data");
  //}

  delay(50);
}
