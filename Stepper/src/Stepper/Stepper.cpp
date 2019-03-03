#include "Stepper.hpp"
#include <iostream>
#include <wiringPi.h>

Stepper::Stepper(const int directionPin, const int pulsePin, const int microStepSize) {
	wiringPiSetupGpio();
	_directionPin = directionPin;
	_pulsePin = pulsePin;
	pinMode(_directionPin, OUTPUT);
	pinMode(_pulsePin, OUTPUT);
	_microStepSize = microStepSize;
}

void Stepper::absStep(const int desiredPosition) {
	const int numSteps = desiredPosition - _currPosition;
	relStep(numSteps);
	_currPosition = desiredPosition;
}


void Stepper::relStep(const int steps) {
	if(steps > 0) {
		moveForward(steps);
	} else {
		moveBackward(steps * -1);
	}
}

void Stepper::moveForward(const int steps) {
	for(int i = 0; i < steps; i += 1) {
		digitalWrite(_directionPin, LOW);
		digitalWrite(_pulsePin, HIGH);
		delayMicroseconds(50);
		digitalWrite(_pulsePin, LOW);
		delayMicroseconds(50);
	}
}

void Stepper::moveBackward(const int steps) {
	for(int i = 0; i < steps; i += 1) {
		digitalWrite(_directionPin, HIGH);
		digitalWrite(_pulsePin, HIGH);
		delayMicroseconds(50);
		digitalWrite(_pulsePin, LOW);
		delayMicroseconds(50);
	}
}


