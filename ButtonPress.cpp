#include "ButtonPress.h"

bool ButtonPress::pushed() {
    bool state = digitalRead(pin) == activeState;
    uint32_t now = millis();
    if (state) {
        if (!lastState) {
            lastChange=now;
            lastState=true;
            return true;
        } else {
            // do auto-repeat
            if ((repeatPeriod>0) && (now-lastChange>repeatPeriod + filterTime)) {
                lastChange=now;
                lastState=true;
                return true;                
            } 
        }
    } else {
        // check if button was released for longer than the filterTime
        if (now-lastChange > filterTime) { // lockout time exceeded?
            lastState=false;
        }
    }
    return false;
}

bool ButtonPress::released() {
    bool state = digitalRead(pin) == activeState;
    uint32_t now = millis();
    if (state) {
        if (!lastState) {
            lastChange=now;
            lastState=true;
        }
    } else {
        // check if button was released for longer than the filterTime
        if (now-lastChange > filterTime) { // lockout time exceeded?
            if (lastState) {
                lastState=false;
                return true;
            }
        }
    }
    return false;
}

bool ButtonPress::currentlyPushed() {
    pushed(); // call pushed to evaluate state
    return lastState;
}