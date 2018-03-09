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
		}

		// check for new targets if there's no target, and the door isn't open
		if ((current->target == -1) && (current->timer == 0))
		{
			findNewTargets(current);
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
