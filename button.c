#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__

#include <stdio.h>
#include "button.h"
#include "elev.h"
#include "channels.h"

void insideButtonLight(int lightOn, int floorRequest)
{
	elev_set_button_lamp(BUTTON_COMMAND, floorRequest, lightOn);
}

void upButtonLight(int lightOn, int currentFloor)
{
	elev_set_button_lamp(BUTTON_CALL_UP, currentFloor, lightOn);
}

void downButtonLight(int lightOn, int currentFloor)
{
	elev_set_button_lamp(BUTTON_CALL_DOWN, currentFloor, lightOn);
}

void floorLight(int curlastFloor)
{
	if (curlastFloor != -1)
	{
		elev_set_floor_indicator(curlastFloor);
	}
}

void clearFloor(int floor, int insideButtons[4], int outsideUpButtons[3], 
	int outsideDownButtons[3], int requestedFloors[4])
{
	insideButtonLight(0, floor);
	insideButtons[floor] = 0;
	requestedFloors[floor] = 0; // set the request weigth to 0 
		// as we've reached the floor
	if ( (floor != 3) )
	{
		upButtonLight(0, floor);
		outsideUpButtons[floor] = 0; 
	}
	if ( (floor != 0) )
	{
		downButtonLight(0, floor);
		outsideDownButtons[floor - 1] = 0;
	}
}


void clearButtons(int floor, state* current)
{
	insideButtonLight(0, floor);
	(current->buttons)[BUTTON_COMMAND][floor] = 0;
	
	if (floor != 3)
	{
		upButtonLight(0, floor);
		(current->buttons)[BUTTON_CALL_UP][floor] = 0;
	}
	if (floor != 0)
	{
		downButtonLight(0, floor);
		(current->buttons)[BUTTON_CALL_DOWN][floor] = 0;
	}
}


void buttonCheck(int curlastFloor, int currentDirection, int targetFloor, int insideButtons[4], 
	int outsideUpButtons[3], int outsideDownButtons[3], int requestedFloors[4])
{	
	if ( (curlastFloor != -1) && (elev_get_floor_sensor_signal() == curlastFloor) )
	{
		insideButtonLight(0, curlastFloor);
		insideButtons[curlastFloor] = 0;
		requestedFloors[curlastFloor] = 0; // set the request weigth to 0 
			// as we've reached the floor
		if ( (curlastFloor != 3) && ( (currentDirection == 1) || (curlastFloor == targetFloor) || (targetFloor == -1) ) )
		{
			upButtonLight(0, curlastFloor);
			outsideUpButtons[curlastFloor] = 0; 
		}
		if ( (curlastFloor != 0) && ( (currentDirection == -1) || (curlastFloor == targetFloor) || (targetFloor == -1) ) )
		{
			downButtonLight(0, curlastFloor);
			outsideDownButtons[curlastFloor - 1] = 0;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		// check the buttons for input from each floor
		// saves the result in the appropriate arrays
		if ( (i != 3) && (!(outsideUpButtons[i])) )
		{
			outsideUpButtons[i] = elev_get_button_signal(BUTTON_CALL_UP, i);
			
			
			if (outsideUpButtons[i])
			{
				upButtonLight(outsideUpButtons[i], i);
				++requestedFloors[i]; // if we want to move to a certain floor, increment
			}
			
		}
		if ((i != 0) && (!(outsideDownButtons[i - 1])))
		{
			outsideDownButtons[i - 1] = elev_get_button_signal(BUTTON_CALL_DOWN, i);
			
			
			if (outsideDownButtons[i - 1])
			{
				downButtonLight(1, i);
				++requestedFloors[i];
			}
			
		}
		if (!(insideButtons[i]))
		{
			insideButtons[i] = elev_get_button_signal(BUTTON_COMMAND, i);
			
			if (insideButtons[i])
			{
				insideButtonLight(insideButtons[i], i);
				++requestedFloors[i];
			}
			
		}
	}
}

void buttonUpdate(state* current)
{
	// clear the buttons on the current floor if necessary
	if ( ((current->floor) != -1) && ((current->floor) == elev_get_floor_sensor_signal()) )
	{
		clearButtons((current->floor), current);
	}
	
	// go through all the floors
	for (int f = 0; f < 4; ++f)
	{
		// check the buttons, don't check up if we're on floor 4 and down if we're on floor 1
		// only check if it hasn't been checked before
		
		
		// up button
		if ((f != 3) && !(current->buttons[BUTTON_CALL_UP][f]))
		{
			current->buttons[BUTTON_CALL_UP][f] = elev_get_button_signal(BUTTON_CALL_UP, f);
			
			// if the button has been pressed, set that button lamp
			if (current->buttons[BUTTON_CALL_UP][f])
			{
				elev_set_button_lamp(BUTTON_CALL_UP, f, 1);
			}
		}
		
		// down button
		if ((f != 0) && !(current->buttons[BUTTON_CALL_DOWN][f]))
		{
			current->buttons[BUTTON_CALL_DOWN][f] = elev_get_button_signal(BUTTON_CALL_DOWN, f);
			
			// if the button has been pressed, set that button lamp
			if (current->buttons[BUTTON_CALL_DOWN][f])
			{
				elev_set_button_lamp(BUTTON_CALL_DOWN, f, 1);
			}
		}
		
		// command button
		if (!(current->buttons[BUTTON_COMMAND][f]))
		{
			current->buttons[BUTTON_COMMAND][f] = elev_get_button_signal(BUTTON_COMMAND, f);
			
			// if the button has been pressed, set that button lamp
			if (current->buttons[BUTTON_COMMAND][f])
			{
				elev_set_button_lamp(BUTTON_COMMAND, f, 1);
			}
		}
	}
}




#endif // #ifndef __INCLUDE_BUTTON_C__
