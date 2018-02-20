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

void buttonCheck(int* curlastFloor, int insideButtons[4], 
	int outsideUpButtons[3], int outsideDownButtons[3], requestedFloors[4])
{
	if (elev_get_floor_sensor_signal() != -1) 
	{
		*curlastFloor = elev_get_floor_sensor_signal();
	}
	for (int i = 0; i < 4; i++)
	{
		if (*curlastFloor == i)
		{
			insideButtonLight(0, i);
			
		}
	}
}


#endif // #ifndef __INCLUDE_BUTTON_C__
