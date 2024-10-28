
#include "ranging.h"

void newRange()
{
    distance = DW1000Ranging.getDistantDevice()->getRange();
    Serial.printf("Distancia medida: %.2f metros\n", distance);
}

void init_UWB()
{
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);
    DW1000.setAntennaDelay(Adelay);
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.startAsAnchor(ANCHOR_ADD, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
}



void stop_UWB()
{
    detachInterrupt(digitalPinToInterrupt(PIN_IRQ));
    SPI.end();
    Serial.println("UWB desconectado.");
}


void uwbTask(void *pvParameters)
{
    init_UWB();
    while (true)
    {
        DW1000Ranging.loop();
        vTaskDelay(1 / portTICK_PERIOD_MS); // Yield to other tasks
    }
}



