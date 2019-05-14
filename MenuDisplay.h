#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#elif defined(WIRING)
#include "Wiring.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#include <Fonts/FreeMonoBold9pt7b.h>

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#include "Menu.h"

#define MAX_DIGITS 10

class MenuDisplay {
 private:
   //Adafruit_SSD1306* display;
   SSD1306AsciiWire* display;
 public:
   //MenuDisplay(Adafruit_SSD1306* display):
   MenuDisplay(SSD1306AsciiWire* display):
   display(display) {};

   void updateDisplay(Menu* currentMenu);

   void print_num_padded(int32_t c, char base, int padded_length, char padding_character);

   void print_float(float c, int before_digits, int after_digits);
};


#endif
