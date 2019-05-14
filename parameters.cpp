#include "parameters.h"

void Parameter::increment() {
};

void Parameter::decrement() {
};

void Parameter::setScaledValue(float value) {
};

void Parameter::getValueAsString(char* valueBuffer) {valueBuffer="?";};

void ParameterInt16::getValueAsString(char* valueBuffer) {
    itoa(value, valueBuffer, 10);
};

void ParameterInt16::increment() {
    value += stepsize;
    if (value>maxval) value=maxval;
    if (callback!=NULL) callback(this);
};

void ParameterInt16::decrement() {
    value -= stepsize;
    if (value<minval) value=minval;
    if (callback!=NULL) callback(this);
};

void ParameterInt16::setScaledValue(float newValue) {
    setValue(int16_t(newValue*(maxval-minval)+minval));
};


void ParameterInt16::setValue(int16_t newValue) {
    value = newValue;
    if (value<minval) value=minval;
    if (value>maxval) value=maxval;
    if (callback!=NULL) callback(this);
};
