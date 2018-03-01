#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "elev.h"
#include "button.h"
#include "state.h"
#include "logic.h"





int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press OBSTRUCTION switch to stop elevator and exit program.\n");

    elev_set_motor_direction(0);

	int stop = 0;
	state currentState;
	state* current = &currentState;
	state_init(current);

    while (!stop) {
		// NEW FLOOR
		if ( updateFloor(&(current->floor)) )
		{
			floorLight(current->floor);
			
			if ((current->floor) == (current->target))
			{
				current->target = -1;
				current->dir = 0;
				openDoor(&(current->timer));
			}

			else if (toStop(current))
			{
				openDoor(&(current->timer));
			}
		}

		// CHANGE TARGET FLOOR - aka endre dir
		if ((current->target == -1) && (current->timer == 0))
		{
			if (((current->floor != 3) && (elev_get_button_signal(BUTTON_CALL_UP, current->floor)))   || 
				((current->floor != 0) && (elev_get_button_signal(BUTTON_CALL_DOWN, current->floor))) || 
				(elev_get_button_signal(BUTTON_COMMAND, current->floor)) )
			{
				openDoor(&(current->timer));
			}
			
			else if ( !(nextTarget(current)) ) 
			{
				// target has been reached, if necessary a new target
				// will be set next

				current->target = -1;
				current->dir = 0;
				
				// if we're in an undefined state (shouldn't happen after state_init)
				// there should be set a target for 1st floor
				if (current->floor == -1)
				{
					current->target = 0;
					current->dir = -1;
				}

			}
			else
			{
				current->dir = desiredDirection(current->floor, current->target);
			}
		}

		if (current->timer)
		{
			elev_set_motor_direction(0);
			if ( (current->timer + 3) <= time(NULL) )
			{
				closeDoor(&(current->timer), current->dir);
			}
		}
		else
		{
			elev_set_motor_direction(current->dir);
		}

		buttonUpdate(current);


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
		usleep(1000);
    }

    printf("End of program\n");
    return 0;
}
