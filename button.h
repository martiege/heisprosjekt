#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__




typedef struct
{
	//int lightOn;
	int desiredFloor;
	elev_button_type_t button;
} Button;


typedef struct
{
	Button button;
	int desiredDirection;
} OutsideButton;



void buttonLight(int lightOn, int currentFloor);

void buttonCheck(elev_button_type_t button[4], int desiredFloor[4]);



#endif // #ifndef __INCLUDE_BUTTON_C__
