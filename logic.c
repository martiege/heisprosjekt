#ifndef __INCLUDE_LOGIC_C__
#define __INCLUDE_LOGIC_C__

#include "logic.h"




// returns the desired direction based on the current floor and the targeted floor
int desiredDirection(state* current)
{
	// if we're at the target, the current direction should be DIRN_STOP / 0
	if ((current->floor) == (current->target))
	{
		return 0;
	}
	// if the target is above the current floor, then the current direction should be DIRN_UP / 1
	else if ((current->floor) < (current->target))
	{
		return 1;
	}
	// if the target is below the current floor, then the current direction should be DIRN_DOWN / -1
	else
	{
		return -1;
	}
}

// checks intersecting floor(s) on the way to the current target floor and returns 1 if we need to stop
int toStop(state* current)
{
	if ((current->buttons)[BUTTON_COMMAND][current->floor])
	{
		return 1;
	}

	if ( (current->floor != 3) && ((current->buttons)[BUTTON_CALL_UP][current->floor]) )
	{
		if (current->dir == 1)
    		{
      			return 1;
    		}
  	}
  	
	if ( (current->floor != 0) && ((current->buttons)[BUTTON_CALL_DOWN][current->floor]) )
  	{
    		if (current->dir == -1)
    		{
				return 1;
    		}
  	}
	
	return 0;
}


// target is -1, find new target. returns 1 if there's a new target
int nextTarget(state* current)
{
	
	//prioritizes the ground and 4th floors, so that we will go through all the floors
	int priorityList[4] = {0, 3, 1, 2};
	
	for (int i = 0; i < 4; ++i)
	{
		// cannot set new target to be the current floor
		if ((current->floor) != priorityList[i])
		{
			// checks if there are any buttons pressed on the current floor
			if ( ((current->buttons)[BUTTON_COMMAND][priorityList[i]]) || 
				 ((priorityList[i] != 3) && ((current->buttons)[BUTTON_CALL_UP][priorityList[i]])) ||
				 ((priorityList[i] != 0) && ((current->buttons)[BUTTON_CALL_DOWN][priorityList[i]])) )
			{
				current->target = priorityList[i];
				current->dir = desiredDirection(current);
				break;
			}
		}
	}
	
	return ((current->target) != -1);
}


//logic for what to do in case the stop button is pressed
void handleEmergency(state* current)
{
	// stops the elevator and updates the necessary values in the state
	elev_set_stop_lamp(1);
	elev_set_motor_direction(DIRN_STOP);
	current->dir = DIRN_STOP;
	current->target = -1;

	while (current->emergency)
	{
		// update the emergency variable
		current->emergency = elev_get_stop_signal();

		// clears all the floor requests
		for (int i = 0; i < 4; ++i)
		{
			clearButtons(i, current);
		}

		if (!(current->emergency))
		{
			elev_set_stop_lamp(0);
			elev_set_motor_direction(0);
			if  (elev_get_floor_sensor_signal() != -1)
			{
				// opens door if we're in a defined state
				current->timer = time(NULL);
				elev_set_door_open_lamp(1);
			}
		}
	}
}


int updateFloor(state* current)
{
	// checks for a valid floor
	int floor = elev_get_floor_sensor_signal();
	
	if ( floor != -1 )
	{
		// checks for a new floor
		if ( (floor != (current->floor)) || ((current->floor) == -1) ) 
		{
			// changes the floor and returns 1 to show that a change has happened
			current->floor = floor;
			return 1;
		}
	}
	
	// returns 0 as there's been no change in floor
	return 0;
}


void checkFloor(state* current)
{
	// changes which floor light is on
	floorLight(current->floor);
	
	// update state if we've reached the target
	if ((current->floor) == (current->target))
	{
		current->target = -1;
		current->dir = 0;
		openDoor(current);
		clearButtons((current->floor), current);
	}

	// if we haven't reached the target, have a temporary stop if needed
	else if (toStop(current))
	{
		openDoor(current);
		clearButtons((current->floor), current);
	}
}


void findNewTargets(state* current)
{
	// special case, elevator has stopped and someone wants to open 
	// the door in the current floor
	if (((current->floor != 3) && (elev_get_button_signal(BUTTON_CALL_UP, current->floor)))   || 
		((current->floor != 0) && (elev_get_button_signal(BUTTON_CALL_DOWN, current->floor))) || 
		(elev_get_button_signal(BUTTON_COMMAND, current->floor)) )
	{
		openDoor(current);
	}
	
	// updates the target, returns 1 if there is a new target
	else if (nextTarget(current)) 
	{
		// sets the new direction based on the changes in target
		current->dir = desiredDirection(current);
	}
	else
	{
		// target has been reached, if necessary a new target
		// will be set next

		current->target = -1;
		current->dir = 0;
		
		// if we're in an undefined state (shouldn't happen as it's handled in state_init)
		// there should be set a target for 1st floor
		if (current->floor == -1)
		{
			current->target = 0;
			current->dir = -1;
		}

	}
}



#endif // #ifndef __INCLUDE_LOGIC_C__
