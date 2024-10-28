#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "ranging.h"
#include "driver/rtc_io.h"
#include "powerManager.h"
// C libaries
#include <string.h>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* strtol */

#define SENSOR_WAKEUP GPIO_NUM_7
bool boxState = false; // Caja cerrada
int pulses = 0;
// Direccion de la caja LOTO
#define LOTO          35 
const int freq =      400;
const int ledChannel = 0;
const int resolution = 8; //Resolution 8, 10, 12, 15

typedef struct struct_message {
    char a[8];
} struct_message;

// Create a struct_message called myData
struct_message myData;

void ChangeBoxState(){
  if(!boxState){
    Serial.println("LOTO box openned"); 
    // Abrir Caja LOTO
    ledcWrite(ledChannel, 255*18/100); // -> 90°
  }else{
    Serial.println("LOTO box closed"); 
    //Cerrar Caja LOTO
    ledcWrite(ledChannel, 255*52/100); // -> 0°
  }
  boxState = !boxState;
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  if(strcmp(myData.a, "Change")==0){
    ChangeBoxState();
    pulses++;
  }else if(strcmp(myData.a, "Release")==0){
    pulses++;
    if(pulses >= 4){
      if(boxState){ // Si la caja LOTO se encuentra abierta, se cierra antes de entrar en DeepSleep
        Serial.println("Cerrando Caja LOTO");
        ChangeBoxState();
      }
      ledcWrite(ledChannel, 255*52/100); // -> 0°
      Serial.println("Iniciando la entrada a DeepSleep");
      enterDeepSleep();
    }
  }
}

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Desactiva el detector de brownout

  ledcSetup(ledChannel, freq, resolution);
  //attach the channel to the GPIO2 to be controlled
  ledcAttachPin(LOTO, ledChannel);
  ledcWrite(ledChannel, 255*52/100); // -> 90°
  // Deep Sleep Mode configuration
  rtc_gpio_pullup_en(SENSOR_WAKEUP);
  esp_sleep_enable_ext0_wakeup(SENSOR_WAKEUP, 0);

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0){
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    Serial.println("Init complete");
    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
    Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
    readMacAddress();

    // Create UWB task on Core 0
      xTaskCreatePinnedToCore(
          uwbTask,    // Task function
          "UWB Task", // Name of the task
          15000,      // Stack size (in words)
          NULL,       // Task input parameter
          2,          // Priority of the task
          NULL,       // Task handle
          1);         // Core to run the task on (0)
  }else
    {
        Serial.println("Iniciando en modo deep sleep...");
        delay(5000);
        enterDeepSleep();
    }
}

void loop() {
  // put your main code here, to run repeatedly:
}
