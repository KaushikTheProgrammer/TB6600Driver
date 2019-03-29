/**
	Stepper.hpp
	Purpose: Library to be used with the TB6600 Stepper Driver and a Raspberry Pi
	
	@author Kaushik Prakash
	@version 1.0 03/01/19
*/


#include "Stepper.hpp"
#include <wiringPi.h>

#include <iostream>
#include <math.h>

Stepper::Stepper(const int DIRECTION_PIN, const int PULSE_PIN, const int MICRO_STEP_SIZE) {	
	_directionPin = DIRECTION_PIN;
	_pulsePin = PULSE_PIN;
	_microStepSize = MICRO_STEP_SIZE;
	_currPosition = 0;
	// Set the direction and pulse pins as output
	pinMode(_directionPin, OUTPUT);
	pinMode(_pulsePin, OUTPUT);
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
	Tells the stepper motor to move a certain number of steps
	
	@param STEPS Number of steps to take
*/
void Stepper::relStep(const int STEPS) {
	if(STEPS > 0) {
		moveForward(STEPS);
	} else {
		moveBackward(STEPS * -1); // Multiply by -1 because the direction change is handled by _directionPin
	}
}

/**
	Moves the stepper motor forward
	
	@param STEPS Number of steps to take
*/
void Stepper::moveForward(const int STEPS) {

	
	float acceleration = 6400;
	float initVel = 3200;
	float targetVel = 128000;
	float initDelay = (100 / pow( pow(initVel, 2) + (2 * acceleration), 0.5));
	float currDelay = initDelay;
	float finalDelay = 100 / targetVel;
	float multiplier = acceleration / 10000;
	float accelMult = multiplier * -1;
	int currentStep = 0;
	int stepsToTake = (pow(targetVel, 2) - pow(initVel, 2)) / (2 * acceleration);
	std::cout << initDelay << std::endl;
	
	digitalWrite(_directionPin, LOW);
	digitalWrite(_pulsePin, HIGH);
	delayMicroseconds(initDelay * 10000);
	digitalWrite(_pulsePin, LOW);
	delayMicroseconds(initDelay);
	currentStep += 1;
	
	while(currentStep < stepsToTake) {
		currDelay = currDelay * (1 + (accelMult * currDelay * currDelay));
		digitalWrite(_directionPin, LOW);
		digitalWrite(_pulsePin, HIGH);
		delayMicroseconds(currDelay * 10000);
		digitalWrite(_pulsePin, LOW);
		//delayMicroseconds(currDelay * 10000);
		currentStep += 1;
		std::cout << currDelay << std::endl;
	}
}

/**
	Moves the stepper motor backward
	
	@param STEPS Number of steps to take
*/
void Stepper::moveBackward(const int STEPS) {
	for(int i = 0; i < STEPS; i += 1) {
		digitalWrite(_directionPin, HIGH);
		digitalWrite(_pulsePin, HIGH);
		delayMicroseconds(50);
		digitalWrite(_pulsePin, LOW);
		delayMicroseconds(50);
	}
}



int getCurrentPosition() {
	return _currPosition;
}


