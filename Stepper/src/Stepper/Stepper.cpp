/**
	Stepper.hpp
	Purpose: Library to be used with the TB6600 Stepper Driver and a Raspberry Pi
	
	@author Kaushik Prakash
	@version 1.0 03/01/19
*/


#include "Stepper.hpp"
#include <wiringPi.h>
#include <iostream>
Stepper::Stepper(const int DIRECTION_PIN, const int PULSE_PIN, const int MICRO_STEP_SIZE) {
	wiringPiSetupGpio(); // Set up wiringPi with Broadcom pin mapping
	
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
	
	for(int i = 0; i < STEPS; i += 1) {
		digitalWrite(_directionPin, LOW);
		digitalWrite(_pulsePin, HIGH);
		delayMicroseconds(50);
		digitalWrite(_pulsePin, LOW);
		delayMicroseconds(50);
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


