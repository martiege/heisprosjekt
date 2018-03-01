#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__

#include <stdio.h>

#include "state.h"
#include "elev.h"
#include "channels.h"

// ensures that the elev_set_floor_indicator
// isn't called with an invalid floor value
void floorLight(int floor);


// clears all the buttons on a floor
void clearButtons(int floor, state* current);


// if we hit a new floor, clear all the button lights and state
// checks input from buttons and updates the state
void buttonUpdate(state* current);


#endif // #ifndef __INCLUDE_BUTTON_C__
