#include "Stepper/Stepper.hpp"
#include <wiringPi.h>
#include <iostream>

int main (int argc, char const* argv[]) {
	
	wiringPiSetupGpio();
	
	Stepper myStepper(14, 15, 32);
	myStepper.relStep(6400);
	
	return 0;
}
