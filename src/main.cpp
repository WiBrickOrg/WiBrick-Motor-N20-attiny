#include "Arduino.h"
#include "../lib/wibrick-wire/slave/wiwire-slave.h"

#define myAddr 0x01
// #define myAddr 0x02
// #define myAddr 0x03

#define DEVICE_TYPE_VERSION 0x01
#define DEVICE_TYPE DEVICE_TYPE_MOTOR_N20

#define pinA PB3
#define pinB PB4

void doCommand();
void move(uint8_t dir);

WiWireSlave wiSlave;

void onData()
{
  switch (wiSlave.dataBuffer[0])
  {
  case WiWire_SCAN:
    wiSlave.clearBuffer(wiSlave.sendBuffer);
    wiSlave.sendBuffer[0] = WiWire_SCAN;
    wiSlave.sendBuffer[1] = DEVICE_TYPE;
    wiSlave.sendBuffer[2] = DEVICE_TYPE_VERSION;
    wiSlave.readyToSend = true;
    break;
  case WiWire_ACTION:
    move(wiSlave.dataBuffer[1]);
    break;
  case WiWire_STATUS:
    uint8_t status = 0;
    bool a = digitalRead(pinA);
    bool b = digitalRead(pinB);
    if (!a && !b) status = WiWire_MOTOR_STOP;
    if (!a && b) status = WiWire_MOTOR_FORWARD;
    if (a && !b) status = WiWire_MOTOR_BACKWARD;
    
    wiSlave.clearBuffer(wiSlave.sendBuffer);
    wiSlave.sendBuffer[0] = WiWire_STATUS;
    wiSlave.sendBuffer[1] = status;
    wiSlave.readyToSend = true;
    break;
  }
}

void setup()
{
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);

  wiSlave.address = myAddr;
  wiSlave.onReceive(onData);
}

void loop()
{
  wiSlave.update();
}

void move(uint8_t dir)
{
  switch (dir) {
    case WiWire_MOTOR_STOP:
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, LOW);
      break;
    case WiWire_MOTOR_FORWARD:
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);
      break;
    case WiWire_MOTOR_BACKWARD:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, LOW);
      break;
  }
}