

typedef struct 
{
	int curlastFloor; // recording the current or previous floor, initialized to -1
	int targetFloor; // the target floor, -1 for no target (elevator stop)
	int currentDirection; // the current direction, TODO change type to 
	// direction enum
	int insideButtons[4]; // state of inside buttons, on / off
	int outsideUpButtons[3]; // state of outside up buttons, on / off
	int outsideDownButtons[3]; // state of inside up buttons, on / off
	int requestedFloors[4]; // holds requests for spesific floors. 
	// can be used to calculate which floor to go to.
} currentState;

typedef struct
{
	int floor; 			// records the current or last floor
	int target;			// the target floor, -1 if there is no target
	int dir; 			// the current direction of the motor
	int buttons[3][4]; 	// 3x4 matrix containing the current state of the buttons. the first 
} state;

void initState(int* curlastFloor, int* targetFloor, int* currentDirection, 
	int insideButtons[4], int outsideUpButtons[3], int outsideDownButtons[3]);

// updates the current / last (previous) floor variable 
// returns 1 if there's been a change in floors (state), 0 otherwise
int updateFloor(int* curlastFloor);

// martin's branch
