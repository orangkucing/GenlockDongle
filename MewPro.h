// Arduino pins
// Assignment of these pins (except I2C's SCL/SDA) can be re-configured here.
//
#if defined(__AVR_ATmega328P__)
//                           0;  //        PD0 (Used by serial port)
//                           1;  //        PD1 (Used by serial port)
const int SHUTTER_PIN      = 2;  //        PD2 interrupt pin w/o software debounce
//                           3;  //        PD3 (Not in use)
//                           4;  //        PD4 (Not in use)
//                           5;  //        PD5 (Not in use)
//                           6;  //        PD6 (Not in use)
//                           7;  //        PD7 (Not in use)
//                           8;  //        PB0 (Not in use)
//                           9;  //        PB1 (Not in use)
//                           10; // (SS)   PB2 (Not in use)
//                           11; // (MOSI) PB3 (Not in use)
//                           12; // (MISO) PB4 (Not in use)
const int BUILTINLED       = 13; // (SCK)  PB5 built-in LED
//                               //        PB6 XTAL1
//                               //        PB7 XTAL2
const int HBUSRDY          = A0; // (14)   PC0
const int PWRBTN           = A1; // (15)   PC1
const int I2CINT           = A2; // (16)   PC2 used by pin change int
const int BPRDY            = A3; // (17)   PC3
//                           A4; // (18)   PC4 (Used by I2C SDA)
//                           A5; // (19)   PC5 (Used by I2C SCL) 
//                           A6; // (20)       Analog only
//                           A7; // (21)       Analog only
#else
#error CPU not supported
#endif
