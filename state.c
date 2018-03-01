#ifndef __INCLUDE_STATE_C__
#define __INCLUDE_STATE_C__

#include "state.h"

void initState(int* curlastFloor, int* targetFloor, int* currentDirection, 
	int insideButtons[4], int outsideUpButtons[3], int outsideDownButtons[3])
{
	*curlastFloor = -1;
	*targetFloor = -1;
	*currentDirection = 0;
	for (int i = 0; i < 4; ++i)
	{
		insideButtons[i] = 0;
		if (i != 3)
		{
			outsideUpButtons[i] = 0;	
		}
		if (i != 0)
		{
			outsideUpButtons[i - 1] = 0;
		}
	}
}

void state_init(state* current)
{
	// intializes all the variables in the state
	current->floor = -1;
	current->target = -1;
	current->dir = DIRN_STOP;
	current->timer = 0;
	current->emergency = 0;
	
	// initializes all the variables in the buttons matrix
	// the outer loop consists of the different sets of buttons, 
	// 0 is BUTTON_CALL_UP, 1 is BUTTON_CALL_DOWN, 2 is BUTTON_COMMAND
	for (int b = BUTTON_CALL_UP; b < BUTTON_COMMAND + 1; ++b) 
	{
		// the inner loop consists of the 4 floors in the elevator
		// NOTE current->buttens[BUTTON_CALL_UP][3] and current->buttens[BUTTON_CALL_DOWN][0]
		// are invalid as those buttons do not exist
		// this is handled in the relevant funcitons
		for (int f = 0; f < 4; ++f)
		{
			current->buttons[b][f] = 0;
		}
	}
	
	// moves to the closest floor below if it isn't already in a valid state
	// ignores all other input at this time
	while (elev_get_floor_sensor_signal() == -1)
	{
		// changes the current direction if it is DIRN_STOP
		// keeps the system from using elev_set_motor_direction many times in a row
		if (current->dir == DIRN_STOP)
		{
			current->dir = DIRN_DOWN;
			elev_set_motor_direction(current->dir);
		}
		
		// gives the system a wait of 1000 microseconds between each loop
		usleep(1000);
	}
	
	// initializes the current floor, re-initializes 
	// the current direction and stops the elevator
	current->dir = DIRN_STOP;
	elev_set_motor_direction(current->dir);
	current->floor = elev_get_floor_sensor_signal();
}

int updateFloor(int* curlastFloor)
{
	// checks for a valid floor
	int floor = elev_get_floor_sensor_signal();
	
	if ( floor != -1 )
	{
		// checks for a new floor
		if ( (floor != (*curlastFloor)) || ((*curlastFloor) == -1) ) 
		{
			// changes the floor and returns 1 to show that a change has happened
			*curlastFloor = floor;
			return 1;
		}
	}
	
	// returns 0 as there's been no change in floor
	return 0;
}

#endif // #ifndef __INCLUDE_STATE_C__
