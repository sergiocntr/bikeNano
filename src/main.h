#ifndef __main_h
#define __main_h
#include <Arduino.h>
#include <TinyGPS.h>
TinyGPS gps;
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
const byte Pin_433  = 7;  // Pin da collegare al trasmettitore
const byte Pin_SW  = 2;    // Pin del bottone sul Joystick
const byte Pin_LED  = 13;  // Pin del LED L di Arduino
byte statoBottone; 
const uint16_t GPSBaud = 9600;
void smartDelay(uint16_t ms);
void gate_interrupt_handler();
void send433(uint8_t cosa);
uint16_t readVcc();
static void print_date(TinyGPS &gps);
#endif