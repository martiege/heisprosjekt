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


void buttonCheck(int curlastFloor, int currentDirection, int insideButtons[4], 
	int outsideUpButtons[3], int outsideDownButtons[3], int requestedFloors[4])
{	
	if ( (curlastFloor != -1) && (elev_get_floor_sensor_signal() == curlastFloor) )
	{
		insideButtonLight(0, curlastFloor);
		insideButtons[curlastFloor] = 0;
		requestedFloors[curlastFloor] = 0; // set the request weigth to 0 
			// as we've reached the floor
		if ( (curlastFloor != 3) && (currentDirection == 1) )
		{
			upButtonLight(0, curlastFloor);
			outsideUpButtons[curlastFloor] = 0; 
		}
		if ( (curlastFloor != 0) && (currentDirection == -1) )
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


#endif // #ifndef __INCLUDE_BUTTON_C__
