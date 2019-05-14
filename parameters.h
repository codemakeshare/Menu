#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <Arduino.h>

/**
 * @class Parameter
 * @author felix
 * @date 14/05/19
 * @file parameters.h
 * @brief Generic parameter class with a name and a value. This parent class is not meant to be used, choose a suitable subclass.
 */
class Parameter {
private:
  char* name;
public:
    /**
     * @brief Constructor for parameter. Takes a name.
     * @param aname The name of the parameter.
     */
    Parameter(char* aname):
        name (aname) {};
    virtual char* getName() {return name;};
    virtual void getValueAsString(char* valueBuffer);
    virtual void increment();
    virtual void decrement();
    virtual void setScaledValue(float value);
};

/**
 * @class ParameterInt16
 * @author felix
 * @date 14/05/19
 * @file parameters.h
 * @brief Integer subclass of Parameter (16 bit), which defines a range and increment step size.
 */
class ParameterInt16: public Parameter {
private:
  int16_t value;  // parameter value
  int16_t minval; // minimum limit of range
  int16_t maxval; // maximum limit of range
  int16_t stepsize; // the step size for incrementing and decrementing
  void (*callback)(ParameterInt16*); // optional callback that is called when the parameter changes
public:
  /**
   * @brief Constructor for 16-bit parameter.
   * @param aname Name of parameter (string)
   * @param value Initial value of parameter
   * @param minval Minimum limit of range
   * @param maxval Maximum limit of range
   * @param stepsize Step size for incrementing/decrementing 
   */
  ParameterInt16(char* aname, int16_t value, int16_t minval, int16_t maxval, int16_t stepsize, void (*callback)(ParameterInt16*) = NULL) :
   Parameter (aname), value(value), minval(minval), maxval(maxval), stepsize(stepsize), callback(callback) {};

    /**
     * @brief Adds stepsize to value, up to maximum of range. Triggers callback.
     */
    virtual void increment();
    /**
     * @brief Subtracts stepsize from value, down to minimum of range. Triggers callback.
     */
    virtual void decrement();
    /**
     * @brief Set value of the parameter. Triggers callback.
     * @param newValue The new value. 
     */
    virtual void setValue(int16_t newValue);
    /**
     * @brief Wrapper function override of parent class.
     * @param value New value of the parameter as a percentage of the defined range (0.0 .. 1.0).
     */
    virtual void setScaledValue(float value);
    
    /**
     * @brief Returns current parameter value.
     * @return parameter value
     */
    int16_t getValue(){return value;};
    
    /**
     * @brief Returns current value as a string representation.
     * @param valueBuffer Output char buffer to hold the result.
     */
    virtual void getValueAsString(char* valueBuffer);

};


#endif