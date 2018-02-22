#ifndef __INCLUDE_STATE_C__
#define __INCLUDE_STATE_C__


void initState(int* curlastFloor, int* targetFloor, int* currentDirection, 
	int insideButtons[4], int outsideUpButtons[3], int outsideDownButtons[3])
{
	*curlastFloor = -1;
	*targetFloor = -1;
	*currentDirection = 0;
	for (int i = 0; i < 4; ++i)
	{
		insideButtons[i] = 0;
		if (i != 3)
		{
			outsideUpButtons[i] = 0;	
		}
		if (i != 0)
		{
			outsideUpButtons[i - 1] = 0;
		}
	}
}


#endif // #ifndef __INCLUDE_STATE_C__