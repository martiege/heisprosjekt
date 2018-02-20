#include "elev.h"
#include "button.h"
#include <stdio.h>
#include <unistd.h>



int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);
/*
    int desiredFloor[4];
    elev_button_type_t button[4];

    for (int i = 0; i < 4; i++)
    {
	//Button b;
	buttonLight(0, desiredFloor[i]);
	desiredFloor[i] = i;
	button[i] = BUTTON_COMMAND;
	//buttons[i] = b;
    }
*/
    while (1) {
	//buttonCheck(button, desiredFloor);
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
	    sleep(1);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
	    sleep(1);
        }
        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    printf("End of program\n");
    return 0;
}
