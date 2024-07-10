#pragma once

#define myPi 3.141592653

// Float clamp function
float clamp(float value, float minVal, float maxVal);

// Sign function (return -1 if less than 0 and 1 if greater)
int sign(float val);

// Turns degrees to rads
float ToRad(float deg);