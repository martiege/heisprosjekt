#ifndef __INCLUDE_STATE_H__
#define __INCLUDE_STATE_H__

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "channels.h"
#include "elev.h"

/*
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
*/
typedef struct {
	int floor; 					// records the current or last floor
	int target;					// the target floor, -1 if there is no target
	elev_motor_direction_t dir; // the current direction of the motor
	int timer; 					// timer for so we can control opening and closing doors7
	int emergency;				// boolean value for if the elevator is in a state of emergency
	int buttons[3][4]; 			// 3x4 matrix containing the current state of the buttons. the first 
}state;

//typedef struct STATE state;

void initState(int* curlastFloor, int* targetFloor, int* currentDirection, 
	int insideButtons[4], int outsideUpButtons[3], int outsideDownButtons[3]);

// initialize the state
void state_init(state* current);

// updates the current / last (previous) floor variable 
// returns 1 if there's been a change in floors (state), 0 otherwise
int updateFloor(int* curlastFloor);


#endif // #ifndef __INCLUDE_STATE_H__
