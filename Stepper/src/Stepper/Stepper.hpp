class Stepper {
	public:
		Stepper(const int directionPin, const int pulsePin, const int microStepSize);
		void relStep(const int steps);
	private:
		int _microStepSize;
		int _directionPin;
		int _pulsePin;
		void moveForward(const int steps);
		void moveBackward(const int steps);

};
