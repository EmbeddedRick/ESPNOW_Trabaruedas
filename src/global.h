#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>
#include "EEPROM.h"


// Variables para envio y recepcion de mensajes
extern volatile boolean received;
extern String message;

// Variables y constantes relacionadas con el botón
extern int buttonStatePrevious;
extern unsigned long minButtonLongPressDuration;
extern unsigned long buttonLongPressMillis;
extern bool buttonStateLongPress;
extern const int intervalButton;
extern unsigned long previousButtonMillis;
extern unsigned long buttonPressDuration;
extern unsigned long currentMillis;


extern const int EEPROM_SIZE; // Se necesita espacio para almacenar el estado de la caja (1 byte)

extern float distance;
extern const uint16_t Adelay;

extern bool connectReceived;
extern bool waitingForDistance;

enum State
{
    STATE_SLEEP,
    STATE_OPEN_1,
    STATE_CLOSED_1,
    STATE_OPEN_2,
    STATE_CLOSED_2
};


extern State currentState;
extern bool stateSleepSent;
extern const uint32_t botoneraId; 
extern const uint32_t panelId;

#endif 
 