#include "Stepper/Stepper.hpp"
#include <wiringPi.h>
#include <iostream>
#include <thread>

Stepper base(9, 8, 4);
Stepper arm1(2, 0, 4);
Stepper arm2(13, 12, 4);
Stepper arm3(22, 21, 4);

const int baseSwitch = 24;
const int arm1Switch = 25;
const int arm2Switch = 27;
const int arm3Switch = 28;

PI_THREAD (runArm1) {
	
}

void homeAll() {
	std::cout << "axis1 homing" << std::endl;
	while(!digitalRead(arm1Switch)) {
		arm1.relStep(1);
	}
	arm1.setCurrentPosition(0);
	
	while(!digitalRead(arm2Switch)) {
		arm2.relStep(-1);
	}
	arm2.setCurrentPosition(0);
	
	while(!digitalRead(arm3Switch)) {
		arm3.relStep(1);
	}
	arm3.setCurrentPosition(0);
}

void moveArm1(int position) {
	arm1.absStep(position);
	arm1.setCurrentPosition(0);

}


void moveArm2(int position) {
	arm2.absStep(position);
	arm2.setCurrentPosition(0);
}

void moveArm3(int position) {
	arm3.absStep(position);
	arm3.setCurrentPosition(0);
}



void setup() {
	pinMode(baseSwitch, INPUT);
	pinMode(arm1Switch, INPUT);
	pinMode(arm2Switch, INPUT);
	pinMode(arm3Switch, INPUT);
	
	arm1.setAcceleration(5);
	arm1.setMaxVelocity(10);
	
	arm2.setAcceleration(5);
	arm3.setAcceleration(5);
}


int main (int argc, char const* argv[]) {
	
	wiringPiSetup();
	setup();
	
	homeAll();
	
	std::thread arm1Thread(moveArm1, -1600);
	arm1Thread.join();
	
	return 0;
}


