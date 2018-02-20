#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__

#include "button.h"
#include "elev.h"

#include "channels.h"


void buttonLight(int lightOn, int currentFloor)
{
	elev_set_button_lamp(BUTTON_COMMAND, currentFloor, lightOn);
}



void buttonCheck(elev_button_type_t button[4], int desiredFloor[4])
{
	for (int i = 0; i < 4; i++)
	{
		if (button[i], desiredFloor[i])
		{
			buttonLight(1, desiredFloor[i]);
		}
	}
}


#endif // #ifndef __INCLUDE_BUTTON_C__
