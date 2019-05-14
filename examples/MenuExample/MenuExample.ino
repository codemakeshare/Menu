#include <Menu.h>
#include <MenuDisplay.h>
#include <ButtonPress.h>
#include <parameters.h>
#include <AnalogKnob.h>

// ----- Hardware setup -------

// Defining the buttons
#define B_UP   5
#define B_DOWN 7
#define B_LEFT 6
#define B_RIGHT 2

// Defining the Display 
#define I2C_ADDRESS 0x3C
SSD1306AsciiWire display;   //(Text mode)
 
// De-bounced button event handlers with 300ms de-bounce blanking time-out
ButtonPress upButton = ButtonPress(B_UP, 300);
ButtonPress downButton = ButtonPress(B_DOWN, 300);
ButtonPress leftButton = ButtonPress(B_LEFT, 300);
ButtonPress rightButton = ButtonPress(B_RIGHT, 300);

/** Event handler mapping button pushes to menu events
 */
menu_event_t buttonEvent() {
  if (upButton.pushed()) return MENU_UP;
  if (downButton.pushed()) return MENU_DOWN;
  if (leftButton.pushed()) return MENU_LEAVE;
  if (rightButton.pushed()) return MENU_SELECT;
  return NONE;
}

// handler for the potentiometer (on input A0, with a deadzone of 5 ticks)
AnalogKnob knob = AnalogKnob(0, 5);

/** Callback for LED parameter
 */
void updateLED(ParameterInt16* param) {
    pinMode(LED_BUILTIN, OUTPUT);
    if (param->getValue()>0) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

/** an action that uses the display and runs until user clicks "back" button
 */
bool updateActionItem(void* argument) {
  int counter = 0;
  int pos=50;
  int dir=1;
  display.clear();
  while (true) {
    if (buttonEvent()==MENU_LEAVE) return false;
    
    display.setCursor(pos,2);
    display.println(" O ");
    display.setCursor(10,6*2);
    display.print(counter);
    display.clearToEOL();
    counter++;
    pos+=dir;
    if ((pos>110) || (pos<1)) dir *= -1;
  }
  return false;
}

// ------------ Defining the menu structure ---------------

// Value parameters, with callback (updateLED)
ParameterInt16 pVal1("Value 0-10", 0, 0, 10, 1);
ParameterInt16 pVal2("Value 0-100", 50, 0, 100, 1);
ParameterInt16 pVal3("Value 0-500", 0, 0, 500, 10);
ParameterInt16 pSwitch1("LED", 0, 0, 1, 1, updateLED);

// MenuItems for values and LED switch
MenuItem* valueMenuItems[] = {
  new ParamMenuItem("Value 0- 10", &pVal1, &knob), 
  new ParamMenuItem("Value 0-100", &pVal2, &knob), 
  new ParamMenuItem("Value 0-500", &pVal3, &knob), 
  new ParamMenuItem("LED ", &pSwitch1), 
};
// Value submenu
Menu valueMenu(valueMenuItems, 4);

// Nested submenu example
MenuItem* subSubMenuItems[] = {
  new SubMenuItem("SubSubValues", &valueMenu), 
  new SubMenuItem("Item1"), 
  new SubMenuItem("Item2"), 
};
Menu subSubMenu(subSubMenuItems, 3);

MenuItem* subMenuItems[] = {
  new SubMenuItem("SubValues", &valueMenu), 
  new SubMenuItem("SubSubMenu", &subSubMenu),
  new ActionMenuItem("Action", &updateActionItem)
};
Menu subMenu(subMenuItems, 3);

// a long menu, to demonstrate scrolling
MenuItem* longMenuItems[] = {
  new MenuItem("Item1"), 
  new MenuItem("Item2"), 
  new MenuItem("Item3"), 
  new MenuItem("Item4"), 
  new MenuItem("Item5"), 
  new MenuItem("Item6"), 
  new MenuItem("Item7"), 
  new MenuItem("Item8"), 
};
Menu longMenu(longMenuItems, 8);


// Assembling it all into the main menu
MenuItem* mainMenuItems[] = 
  {new SubMenuItem("Values", &valueMenu), 
   new SubMenuItem("Sub-Menu", &subMenu), 
   new ActionMenuItem("Action", &updateActionItem), 
   new SubMenuItem("Long Menu", &longMenu), 
   };

Menu mainMenu = Menu(mainMenuItems, 4);

// Instance for the menu visualisation
MenuDisplay menuDisplay = MenuDisplay(&display);


void setup() {
  // Initialise Display in text mode
  Wire.begin();
  Wire.setClock(400000L);
  display.begin(&Adafruit128x64, I2C_ADDRESS);

  
}

void loop() {
  // visualize the currently active submenu on the display
  menuDisplay.updateDisplay(mainMenu.getCurrentSubmenu());
  // run the menu navigation, based on the button events
  MenuItem* selectedItem = mainMenu.navigateMenu(buttonEvent());
}
