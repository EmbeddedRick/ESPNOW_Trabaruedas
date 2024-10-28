

#include "powerManager.h"
#include <Arduino.h>

void enterDeepSleep()
{
    Serial.flush();
    Serial.println("Entrando a modo deep sleep...");
    delay(500);
    esp_deep_sleep_start();
}




