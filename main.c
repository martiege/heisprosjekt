#include "elev.h"
#include "button.h"
#include "state.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>



int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(0);

    //elev_set_motor_direction(DIRN_UP);

    int currentLastFloor = -1;
    int targetFloor = -1;
    int currentDirection = 0; // TODO remember change type
    int insideButtons[4] = {0, 0, 0, 0};
    int outsideUpButtons[3] = {0, 0, 0};
    int outsideDownButtons[3] = {0, 0, 0};

    int requestedFloors[4] = {0, 0, 0, 0};
    // TODO clean up the state initilization
    //currentState state = { currentLastFloor, targetFloor, currentDirection, insideButttons,
	//outsideUpButtons, outsideDownButtons, requestedFloors };

	
	int stop = 0;
	int openDoor = 0;
	int toStop = 0;
	int stopLight = 1;
	int timer = 0;

    while (!stop) {

		// NEW FLOOR
		if (updateFloor(&currentLastFloor))
		{
			floorLight(currentLastFloor);
			
			if (currentLastFloor == targetFloor)
			{
				targetFloor = -1;
				currentDirection = 0;
				toStop = 1;	
				elev_set_door_open_lamp(1);				
				/*
				elev_set_stop_lamp(stopLight);
				stopLight = !stopLight;
				*/
			}

			if (shouldStop(currentLastFloor, currentDirection, insideButtons,
				outsideUpButtons, outsideDownButtons))
			{
				printf("AT FLOOR %d and I need to stop \n", currentLastFloor);
				openDoor = 1;
				timer = time(NULL);
				elev_set_motor_direction(0);
				elev_set_door_open_lamp(1);
			}
		}

		// CHANGE TARGET FLOOR - aka endre dir
		if (targetFloor == -1)
		{
			if ( ! (nextTargetFloor(currentLastFloor, &targetFloor, 
				&currentDirection, insideButtons, outsideUpButtons, 
				outsideDownButtons)) )
			{
				// automatically moves to 1st floor
				// if there are no new targets, could be removed
				if (currentLastFloor == 0)
				{
					targetFloor = -1;
					currentDirection = 0;
					//usleep(1000);
				}
				else
				{
					targetFloor = 0;
					currentDirection = -1;
				}
			}
		}

		if ( (timer + 3) >= time(NULL) )
		{
			openDoor = 0;
			toStop = 0;
			timer = 0;
			elev_set_motor_direction(currentDirection);
			elev_set_door_open_lamp(0);
		}
		
		if (!openDoor && !toStop)
		{
			elev_set_motor_direction(currentDirection);
		}
		else
		{
			elev_set_motor_direction(0);
			usleep(1000);
		}

		// updates the buttons after we've figured out if we should stop
		buttonCheck(currentLastFloor, currentDirection, insideButtons, outsideUpButtons,
			outsideDownButtons, requestedFloors);
		
		/*
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
			currentDirection = -1;
            elev_set_motor_direction(currentDirection);
			//sleep(1);
        } else if (elev_get_floor_sensor_signal() == 0) {
			currentDirection = 1;
            elev_set_motor_direction(currentDirection);
			//sleep(1);
        }
		*/
        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
			printf("STOPPER!!");
			currentDirection = 0;
            elev_set_motor_direction(currentDirection);
			stop = 1;
            //break; // #BreakIsBad
        }
		usleep(1000);
    }

    printf("End of program\n");
    return 0;
}
