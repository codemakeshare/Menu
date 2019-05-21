#include "AnalogKnob.h"

float AnalogKnob::getValue() {
    last_value = analogRead(input);
    float value = last_value/1000.0;
    if (value>1.0) value=1.0;
    return value;
}
    
bool AnalogKnob::hasChanged() {
    float value = analogRead(input);
    return (abs(value-last_value)>deadzone);
}