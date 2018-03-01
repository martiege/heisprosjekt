#ifndef __INCLUDE_LOGIC_C__
#define __INCLUDE_LOGIC_C__

#include "logic.h"





int desiredDirection(int currentFloor, int targetFloor)
{
	if (currentFloor == targetFloor)
	{
		return 0;
	}
	else if (currentFloor < targetFloor)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}


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


int nextTargetFloor(int currentFloor, int* targetFloor, int currentDirection,
	int insideButtons[4], int outsideUpButtons[3], int outsideDownButtons[3])
{
	// TODO: add logic prioritize 1st floor, then 4th

	// will always prioritize:  insideButtons > outsideUpButtons > outsideDownButtons
	// and:  1st floor > 2nd floor > 3rd floor > 4th floor
	for (int i = 0; i < 4; ++i)
	{
		if (currentFloor != i)
		{
			if (insideButtons[i] )
			{
				*targetFloor = i;
				//*currentDirection = desiredDirection(currentFloor, i);
				break;
			}
			if ((i != 3) && (outsideUpButtons[i]))
			{
				*targetFloor = i;
				//*currentDirection = desiredDirection(currentFloor, i);
				break;
			}
			if ((i != 0) && (outsideDownButtons[i - 1]))
			{
				*targetFloor = i;
				//*currentDirection = desiredDirection(currentFloor, i);
				break;
			}
		}
	}
	return ((*targetFloor) != -1);
}


//decides next target floor based on priority
int nextTarget(state* current)
{
	
	//prioritizes the ground and 4th floors
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
				current->dir = desiredDirection(current->floor, priorityList[i]);
				break;
			}
		}
	}
	return ((current->target) != -1);
}

//resets timer, stops motor and opens door (lights up lamp)
void openDoor(int* timer)
{
	*timer = time(NULL);
	elev_set_motor_direction(0);
	elev_set_door_open_lamp(1);
}

//resets timer, closes door (light turns off) and sets direction of motor
void closeDoor(int* timer, int dir)
{
	*timer = 0;
	elev_set_door_open_lamp(0);
	elev_set_motor_direction(dir);
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
				//openDoor = 1;
				current->timer = time(NULL);
				elev_set_door_open_lamp(1);
			}
		}
	}
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


void runElevatorLogic()
{
	int stop = 0;
	state currentState;
	state* current = &currentState;
	state_init(current);
	elev_set_motor_direction(0);

    while (!stop) {
		// NEW FLOOR
		if ( updateFloor(&(current->floor)) )
		{
			floorLight(current->floor);
			
			if ((current->floor) == (current->target))
			{
				current->target = -1;
				current->dir = 0;
				openDoor(&(current->timer));
				clearButtons((current->floor), current);
			}

			else if (toStop(current))
			{
				openDoor(&(current->timer));
				clearButtons((current->floor), current);
			}
		}

		// CHANGE TARGET FLOOR - aka endre dir
		if ((current->target == -1) && (current->timer == 0))
		{
			if (((current->floor != 3) && (elev_get_button_signal(BUTTON_CALL_UP, current->floor)))   || 
				((current->floor != 0) && (elev_get_button_signal(BUTTON_CALL_DOWN, current->floor))) || 
				(elev_get_button_signal(BUTTON_COMMAND, current->floor)) )
			{
				openDoor(&(current->timer));
			}
			
			else if ( !(nextTarget(current)) ) 
			{
				// target has been reached, if necessary a new target
				// will be set next

				current->target = -1;
				current->dir = 0;
				
				// if we're in an undefined state (shouldn't happen after state_init)
				// there should be set a target for 1st floor
				if (current->floor == -1)
				{
					current->target = 0;
					current->dir = -1;
				}

			}
			else
			{
				current->dir = desiredDirection(current->floor, current->target);
			}
		}

		if (current->timer)
		{
			elev_set_motor_direction(0);
			if ( (current->timer + 3) <= time(NULL) )
			{
				closeDoor(&(current->timer), current->dir);
			}
		}
		else
		{
			elev_set_motor_direction(current->dir);
		}

		buttonUpdate(current);


		current->emergency = elev_get_stop_signal();
		if (current->emergency)
		{
			handleEmergency(current);
		}

        // Stop elevator and exit program if the obstruction pin is switched
        if (elev_get_obstruction_signal()) 
		{
			printf("STOPPER!! \n");
			current->dir = 0;
            elev_set_motor_direction(current->dir);
			stop = 1;
        }
		usleep(1000);
    }
}


#endif // #ifndef __INCLUDE_LOGIC_C__
