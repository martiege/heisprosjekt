#ifndef __INCLUDE_LOGIC_H__
#define __INCLUDE_LOGIC_H__

#include <time.h>
#include <unistd.h>

#include "elev.h"
#include "channels.h"
#include "state.h"


//prioritizes the ground and 3rd floors
int priorityList = {0, 3, 1, 2};

// returns the desired direction based on the current floor and the targeted floor
int desiredDirection(int currentFloor, int targetFloor);

// checks intersecting floor(s) on the way to the current target floor,
// and returns 1 if we need to stop
int shouldStop(int currentFloor, int currentDirection, int insideButtons[4],
	int outsideUpButtons[3], int outsideDownButtons[3]);

int toStop(state* current);

// OLD targetFloor is -1, no current target. returns 0 if no new target
int nextTargetFloor(int currentFloor, int* targetFloor, int currentDirection,
	int insideButtons[4], int outsideUpButtons[3], int outsideDownButtons[3]);

// target is -1, find new target. returns 1 if there's a new target
int nextTarget(state* current);

// opens door and starts the timer
void openDoor(int* timer);

// closes door and stops the timer
void closeDoor(int* timer, int currentDirection);

// handle emergency
void handleEmergency(state* current);

#endif // #ifndef __INCLUDE_LOGIC_H__
