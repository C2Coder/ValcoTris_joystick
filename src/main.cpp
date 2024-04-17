#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Test Valec MAC = 58:BF:25:82:76:08

// Valec MAC    = C8:2B:96:B5:CA:84
// Joystick MAC = C8:2B:96:B8:C9:C0


#define x_joy_pin 34
#define y_joy_pin 35
#define z_joy_pin 32

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t valec_addr[] = {0xC8,0x2B,0x96,0xB5,0xCA,0x84};

typedef struct data_struct {
  int x;
  int y;
  int z;
} data_struct;

data_struct data;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  /*
  char macStr[18];
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  */
}
 
void setup() {
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);
  WiFi.macAddress();
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
   
  // register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, valec_addr, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  data.x = analogRead(x_joy_pin);
  data.y = analogRead(y_joy_pin);
  data.z = analogRead(z_joy_pin);

  // data.x = random(0, 20);
  // data.y = random(0, 20);
  // data.z = random(0, 20);
 
  esp_err_t result = esp_now_send(0, (uint8_t *) &data, sizeof(data_struct));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(50); // 20 Hz
}