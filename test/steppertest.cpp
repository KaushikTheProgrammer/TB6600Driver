#include "Stepper/Stepper.hpp"
#include <wiringPi.h>
#include <iostream>
#include <thread>

Stepper base(9, 8, 4);
Stepper arm1(2, 0, 16);
Stepper arm2(13, 12, 4);
Stepper arm3(22, 21, 4);

const int baseSwitch = 24;
const int arm1Switch = 25;
const int arm2Switch = 27;
const int arm3Switch = 28;


void homeAxis1() {
	std::cout << "axis1 homing" << std::endl;
	while(digitalRead(arm1Switch) != 1) {
		arm1.relStep(1);
	}
	arm1.setCurrentPosition(0);
}

void homeAxis2() {
	std::cout << "axis2 homing" << std::endl;
	while(digitalRead(arm2Switch) != 1) {
		arm2.relStep(-1);
	}
	arm2.setCurrentPosition(0);
}

void homeAxis3() {
	std::cout << "axis3 homing" << std::endl;
	int val = digitalRead(arm3Switch);
	while(val != 1) {
		val = digitalRead(arm3Switch);
		std::cout << "switch3 val " << val << std::endl;
		arm3.relStep(1);
	}
	std::cout << "switch3 val " << val << std::endl;
	arm3.setCurrentPosition(0);
}

void homeAll() {
	homeAxis1();
	homeAxis2();
	homeAxis3();
}

void moveArm1(int position) {
	while(true) {
		arm1.absStep(position);
		arm1.absStep(-position);
	}
	
}


void moveArm2(int position) {
	while(true) {
		arm2.absStep(position);
		arm2.absStep(-position);
	}
}

void moveArm3(int position) {
	while(true) {
		arm3.absStep(position);
		arm3.absStep(-position);
	}
}



void setup() {
	wiringPiSetup();
	pinMode(baseSwitch, INPUT);
	pinMode(arm1Switch, INPUT);
	pinMode(arm2Switch, INPUT);
	pinMode(arm3Switch, INPUT);
	
	arm1.setAcceleration(1);
	arm3.setAcceleration(3.5);
	
	

}


int main (int argc, char const* argv[]) {
	setup();
	arm1.absStep(-70000);
	return 0;
}


