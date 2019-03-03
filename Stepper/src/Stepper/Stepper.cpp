#include "Stepper.hpp"
#include <wiringPi.h>

Stepper::Stepper(const int directionPin, const int pulsePin) {
	pinMode(directionPin, OUTPUT);
	pinMode(pulsePin, OUTPUT);
}

void Stepper::step(int steps) {
	
}
