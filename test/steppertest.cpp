#include "Stepper/Stepper.hpp"
#include <wiringPi.h>
#include <iostream>
int main (int argc, char const* argv[]) {
	
	wiringPiSetupGpio();
	
	Stepper myStepper(14, 15, 32);
	myStepper.absStep(6400);
	myStepper.absStep(0);
	myStepper.absStep(-6400);
	myStepper.absStep(0);
	myStepper.absStep(3200);
	myStepper.absStep(0);
	
	
	
	
	return 0;
}
