#ifndef __INCLUDE_BUTTON_C__
#define __INCLUDE_BUTTON_C__



/*
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
*/

void insideButtonLight(int lightOn, int currentFloor);

void upButtonLight(int lightOn, int currentFloor);

void downButtonLight(int lightOn, int currentFloor);

void floorLight(int curlastFloor);

void buttonCheck(int curlastFloor, int currentDirection, int insideButtons[4], 
	int outsideUpButtons[3], int outsideDownButtons[3], int requestedFloors[4]);



#endif // #ifndef __INCLUDE_BUTTON_C__
