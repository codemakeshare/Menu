#ifndef MENU_H
#define MENU_H
#include <Arduino.h>
#include "parameters.h"
#include <AnalogKnob.h>

// Events to control the menu navigation. This would typically be mapped to some buttons.
typedef enum menu_event_t {NONE, MENU_UP, MENU_DOWN, MENU_SELECT, MENU_LEAVE} menu_event_t;

class Menu;

 /**
  * @class MenuItem
  * @author felix
  * @date 13/05/19
  * @file Menu.h
  * @brief Parent class for menu items. There are multiple subclasses for different types of menu items defined below.
  *        MenuItems are added to the Menu class.
  */
class MenuItem {
protected:
    char* name;
    bool redraw;
    Menu* parent;
public:
    
    MenuItem(char* aname):
        name(aname), parent(NULL) { };
    /** getText
     * @brief returns the display text for this item
     * @param buffer: the destination array to write the text into (needs to be big enough to hold the text).
     */
    virtual void getText(char* buffer); 
    /** update
     * @brief arbitrary execution function for menu items, eg. parameter update. Subclasses should override this method.
     * @param event Button pushes to be passed to the update method
     * @return true if activated (update will be repeatedly called). false when done (return to menu navigation).
     */
    virtual bool update(menu_event_t event); // 
    
    /** needsRedraw
     * @brief return flag if this menu item has to be refreshed on the display
     * @return true when redraw is required
     */
    bool needsRedraw() {return redraw;};
    
    /** doneRedraw
     * @brief clear redraw flag to false when redraw is completed
     */
    void doneRedraw() {redraw = false;};
    
    /** requestRedraw
     * @brief set redraw flag to trigger a refresh
     */
    void requestRedraw() {redraw = true;};
    
    MenuItem* getParent();
    void setParent(MenuItem* newParent);
};


/**
 * @class SubMenuItem
 * @author felix
 * @date 13/05/19
 * @file Menu.h
 * @brief Subclass of MenuItem to implement a submenu. If this item is selected, the menu navigation will go to a submenu.
 */
class SubMenuItem: public MenuItem {
protected:
    Menu* submenu;
public:

    SubMenuItem(char* aname, Menu* submenu=NULL):
        MenuItem(aname),  submenu(submenu) { };
    /**
     * @brief update method override to enter the submenu
     * @param event: Button events for menu control
     * @return 
     */
    virtual bool update(menu_event_t event); 
};

/**
 * @class BackMenuItem
 * @author felix
 * @date 13/05/19
 * @file Menu.h
 * @brief Subclass of MenuItem to implement a "back" item. If this item is selected, the menu navigation will go back to the enclosing menu..
 */
class BackMenuItem: public MenuItem {
protected:
public:

    BackMenuItem(char* aname):
        MenuItem(aname) { };
    /**
     * @brief update method override to enter the submenu
     * @param event: Button events for menu control
     * @return 
     */
    virtual bool update(menu_event_t event); 
};

/**
 * @class ActionMenuItem
 * @author felix
 * @date 13/05/19
 * @file Menu.h
 * @brief Subclass for menu items that call a method upon activation. Use this to run programs from the menu.
 */
class ActionMenuItem: public MenuItem {
protected:
    bool (*callback)(void*);
    void* callbackArgument;
public:
    /**
     * @brief Constructor for ActionMenuItem. Takes a callback function that will be called as long as the item is active. The callback function
     *        is repeatedly called until it returns false.
     * @param aname The name of this menu item
     * @param callback A function pointer to a bool callback(void* argument) method, 
     * @param callbackArgument optional void pointer argument passed to the callback function
     */
    ActionMenuItem(char* aname, bool (*callback)(void*) = NULL, void* callbackArgument = NULL) :
    MenuItem(aname), callback(callback), callbackArgument(callbackArgument) {};
    
    /**
     * @brief Overridden update method to call the callback function.
     * @param event button pushes for menu control
     * @return 
     */
    virtual bool update(menu_event_t event); // arbitrary execution function for menu items, eg. parameter update
};

/**
 * @class ParamMenuItem
 * @author felix
 * @date 13/05/19
 * @file Menu.h
 * @brief Subclass for parameter menu items. Implements controls to adjust parameter values with buttons, or analog inputs.
 */
class ParamMenuItem: public MenuItem {
protected:
    Parameter* parameter;
    
    AnalogKnob* knob;
public:
    Menu* parent;
    /**
     * @brief Constructor for ParamMenuItem. Takes a parameter (from parameters.h), and optionally an analog input.
     * @param aname The name of the parameter
     * @param parameter Pointer to the parameter 
     * @param knob optional analog input for direct control of the parameter
     */
    ParamMenuItem(char* aname, Parameter* parameter=NULL, AnalogKnob* knob = NULL):
        MenuItem(aname), parameter(parameter), knob(knob) { };

    virtual void getText(char* buffer); // returns the display text for this item
    virtual bool update(menu_event_t event); // arbitrary execution function for menu items, eg. parameter update
};

/**
 * @class Menu
 * @author felix
 * @date 14/05/19
 * @file Menu.h
 * @brief Menu class to construct menu structures, consisting of MenuItems. Individual menu items can be submenus, parameters or actions. 
 * Submenus can be cascaded as desired.
 */
class Menu: public MenuItem {
private:
  bool rollover; // flag if menu should roll around at top and bottom
  MenuItem** items;
  uint8_t selectedItem;
  bool activated;
  bool redraw;
  uint8_t scrollOffset;
  uint8_t maxCount;
  //Menu* parent;
  Menu* currentSubmenu;
  uint8_t menuLines; //number of lines that fit on the display
public:
  /**
   * @brief Constructor for menu. Takes a list of pointers to menu items. Submenus have to be give
   * @param items Array of pointers to menu items
   * @param count Number of menu items in the array
   */
  Menu(MenuItem** items, uint8_t count, char* name="", bool rollover=false, uint8_t menuLines=4) :
  MenuItem(name), items(items), selectedItem(0), activated(false), rollover(rollover), redraw(true), scrollOffset(0), maxCount(count), currentSubmenu(this), menuLines(menuLines)
  {}
  MenuItem* getCurrentItem();
  
  MenuItem* getItem(uint8_t index);
  
  uint8_t getSelectedItem();
  
  uint8_t getScrollOffset();

  Menu* getParent();
  void setParent(Menu* newParent);

  Menu* getCurrentSubmenu();

  void goSubmenu(Menu* submenu);

  void leaveSubmenu();
  
  void goNext();
  
  void goPrevious();

  bool isActivated() {return activated;};
  
  bool needsRedraw() {return redraw;};
  void doneRedraw() {redraw = false;};
  void requestRedraw() {redraw = true;};
  
  void setRollover(bool roll) {rollover = roll;};
  
  /**
   * @brief Method to run menu navigation. Call this method regularly from the main loop.
   * @param event Button pushes translated to menu_event_t to drive the navigation.
   * @return Returns the currently activated menu item, or NULL if no menu item is active.
   */
  MenuItem* navigateMenu(menu_event_t event);

/**
     * @brief update method override to enter the submenu
     * @param event: Button events for menu control
     * @return 
     */
    virtual bool update(menu_event_t event); 
};

#endif
