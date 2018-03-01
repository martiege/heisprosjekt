#ifndef __INCLUDE_STATE_C__
#define __INCLUDE_STATE_C__

#include "state.h"


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


#endif // #ifndef __INCLUDE_STATE_C__
