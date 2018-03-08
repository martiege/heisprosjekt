#ifndef __INCLUDE_MAIN_C__
#define __INCLUDE_MAIN_C__

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "elev.h"
#include "button.h"
#include "state.h"
#include "logic.h"
#include "channels.h"
#include "io.h"





int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press OBSTRUCTION switch to stop elevator and exit program.\n");

	//runElevatorLogic();
	int stop = 0;
	state currentState;
	state* current = &currentState;
	state_init(current);
	elev_set_motor_direction(0);

    while (!stop) {
		// check for new floor
		if (updateFloor(current))
		{
			checkFloor(current);
			/*
			// changes which floor light is on
			floorLight(current->floor);
			
			// update state if we've reached the target
			if ((current->floor) == (current->target))
			{
				current->target = -1;
				current->dir = 0;
				openDoor(current);
				clearButtons((current->floor), current);
			}

			// if we haven't reached the target, have a temporary stop if needed
			else if (toStop(current))
			{
				openDoor(current);
				clearButtons((current->floor), current);
			}
			*/
		}

		// check for new targets if there's no target, and the door isn't open
		if ((current->target == -1) && (current->timer == 0))
		{
			findNewTargets(current);
			/*
			// special case, elevator has stopped and someone wants to open 
			// the door in the current floor
			if (((current->floor != 3) && (elev_get_button_signal(BUTTON_CALL_UP, current->floor)))   || 
				((current->floor != 0) && (elev_get_button_signal(BUTTON_CALL_DOWN, current->floor))) || 
				(elev_get_button_signal(BUTTON_COMMAND, current->floor)) )
			{
				openDoor(current);
			}
			
			// updates the target, returns 1 if there is a new target
			else if (nextTarget(current)) 
			{
				// sets the new direction based on the changes in target
				current->dir = desiredDirection(current);
			}
			else
			{
				// target has been reached, if necessary a new target
				// will be set next

				current->target = -1;
				current->dir = 0;
				
				// if we're in an undefined state (shouldn't happen as it's handled in state_init)
				// there should be set a target for 1st floor
				if (current->floor == -1)
				{
					current->target = 0;
					current->dir = -1;
				}

			}
			*/
		}

		// checks the timer, as long as it isn't 0, the doors should be open
		if (current->timer)
		{
			// doors open, keep the motor stopped
			elev_set_motor_direction(0);
			if ( (current->timer + 3) <= time(NULL) )
			{
				// reached the end of the timer (3 seconds since timer was started)
				closeDoor(current);
			}
		}
		else
		{
			// doors closed, keep moving
			elev_set_motor_direction(current->dir);
		}

		// updates the buttons matrix based on users
		buttonUpdate(current);

		// updates the emergency state and handles it
		current->emergency = elev_get_stop_signal();
		if (current->emergency)
		{
			handleEmergency(current);
		}

        // Stop elevator and exit program if the obstruction pin is switched
        if (elev_get_obstruction_signal()) 
		{
			printf("STOPPER!! \n");
			current->dir = 0;
            elev_set_motor_direction(current->dir);
			stop = 1;
        }
		
		// small wait time to ensure the loop isn't run too "fast"
		usleep(1000);
    }
	
    printf("End of program\n");
    return 0;
}


#endif // #ifndef __INCLUDE_MAIN_C__
