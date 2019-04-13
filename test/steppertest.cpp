#include "Stepper/Stepper.hpp"
#include <wiringPi.h>
#include <iostream>

int main (int argc, char const* argv[]) {
	
	wiringPiSetupGpio();
	
	Stepper myStepper(14, 15, 16);
	
	myStepper.relStep(-300);
	
	return 0;
}
