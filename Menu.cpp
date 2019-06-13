#include "Menu.h"

bool MenuItem::update(menu_event_t event) {
    return false; // nothing to do - just return false
};

void MenuItem::getText(char* buffer) {
    strcpy(buffer, name);
}

MenuItem* MenuItem::getParent() {
    return parent;
  }
void MenuItem::setParent(MenuItem* newParent) {
    parent=newParent;
  }

bool SubMenuItem::update(menu_event_t event) {
    if (submenu!=NULL) {
        parent->goSubmenu(submenu);
    }
    return false; // return false to leave activation state
};

bool BackMenuItem::update(menu_event_t event) {
    parent->leaveSubmenu();
    return false;
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
    if ((event==MENU_LEAVE)||(event==MENU_SELECT)) {requestRedraw(); return false;} // nothing to do - just return false
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
    if (parent!=NULL) parent->leaveSubmenu(); // recursively go to root, to set current submenu everywhere
    currentSubmenu->redraw = true;
  }
  
void Menu::goNext() {
    selectedItem++;
    if (selectedItem>=maxCount) 
    if (rollover) {
        selectedItem=0;
    } else {
        selectedItem=maxCount-1;
    }
    if (selectedItem<scrollOffset) scrollOffset=selectedItem;
    if (selectedItem-scrollOffset>menuLines-1) scrollOffset=selectedItem-(menuLines-1);
  };
  
void Menu::goPrevious() {
    if (rollover) {
        if (selectedItem>0)  selectedItem--; else selectedItem=maxCount-1;
    } else {
        if (selectedItem>0)  selectedItem--;
    }
    if (selectedItem<scrollOffset) scrollOffset=selectedItem;
    if (selectedItem-scrollOffset>menuLines-1) scrollOffset=selectedItem-(menuLines-1);
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
                    currentSubmenu->getCurrentItem()->setParent(currentSubmenu);
                    currentSubmenu->activated = currentSubmenu->getCurrentItem()->update(NONE);
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

bool Menu::update(menu_event_t event) {
    if (parent!=NULL) {
        parent->goSubmenu(this);
    }
    
    return false; // return false to leave activation state
};

