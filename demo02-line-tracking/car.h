enum state{OUTER, READYFORB, OUTER2INNER, INNER, INNER_YKDJ, YMB, INNER2OUTER,STOP};

class Car {
	public: 
		Car(void); // constructor
		Car(unsigned int, unsigned int, int, float, int);
	
		// variable
		state stateCar;

	
	
	// Phototransister from right to left: 
		void track_detect();  // detect the track( p8 p7 ..... p2 p1)
		void printPath();			// print the value read from the photoresistor
		void tracker();
		void calculateError_outer();
		void reset();
		void changePID(int, int,int);
		void changeBasicSpeed(unsigned int, unsigned int);
	
		void checkFlagB();
		void checkFlagY();
		void calculateError_readyForB();
	
		void monitorCounter();
		
		// void checkFlag_O2I();

		void calculateError_inner();
		void setState(state tempState);
		
		void calculateError_innerYKDJ();
		void checkFlagY_2();
		
	

	private:
		// track detect
		int p1,p2, p3, p4, p5, p6, p7, p8;
		char c;
		char buffer[30];
	
		// PID variable
		unsigned int basicSpeedL;
		unsigned int basicSpeedR;
		int turn;
		int error;
		int lastError;
		int derivative;
		int integral;
		int kp;
		int kd;
		float ki;
		
		// flag to control the car
		int countAC;
		int countY;
	
		int flagAC;
		int flagB;
		char flagY;
//		int flagCheck;
		char flag_O2I;
	
		char ifBlack_1B;
		char ifWhite_1B;
		char ifBlack_2B;
		
		char ifWhite_1Y;
		char ifWhite_2Y;
		char ifBlack_1Y;
		char ifBlack_2Y;
		
		char ifWhite_1DY;
		char ifBlack_1DY;
		char ifWhite_2DY;
		
		int last_leftCounter;
		int last_rightCounter;

};

