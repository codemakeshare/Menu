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
 
// De-bounced button event handlers with 300ms repeat period and 100ms de-bounce filter time
ButtonPress upButton = ButtonPress(B_UP, 300, 100);
ButtonPress downButton = ButtonPress(B_DOWN, 300, 100);

// no repeat and longer de-bounce time to avoid multiple activations
ButtonPress leftButton = ButtonPress(B_LEFT, 0, 200); // this button is optional
ButtonPress rightButton = ButtonPress(B_RIGHT, 0, 200); 

/** Event handler mapping button pushes to menu events
 */
menu_event_t buttonEvent() {
  if (upButton.pushed()) return MENU_UP;
  if (downButton.pushed()) return MENU_DOWN;
  if (leftButton.pushed()) return MENU_LEAVE; // optional, menu can be used without (a little less convenient)
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
    // if the return or select button is pushed, return false to stop and go back to the menu
    menu_event_t event = buttonEvent();
    if ((event==MENU_LEAVE)||(event==MENU_SELECT)) return false;
    
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
// menu item to return from submenus
// optional for 4-button control, needed for 3-button control
BackMenuItem backMenuItem = BackMenuItem("back..."); 

// Value parameters
ParameterInt16 pVal1("Value 0-10", 0, 0, 10, 1);
ParameterInt16 pVal2("Value 0-100", 50, 0, 100, 1);
ParameterInt16 pVal3("Value 0-500", 0, 0, 500, 10);
ParameterInt16 pSwitch1("LED", 0, 0, 1, 1, updateLED); // value with callback (updateLED)



// MenuItems for values and LED switch
MenuItem* valueMenuItems[] = {
  new ParamMenuItem("Value 0- 10", &pVal1, &knob), 
  new ParamMenuItem("Value 0-100", &pVal2, &knob), 
  new ParamMenuItem("Value 0-500", &pVal3, &knob), 
  new ParamMenuItem("LED ", &pSwitch1), 
  &backMenuItem // optional for 4-button control, needed for 3-button
};
// Value submenu
Menu valueMenu(valueMenuItems, 5, "Values");

// Nested submenu example
MenuItem* subSubMenuItems[] = {
  new SubMenuItem("SubSubValues", &valueMenu), //create a new SubMenuItem to give it a different name
  new MenuItem("Item1"), 
  new MenuItem("Item2"), 
  &backMenuItem // optional for 4-button control, needed for 3-button
};
Menu subSubMenu(subSubMenuItems, 4, "Sub-sub-menu");

MenuItem* subMenuItems[] = {
  &subSubMenu,  // we can use a Menu as MenuItem, using the default name of the Menu
  new SubMenuItem("SubValues", &valueMenu), // or we can create a new SubMenuItem to give it a different name
  new ActionMenuItem("Action", &updateActionItem), 
  &backMenuItem // optional for 4-button control, needed for 3-button
};
Menu subMenu(subMenuItems, 4, "Sub-Menu");

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
  &backMenuItem // optional for 4-button control, needed for 3-button
};
Menu longMenu(longMenuItems, 9, "Long menu", true); // initialise long menu with rollover=true

// Assembling it all into the main menu

MenuItem* mainMenuItems[] = 
  { (MenuItem*)&valueMenu,
    (MenuItem*)&subMenu, 
    new ActionMenuItem("Action", &updateActionItem),
    (MenuItem*)&longMenu, 
   };

Menu mainMenu = Menu(mainMenuItems, 4, "Main menu", false);

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
