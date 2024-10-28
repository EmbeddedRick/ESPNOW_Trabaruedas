#ifndef RANGING_H
#define RANGING_H
#include "DW1000Ranging.h"
#include "DW1000.h"
#include "global.h"
#include "FreeRTOS.h"
#include "freertos/task.h"

//------------------UWB------------------------
#define SPI_SCK             12
#define SPI_MISO            13
#define SPI_MOSI            11
#define DW_CS               10
#define PIN_RST             8
#define PIN_IRQ             6
#define PIN_SS              10


// leftmost two bytes below will become the "short address"
// Dirección MAC del anchor
#define ANCHOR_ADD  "01:00:64:E8:33:58:12:78" // <- trabaruedas 1

// prototypes
void newRange();
void newDevice(DW1000Device *device);
void inactiveDevice(DW1000Device *device);
void init_UWB();
void stop_UWB();

void newConnectionCallback(uint32_t nodeId);
bool sendMessage(String msg);

void uwbTask(void *pvParameters);

#endif

