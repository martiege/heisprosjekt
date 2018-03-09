#ifndef __INCLUDE_LOGIC_H__
#define __INCLUDE_LOGIC_H__

#include <time.h>
#include <unistd.h>

#include "elev.h"
#include "channels.h"
#include "state.h"


// stops the elevator and keeps running as long as the elevator is in an emergency state
void handleEmergency(state* current);


// updates the states current floor and returns 1 if there's been a change
int updateFloor(state* current);


// ran every time the elevator reaches a new floor
// updates floor indicator, handles reaching the target
// handles any floors on the way that should be stopped at
void checkFloor(state* current);


// handles finding new targets if the elevator is not moving
// also handles the special case where the elevator stands still
// and the door should be opened in the current floor
void findNewTargets(state* current);


#endif // #ifndef __INCLUDE_LOGIC_H__
