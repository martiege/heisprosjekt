#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__

#include <stdio.h>

#include "state.h"
#include "elev.h"
#include "channels.h"

// only call elev_set_floor_indicator if we're in a valid state
void floorLight(int floor);


// clears all the buttons on a floor
void clearButtons(int floor, state* current);


// checks input from buttons and updates the state
void buttonUpdate(state* current);


#endif // #ifndef __INCLUDE_BUTTON_C__
