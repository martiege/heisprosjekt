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

	runElevatorLogic();
	
    printf("End of program\n");
    return 0;
}


#endif // #ifndef __INCLUDE_MAIN_C__
