typedef struct 
{
	int curlastFloor; // recording the current or previous floor, initialized to -1
	int targetFloor; // the target floor, -1 for no target (elevator stop)
	int currentDirection; // the current direction
	int insideButtons[4]; // state of inside buttons, on / off
	int outsideUpButtons[3]; // state of outside up buttons, on / off
	int outsideDownButtons[3]; // state of inside up buttons, on / off
	int requestedFloors[4]; // holds requests for spesific floors. 
	// can be used to calculate which floor to go to.
} currentState;


void initState(int* curlastFloor, int* targetFloor, int* currentDirection, 
	int insideButtons[4], int outsideUpButtons[3], int outsideDownButtons[3]);

// martin's branch