#ifndef BUTTON_PRESS_H
#define BUTTON_PRESS_H
#include <Arduino.h>

/**
 * @class ButtonPress
 * @author felix
 * @date 14/05/19
 * @file ButtonPress.h
 * @brief De-bounces button pushes, to create "once-only" events at button pushes, and definable auto-repeat when button is held down.
 */
class ButtonPress {
private:
    uint8_t pin;
    bool lastState;
    uint8_t activeState; 
    uint32_t lastChange;
    uint16_t filterTime;
    uint16_t repeatPeriod;
public:
    /**
     * @brief Constructor for button press.
     * @param pin Input pin to use for the button
     * @param repeatPeriod Time in milliseconds after which the button press event is repeated when button is held down.
     * @param config Pin state flag (pull-up, pull-down, etc.) for pin configuration.
     * @param activeState Polarity of button. Default: low when pushed.
     * @param filterTime De-bouncing rejection time span in millisecond (jitter will be ignored within this time period after the first activation).
     */
    ButtonPress(uint8_t pin, uint16_t repeatPeriod=0, uint8_t config=INPUT_PULLUP, uint8_t activeState=LOW, uint16_t filterTime=50):
    pin(pin), activeState(activeState), filterTime(filterTime), repeatPeriod(repeatPeriod), lastState(0), lastChange(0) {
        pinMode(pin, config);
    };
    
    /**
     * @brief True only once for each button push. Repeats at specified period.
     * @return True once after button is pushed, and true repeatedly after a defined repeat period.
     */
    bool pushed(); 
    
    /**
     * @brief True only once for each button push after release.
     * @return True once after button release.
     */
    bool released(); 
    
    /**
     * @brief Current state of button.
     * @return True when pushed, false otherwise.
     */
    bool currentlyPushed(); // current state (remains set). True when pushed, false otherwise.
};


#endif