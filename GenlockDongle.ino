// GoPro hardware emu (MewPro Genlock Dongle)
//
// To comple please use Arduino IDE 1.5.7 (or later) and PinChangeInt 2.40-rc2 (or later)

//   Copyright (c) 2015 orangkucing

// enable console output
boolean debug = true;

//********************************************************
// c_I2C: I2C interface
//
// Note: in order to use MewPro Genlock Dongle reliably, THE FOLLOWING MODIFICATIONS TO STANDARD ARDUINO LIBRARY SOURCE IS
// STRONGLY RECOMMENDED:
//
//     1. hardware/arduino/avr/libraries/Wire/Wire.h
//            old: #define BUFFER_LENGTH 32                        -->   new: #define BUFFER_LENGTH 64
//     2. hardware/arduino/avr/libraries/Wire/utility/twi.h
//                 #define TWI_BUFFER_LENGTH 32                    -->   new: #define TWI_BUFFER_LENGTH 64
#include <Wire.h>
#if BUFFER_LENGTH < 64
#error Please modify Arduino Wire library source code to increase the I2C buffer size
#endif

//********************************************************
// e_Shutter: A shutter without contact bounce or chatter
#define USE_SHUTTER

//********************************************************
// pin change interrupt
// please download PinChangeInt library from https://github.com/GreyGnome/PinChangeInt
//
// You can reduce the memory footprint of this handler by declaring that there will be no pin change interrupts
// on any one or two of the three ports.  If only a single port remains, the handler will be declared inline
// reducing the size and latency of the handler.
#define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
// #define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
#define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
// *** New in version 1.5: ********************************
// You can reduce the code size by 20-50 bytes, and you can speed up the interrupt routine
// slightly by declaring that you don't care if the static variables PCintPort::pinState and/or
// PCintPort::arduinoPin are set and made available to your interrupt routine.
#define NO_PIN_STATE        // to indicate that you don't need the pinState
#define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin
// *********************************************************
// if there is only one PCInt vector in use the code can be inlined
// reducing latency and code size
// define DISABLE_PCINT_MULTI_SERVICE below to limit the handler to servicing a single interrupt per invocation.
#define DISABLE_PCINT_MULTI_SERVICE
#include <PinChangeInt.h>

// it is better to define this when RXI is connected to nothing (eg. Dongle #1 of single dongle configuration or Dongle #0)
#undef  UART_RECEIVER_DISABLE

// end of Options
//////////////////////////////////////////////////////////

#include <string.h>
#include <Arduino.h>
#include "MewPro.h"

volatile boolean poweron = false;
boolean dontSendPW = false;
boolean heartBeatIsOn = false;
boolean isMaster = false;

unsigned long previous_shutter;
unsigned long timelapse = 0;

void setup() 
{
  // Remark. Arduino Pro Mini 328 3.3V 8MHz is too slow to catch up with the highest 115200 baud.
  //     cf. http://forum.arduino.cc/index.php?topic=54623.0
  // Set 57600 baud or slower.
  Serial.begin(57600);
#ifdef UART_RECEIVER_DISABLE
  UCSR0B &= (~_BV(RXEN0));
#endif

  pinMode(BPRDY, INPUT_PULLUP);
  pinMode(PWRBTN, INPUT_PULLUP);
  digitalWrite(HBUSRDY, LOW);
  pinMode(HBUSRDY, OUTPUT);
  pinMode(BUILTINLED, OUTPUT);
  digitalWrite(BUILTINLED, HIGH);
  
  setupShutter();
}

void loop() 
{
  static unsigned long lastHeartBeat = 0;
  static boolean pwrbtn = true;
  
  if (digitalRead(BPRDY) == LOW && digitalRead(PWRBTN) == LOW) { // slave: power button of main camera depressed
    pwrbtn = false;
    delayMicroseconds(10500);
    while (digitalRead(PWRBTN) == LOW) {
      ;
    }
  } else if (!pwrbtn) {
    emptyQueue();
    pwrbtn = true;
    poweron = true;
    dontSendPW = true;
    if (1) {
      Serial.println("");
      Serial.println("@");  // power on slaves
    }
  }
  if (poweron) {
    digitalWrite(BUILTINLED, LOW);
    digitalWrite(HBUSRDY, HIGH);
    resetI2C();
    poweron = false;
  }
  if (heartBeatIsOn) {
    if (millis() - lastHeartBeat >= 1000) {
      SendHeartBeat();
      lastHeartBeat += 1000;
    }
  } else {
    lastHeartBeat = millis();
  }
  if (timelapse != 0) {
    if (millis() - previous_shutter >= timelapse) {
      queueIn("SR3");
      timelapse = 0;
    }
  }
  checkCommands();
}

