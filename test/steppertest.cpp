#include "Stepper/Stepper.hpp"
#include <wiringPi.h>
#include <iostream>

int main (int argc, char const* argv[]) {
	
	wiringPiSetupGpio();
	
	Stepper myStepper(14, 15, 8);
	//myStepper.setMaxAcceleration(0.1);
	
	myStepper.relStep(1600);
	
	return 0;
}
