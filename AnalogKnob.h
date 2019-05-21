#ifndef ANALOG_KNOB_H
#define ANALOG_KNOB_H

#include <Arduino.h>

class AnalogKnob {
private:
    uint8_t input;
    uint16_t last_value;
    uint16_t deadzone;
public:
    AnalogKnob(uint8_t input, uint16_t deadzone=5) :
    input(input), last_value(0), deadzone(deadzone)
    {};
    
    float getValue();
    
    bool hasChanged();
    
};

#endif