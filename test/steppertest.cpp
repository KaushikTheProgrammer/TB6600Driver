#include "Stepper/Stepper.hpp"
#include <wiringPi.h>
#include <iostream>

int main (int argc, char const* argv[]) {
	
	wiringPiSetupGpio();
	
	Stepper myStepper(14, 15, 1);
	
	myStepper.absStep(1200);
	myStepper.absStep(0);
	myStepper.absStep(2400);
	myStepper.absStep(0);
	
	return 0;
}
