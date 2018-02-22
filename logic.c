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
	/*
	for (int i = 0; i < 4; ++i)
	{
		
		if (desiredDirection(currentFloor, insideButtons[i])  == currentDirection) 
		{
			return 1;
		}
		
		if (i != 3)
		{
			if (desiredDirection(currentFloor, outsideUpButtons[i]) == currentDirection)
			{
				return 1;
			}
		}
		if (i != 0)
		{
			if (desiredDirection(currentFloor, outsideDownButtons[i - 1]) == currentDirection) 
			{
				return 1;
			}
		}
	}
	*/
}



int nextTargetFloor(int currentFloor, int* targetFloor, int* currentDirection,
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
				*currentDirection = desiredDirection(currentFloor, i);
				break;
			}
			if ((i != 3) && (outsideUpButtons[i]))
			{
				*targetFloor = i;
				*currentDirection = desiredDirection(currentFloor, i);
				break;
			}
			if ((i != 0) && (outsideDownButtons[i - 1]))
			{
				*targetFloor = i;
				*currentDirection = desiredDirection(currentFloor, i);
				break;
			}
		}
	}
	return ((*targetFloor) != -1);
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
