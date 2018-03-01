#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__

#include "button.h"

#include "state.h"
#include "elev.h"
#include "channels.h"

void floorLight(int floor)
{
	if (floor != -1)
	{
		elev_set_floor_indicator(floor);
	}
}


void clearButtons(int floor, state* current)
{
	elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
	(current->buttons)[BUTTON_COMMAND][floor] = 0;

	if (floor != 3)
	{
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
		(current->buttons)[BUTTON_CALL_UP][floor] = 0;
	}
	if (floor != 0)
	{
		elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
		(current->buttons)[BUTTON_CALL_DOWN][floor] = 0;
	}
}


void buttonUpdate(state* current)
{
	// clear the buttons on the current floor if necessary
/*
	if ( ((current->floor) != -1) && ((current->floor) == elev_get_floor_sensor_signal()))
	{
		clearButtons((current->floor), current);
	}
*/
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
