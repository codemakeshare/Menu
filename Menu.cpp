#include "Menu.h"

bool MenuItem::update(menu_event_t event) {
    return false; // nothing to do - just return false
};

void MenuItem::getText(char* buffer) {
    strcpy(buffer, name);
}

bool SubMenuItem::update(menu_event_t event) {
    if (submenu!=NULL) {
        parent->goSubmenu(submenu);
    }
    return false; // return false to leave activation state
};

bool ActionMenuItem::update(menu_event_t event) {
    if (callback!=NULL) {
        bool returnValue= callback(callbackArgument);
        if (!returnValue) requestRedraw();
        return returnValue;
    }
    requestRedraw();
    return false; // nothing to do - just return false
};

bool ParamMenuItem::update(menu_event_t event) {
    if (event==MENU_LEAVE) {requestRedraw(); return false;} // nothing to do - just return false
    if (event==MENU_UP) {parameter->increment();    requestRedraw();}
    if (event==MENU_DOWN) {parameter->decrement();  requestRedraw();}
    
    // check analog input
    if (knob!=NULL) {
        if (knob->hasChanged()) {
            parameter->setScaledValue(knob->getValue());
            requestRedraw();
        }
    }
    return true;
};

void ParamMenuItem::getText(char* buffer) {
    strcpy(buffer, name);
    strcat(buffer, "=");
    char valueBuffer[5];
    parameter->getValueAsString(valueBuffer);
    strcat(buffer, valueBuffer);
    return buffer;
}

MenuItem* Menu::getCurrentItem() {
    return items[selectedItem];
  };

MenuItem* Menu::getItem(uint8_t index) {
    if ((index>=0)&&(index<maxCount)) 
      return items[index];
    else return NULL;
  };
  
uint8_t Menu::getSelectedItem() {return selectedItem;};
  
uint8_t Menu::getScrollOffset() {return scrollOffset;};

Menu* Menu::getParent() {
    return parent;
  }
void Menu::setParent(Menu* newParent) {
    parent=newParent;
  }

Menu* Menu::getCurrentSubmenu() {
    return currentSubmenu;
  }

void Menu::goSubmenu(Menu* submenu) {
    currentSubmenu=submenu;
    if (parent!=NULL) parent->goSubmenu(submenu); // recursively go to root, to set current submenu everywhere
    submenu->setParent(this);
  }

void Menu::leaveSubmenu() {
    currentSubmenu=currentSubmenu->getParent();
    if (currentSubmenu==NULL) currentSubmenu=this;
    currentSubmenu->redraw = true;
  }
  
void Menu::goNext() {
    if (selectedItem<maxCount-1) selectedItem++;
    if (selectedItem-scrollOffset>3) scrollOffset=selectedItem-3;
  };
  
void Menu::goPrevious() {
    if (selectedItem>0)  selectedItem--;
    if (selectedItem<scrollOffset) scrollOffset=selectedItem;
  };


MenuItem* Menu::navigateMenu(menu_event_t event) {
    currentSubmenu->redraw=false; 
    if (currentSubmenu->activated) {
        currentSubmenu->activated = currentSubmenu->getCurrentItem()->update(event);
        currentSubmenu->redraw = currentSubmenu->getCurrentItem()->needsRedraw(); 
        return currentSubmenu->getCurrentItem();
    } else {
        currentSubmenu->redraw=true; 
        switch(event) {
            case MENU_UP:
                currentSubmenu->goPrevious();
            break;
            case MENU_DOWN:
                currentSubmenu->goNext();
            break;
            case MENU_SELECT:
                if (currentSubmenu->getCurrentItem()!=NULL) {
                    // set parent to ensure correct return
                    currentSubmenu->getCurrentItem()->parent = currentSubmenu;
                    currentSubmenu->activated = currentSubmenu->getCurrentItem()->update(event);
                    return currentSubmenu->getCurrentItem();
                    
                }
            break;
            case MENU_LEAVE:
                leaveSubmenu();
                currentSubmenu->redraw=true; 
            break;
            default:
               currentSubmenu->redraw=false; // nothing has changed
            break;
        }
        
    }
    return NULL;
}


