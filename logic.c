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


int shouldStop(int currentFloor, int currentDirection, int insideButtons[4],
	int outsideUpButtons[3], int outsideDownButtons[3])
{
	if (insideButtons[currentFloor])
	{
		return 1;
	}
	if ((currentFloor != 3) && (outsideUpButtons[currentFloor]))
	{
		if (currentDirection == 1)
		{
			return 1;
		}
	}
	if ((currentFloor != 0) && (outsideDownButtons[currentFloor - 1]))
	{
		if (currentDirection == -1)
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
	for (int i = 0; i < 4; ++i)
	{
		// cannot set new target to be the current floor
		if ((current->floor) != priorityList[i])
		{
			// checks if there are any buttons pressed on the current floor
			if ( (current->buttons[BUTTON_COMMAND][priorityList[i]]) ||
			((priorityList[i] != 3) && (current->buttons[BUTTON_CALL_UP][priorityList[i]])) ||
			((priorityList[i] != 0) && (current->buttons[BUTTON_CALL_DOWN][priorityList[i]])))
			{
				current->target = priorityList[i];
				current->dir = desiredDirection(current->floor, priorityList[i]);
				break;
			}
		}
	}
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



void moveToFloor(int* currentFloor, int* targetFloor, int* currentDirection,
	int insideButtons[4], int outsideUpButtons[3], int outsideDownButtons[3],
	int requestedFloors[4])
{
	// TODO
	// check if we're at the correct floor with buttonCheck
	// check if we should stop at the current floor if we've hit a sensor
	buttonCheck(currentFloor, insideButtons, outsideUpButtons,
		outsideDownButtons, requestedFloors);
	if ((*targetFloor) == -1)
	{
		nextTargetFloor(*currentFloor, targetFloor, currentDirection,
			insideButtons, outsideUpButtons, outsideDownButtons);
	}
	*currentDirection = desiredDirection(*currentFloor, *targetFloor);

	if (shouldStop(currentFloor, currentDirection, insideButtons, outsideUpButtons, outsideDownButtons))
	{
		elev_set_motor_direction(0);
	}
}


#endif // #ifndef __INCLUDE_LOGIC_C__
