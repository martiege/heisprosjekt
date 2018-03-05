#ifndef __INCLUDE_STATE_H__
#define __INCLUDE_STATE_H__

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "channels.h"
#include "elev.h"


typedef struct {
	int floor; 					// records the current or last floor
	int target;					// the target floor, -1 if there is no target
	elev_motor_direction_t dir; // the current direction of the motor
	int timer; 					// timer for so we can control opening and closing doors7
	int emergency;				// boolean value for if the elevator is in a state of emergency
	int buttons[3][4]; 			// 3x4 matrix containing the current state of the buttons. the first 
}state;


// opens door and starts the timer
void openDoor(state* current);


// closes door and stops the timer
void closeDoor(state* current);


// initialize the state
void state_init(state* current);


#endif // #ifndef __INCLUDE_STATE_H__
