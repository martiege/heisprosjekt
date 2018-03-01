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

    //elev_set_motor_direction(DIRN_UP);
/*
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

	
	i
	//int openDoor = 0;
	//int toStop = 0;
	//int stopLight = 1;
	int timer = 0;
	int emergency = 0;
*/
	int stop = 0;
	state currentState;
	state* current = &currentState;
	state_init(current);

    while (!stop) {
		//printf("While\n");
		// NEW FLOOR
		if ( updateFloor(&(current->floor)) )
		{
			floorLight(current->floor);
			
			if ((current->floor) == (current->target))
			{
				//printf("AT TARGETFLOOR %d, need to stop\n", targetFloor);
				current->target = -1;
				current->dir = 0;
				openDoor(&(current->timer));
				/*
				elev_set_motor_direction(0);
				//toStop = 1;	
				elev_set_door_open_lamp(1);	
				//openDoor = 1;		
				timer = time(NULL);
				*/
				//printf("Time: %d \n", timer);
				//usleep(3000000);
				/*
				elev_set_stop_lamp(stopLight);
				stopLight = !stopLight;
				*/
			}

			else if (toStop(current))//(shouldStop(currentLastFloor, currentDirection, insideButtons, outsideUpButtons, outsideDownButtons))
			{
				//printf("AT FLOOR %d and I need to stop \n", currentLastFloor);
				//openDoor = 1;
				openDoor(&(current->timer));
				/*
				timer = time(NULL);
				elev_set_motor_direction(0);
				elev_set_door_open_lamp(1);
				*/
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
			
			else if ( !(nextTarget(current)) ) //( ! (nextTargetFloor(currentLastFloor, &targetFloor, 
				//currentDirection, insideButtons, outsideUpButtons, 
				//outsideDownButtons)) )
			{
				
				printf("waiting for new target\n");
				// automatically moves to 1st floor
				// if there are no new targets, could be removed
/*	
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
*/
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
				printf("changing direciton...\n");
				current->dir = desiredDirection(current->floor, current->target);
			}
		}
/*
		if (elev_get_obstruction_signal())
		{
			elev_set_door_open_lamp(1);	
			//printf("Timer: %d Time: %d \n", timer, time(NULL));
		}
*/
		if (current->timer)
		{
			elev_set_motor_direction(0);
			if ( (current->timer + 3) <= time(NULL) )
			{
				//printf("Timer: %d Time: %d \n", timer, time(NULL));
				//openDoor = 0;
				//toStop = 0;
				closeDoor(&(current->timer), current->dir);
				/*
				timer = 0;
				elev_set_door_open_lamp(0);
				elev_set_motor_direction(currentDirection);
				*/
			}
		}
		else
		{
			elev_set_motor_direction(current->dir);
		}
/*
		if ( (timer + 3) <= time(NULL))
		{
			printf("Timer: %d Time: %d \n", timer, time(NULL));
			openDoor = 0;
			toStop = 0;
			timer = 0;
			elev_set_door_open_lamp(0);
			//elev_set_motor_direction(currentDirection);
		}
		else
		{
			if (!timer)
			{
				elev_set_motor_direction(currentDirection);
			}
			else
			{
				elev_set_motor_direction(0);
			}
		}
*/		
/*
		if (!timer)
		{
			elev_set_motor_direction(currentDirection);
		}
		else
		{
			elev_set_motor_direction(0);
			usleep(1000);
		}
*/
		// updates the buttons after we've figured out if we should stop
		buttonUpdate(current);
		//buttonCheck(currentLastFloor, currentDirection, targetFloor, insideButtons, outsideUpButtons,
			//outsideDownButtons, requestedFloors);
		
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

		current->emergency = elev_get_stop_signal();
		if (current->emergency)
		{
			handleEmergency(current);
		}
/*		
		while (emergency)
		{
			emergency = elev_get_stop_signal();
			elev_set_stop_lamp(1);
			elev_set_motor_direction(0);
			currentDirection = 0;
			targetFloor = -1;

			for (int i = 0; i < 4; ++i)
			{
				clearFloor(i, insideButtons, outsideUpButtons, outsideDownButtons, requestedFloors);
			}

			if (!emergency)
			{
				elev_set_stop_lamp(0);
				elev_set_motor_direction(0);
				if  (elev_get_floor_sensor_signal() != -1) 
				{
					//openDoor = 1;
					timer = time(NULL);
					elev_set_door_open_lamp(1);
				}
			}
		}
*/
        // Stop elevator and exit program if the obstruction pin is switched
        if (elev_get_obstruction_signal()) 
		{
			printf("STOPPER!! \n");
			current->dir = 0;
            elev_set_motor_direction(current->dir);
			stop = 1;
            //break; // #BreakIsBad
        }
		usleep(1000);
    }

    printf("End of program\n");
    return 0;
}
