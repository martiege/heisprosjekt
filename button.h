#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__

#include <stdio.h>

#include "state.h"
#include "elev.h"
#include "channels.h"

// OLD possibly don't need these functions, as they are only 
// layers above one other function
void insideButtonLight(int lightOn, int currentFloor);

void upButtonLight(int lightOn, int currentFloor);

void downButtonLight(int lightOn, int currentFloor);

// ensures that the elev_set_floor_indicator
// isn't called with an invalid floor value
void floorLight(int curlastFloor);

// OLD possibly can be changed to the other clear function
void clearFloor(int floor, int insideButtons[4], int outsideUpButtons[3], 
	int outsideDownButtons[3], int requestedFloors[4]);
	
// clears all the buttons on a floor
void clearButtons(int floor, state* current);

// OLD check input from buttons, and updates arrays
void buttonCheck(int curlastFloor, int targetFloor, int currentDirection, int insideButtons[4], 
	int outsideUpButtons[3], int outsideDownButtons[3], int requestedFloors[4]);

// if we hit a new floor, clear all the button lights and state
// checks input from buttons and updates the state
void buttonUpdate(state* current);
	

#endif // #ifndef __INCLUDE_BUTTON_C__
