/**
	Stepper.cpp
	Purpose: Library to be used with the TB6600 Stepper Driver and a Raspberry Pi

	@author Kaushik Prakash
	@version 1.0 03/01/19
*/


#include "Stepper.hpp"
#include <wiringPi.h>
#include <cmath>
#include <iostream>

Stepper::Stepper(const int DIRECTION_PIN, const int PULSE_PIN, const int MICRO_STEP_SIZE) {
    _directionPin = DIRECTION_PIN;
    _pulsePin = PULSE_PIN;
    
    _microStepSize = MICRO_STEP_SIZE;

    _maxSteps = 200;
    _initVel = 0.01;     // Start at 0.01 rev/s
    _maxVel = 4;        // Max Velocity is 4 rev/s
    _maxAccel = 0.5;    // Acceleraition is 0.5 rev/s^2

    _currPosition = 0;

    pinMode(_directionPin, OUTPUT);
    pinMode(_pulsePin, OUTPUT);
}

/**
 * Calculates delays for every step in the routine
 * @param STEPS Number of steps to take
*/
void Stepper::calculateParameters(int STEPS) {
	STEPS = std::abs(STEPS); // Direction doesn't matter
    
    float currDelay; // Delay period for each step
    int accelSteps = _propAccel * STEPS; // Step number after which acceleration should stop
    int velSteps = STEPS - accelSteps; // Step number after which constant velocity should stop

    _maxSteps *= _microStepSize;

    // Convert rev/s to steps/s
    _initVel *= _maxSteps;
    _maxVel *= _maxSteps;
    _maxAccel *= _maxSteps;
    
    _maxVelDelay = (_frqcy / _maxVel);  // Delay per step that should not be exceeded
    
    _multiplier = _maxAccel / (float) pow(_frqcy, 2);   // Recalculate multiplier 

    currDelay = (_frqcy / pow( pow(_initVel, 2) + (2 * _maxAccel), 0.5));	    // Delay for the first step [17]
    _allDelays.push_back( (int) (currDelay * 10000));										// Add it
    
    int stepNumber = 1;
    
    for(stepNumber; stepNumber < accelSteps; stepNumber += 1) {
    	currDelay = currDelay * (1 + (-1 * _multiplier * pow(currDelay, 2)));	// [20]
    	_allDelays.push_back( (int) (currDelay * 10000));
    }
    
    // Motor should not move faster than the maximum speed
    if(currDelay < _maxVelDelay) {
        currDelay = _maxVelDelay;
    }

    for(stepNumber; stepNumber < velSteps; stepNumber += 1) {
    	_allDelays.push_back( (int) (currDelay * 10000));
    }
    
    for(stepNumber; stepNumber < STEPS; stepNumber += 1) {
    	currDelay = currDelay * (1 + (_multiplier * pow(currDelay, 2)));	// [20]
    	_allDelays.push_back((int) (currDelay * 10000));
    }   
}

/**
	Steps through the entire stepper routine for a given number of steps
	@param STEPS Number of steps to take
*/
void Stepper::relStep(const int STEPS) {
	
    calculateParameters(STEPS);	// Calculate delays for every step

    bool isForward = true;
    
    if(STEPS < 0) {
        isForward = false;
    }
   	for(float stepDelay : _allDelays) {
   		pulse(isForward, stepDelay);
   	}

    std::vector<int>().swap(_allDelays); // Remove all delays for this routine and force a reallocation
}

void Stepper::velStep(int STEPS, float radps) {
    float radpsDelay = (_frqcy / (radps*_maxSteps)) * 10000;
    bool isForward = true;
    
    
    if(STEPS < 0) {
        isForward = false;
    }

    for(int i = 0; i < STEPS; i += 1) {
        pulse(isForward, (int) radpsDelay);
    }

}

/**
 * Move the stepper by 1 step in the given direction with the given delay
*/
void Stepper::pulse(bool isClockwise, int pulseDelay) {
    if(isClockwise) {
        digitalWrite(_directionPin, LOW);
        digitalWrite(_pulsePin, HIGH);
        delayMicroseconds(pulseDelay);
        digitalWrite(_pulsePin, LOW);
        _currPosition += 1;
    } else {
        digitalWrite(_directionPin, HIGH);
        digitalWrite(_pulsePin, HIGH);
        delayMicroseconds(pulseDelay);
        digitalWrite(_pulsePin, LOW);
        _currPosition -= 1;
    }
}

/**
	Tells the stepper motor to go to DESIRED_POSITION from _currPosition, which is 0 on startup
	@param DESIRED_POSITION Desired final position of the shaft
*/
void Stepper::absStep(const int DESIRED_POSITION) {
    const int NUM_STEPS = DESIRED_POSITION - _currPosition; // Number of steps to take to reach DESIRED_POSITION
    relStep(NUM_STEPS); // Move to reach DESIRED_POSITION
    _currPosition = DESIRED_POSITION; // DESIRED_POSITION has been reached
}

/**
 * Converts and sets max acceleration from rev/s^2 to steps/s^2
 * @param MAX_ACCELERATION acceleration in rev/s^2
*/
void Stepper::setMaxAcceleration(const float MAX_ACCELERATION) {
    _maxAccel = MAX_ACCELERATION;
}

/**
 * Converts and sets max velocity from rev/s to steps/s
 * @param MAX_VELOCITY velocity in rev/s
*/
void Stepper::setMaxVelocity(const float MAX_VELOCITY) {
    _maxVel = MAX_VELOCITY;
}

/**
 * @return Position of the shaft relative to startup position
*/
int Stepper::getCurrentPosition() {
    return _currPosition;
}
