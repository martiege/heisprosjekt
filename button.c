#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__

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

/*

typedef struct 
{
	int curlastFloor; // recording the current or previous floor
	int insideButtons[4]; // state of inside buttons, on / off
	int outsideUpButtons[3]; // state of outside up buttons, on / off
	int outsideDownButtons[3]; // state of inside up buttons, on / off
	int requestedFloors[4]; // holds requests for spesific floors. 
	// can be used to calculate which floor to go to.
} currentState;

*/

// returns 1 if 
int buttonCheck(int* curlastFloor, int insideButtons[4], 
	int outsideUpButtons[3], int outsideDownButtons[3], requestedFloors[4])
{
	int changedFloor = 0;
	if (elev_get_floor_sensor_signal() != -1) 
	{
		*curlastFloor = elev_get_floor_sensor_signal();
		changedFloor = 1;
	}
	
	for (int i = 0; i < 4; i++)
	{
		// check the buttons for input from each floor
		// saves the result in the appropriate arrays
		if ((i != 3) && (!(outsideUpButtons[i])))
		{
			outsideUpButtons[i] = elev_get_button_signal(BUTTON_CALL_UP, i);
			/*
			if (outsideUpButtons[i])
			{
				++requestedFloors[i]; // if we want to move to a certain floor, increment
			}
			*/
		}
		if ((i != 0) && (!(outsideDownButtons[i])))
		{
			outsideDownButtons[i] = elev_get_button_signal(BUTTON_CALL_DOWN, i);
			/*
			if (outsideDownButtons[i])
			{
				++requestedFloors[i];
			}
			*/
		}
		if (!(insideButtons[i]))
		{
			insideButtons[i] = elev_get_button_signal(BUTTON_COMMAND, i);
			/*
			if (insideButtons[i])
			{
				++requestedFloors[i];
			}
			*/
		}
		
		 // turn light off if reached 
		if ((*curlastFloor) == i) && (elev_get_floor_sensor_signal() == i))
		{
			insideButtonLight(0, i);
			insideButtons[i] = 0;
			requestedFloors[i] = 0; // set the request weigth to 0 as we've reached the floor
			if (i != 3)
			{
				upButtonLight(0, i);
				outsideUpButtons[i] = 0;
			}
			if (i != 0)
			{
				downButtonLight(0, i);
				outsideDownButtons[i - 1] = 0;
			}
		}
	}
	return changedFloor;
}


#endif // #ifndef __INCLUDE_BUTTON_C__
