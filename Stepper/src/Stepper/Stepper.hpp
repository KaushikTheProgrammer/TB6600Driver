class Stepper {
	public:
		Stepper(const int directionPin, const int pulsePin, const int microStepSize);
		void relStep(const int steps);
		void absStep(const int desiredPosition);
	private:
		int _microStepSize;
		int _directionPin;
		int _pulsePin;
		int _currPosition;
		void moveForward(const int steps);
		void moveBackward(const int steps);
};
