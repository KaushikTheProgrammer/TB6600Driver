/**
	Stepper.cpp
	Purpose: Library to be used with the TB6600 Stepper Driver and a Raspberry Pi

	@author Kaushik Prakash
	@version 1.0 03/01/19
*/


#include "Stepper.hpp"
#include <wiringPi.h>
#include <math.h>

Stepper::Stepper(const int DIRECTION_PIN, const int PULSE_PIN, const int MICRO_STEP_SIZE) {
	_directionPin = DIRECTION_PIN;
    _pulsePin = PULSE_PIN;
    _microStepSize = MICRO_STEP_SIZE;
    _maxSteps = _microStepSize * 200;	// 200 steps/rev with no microstepping
    _initVel = _maxSteps * 0.5;			// 0.5 rev/s
    _targetVel = _maxSteps;				// 1 rev/s
    _acceleration = _maxSteps * 0.5;	// 0.5 rev/s^2
    _currPosition = 0;
    _frqcy = 100;

    // Set the direction and pulse pins as output
    pinMode(_directionPin, OUTPUT);
    pinMode(_pulsePin, OUTPUT);
}

/**
 * Calculates delays for every step in the routine
 * @param STEPS Number of steps to take
*/
void calculateParameters(const int STEPS) {
    float currDelay;
    if((STEPS / _maxSteps) <= 0.1) {
        // Stepper only needs to move a small distance
        for(int i = 0; i < _maxSteps; i += 1) {
            const float _smallVel = _maxSteps * 0.02;	// Velocity for small movements in steps/s
            currDelay = _frqcy / _smallVel;				// [18]
            _allDelays.push_back(currDelay);
        }
    } else {
        _stopAccel = _propAccel * STEPS;			// Stop accelerating only after a certain number of steps
        _startDecel = STEPS - stopAccel;			// Start decelerating after a certain number of steps
        _multiplier = _maxAccel / pow(_frqcy, 2);	// [19]

        currDelay = (_frqcy / pow( pow(_initVel, 2) + (2 * _maxAccel), 0.5));	// Delay for the first step [17]
        _allDelays.push_back(currDelay);										// Add it as the first delay

        // Calculate delay periods for every step in the routine after the first
        for(int i = 1; i < STEPS; i += 1) {
            if(i <= _stopAccel) {
                // Stepper is in the acceleration phase
                currDelay = currDelay * (1 + (-1 * _multiplier * pow(currDelay, 2)));	// [20]
            } else if(i >= _startDecel) {
                // Stepper is in the deceleration phase
                currDelay = currDelay * (1 + (_multiplier * pow(currDelay, 2)));		// [20]
            } else {
                // Stepper is moving at constant velocity
                currDelay = _frqcy / _maxVel; // [18]
            }
            _allDelays.push_back(currDelay);
        }
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

    // Step the motor shaft
    for(float currDelay : _allDelays) {
        pulse(isForward, currDelay);
    }

    vector<float>().swap(_allDelays); // Remove all delays for this routine and force a reallocation
}

/**
 * Move the stepper by 1 step in the given direction with the given delay
*/
void Stepper::pulse(bool isClockwise, int pulseDelay) {
    if(isClockwise) {
        digitalWrite(_directionPin, LOW);
        digitalWrite(_pulsePin, HIGH);
        delayMicroseconds(pulseDelay * 10000);
        digitalWrite(_pulsePin, LOW);
        _currPosition += 1;
    } else {
        digitalWrite(_directionPin, HIGH);
        digitalWrite(_pulsePin, HIGH);
        delayMicroseconds(pulseDelay * 10000);
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
    move(NUM_STEPS); // Move to reach DESIRED_POSITION
    _currPosition = DESIRED_POSITION; // DESIRED_POSITION has been reached
}

/**
 * Converts and sets max acceleration from rev/s^2 to steps/s^2
 * @param MAX_ACCELERATION acceleration in rev/s^2
*/
void setMaxAcceleration(const float MAX_ACCELERATION) {
    _acceleration = MAX_ACCELERATION * _maxSteps;
}

/**
 * Converts and sets max velocity from rev/s to steps/s
 * @param MAX_VELOCITY velocity in rev/s
*/
void setMaxVelocity(const float MAX_VELOCITY) {
    _targetVel = MAX_VELOCITY * _maxSteps;
}

/**
 * @return Position of the shaft relative to startup position
*/
int getCurrentPosition() {
    return _currPosition;
}