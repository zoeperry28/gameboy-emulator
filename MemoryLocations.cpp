#include "MemoryLocations.h"
#include <cstdint>

myTest INITIAL_VALUES;
uint8_t* GBA_STACK; 
int inc = 0;
void GBA_INIT_STACK()
{
	for (int i = START_OF_STACK; i < END_OF_STACK; i++)
	{
		GBA_STACK[inc] = INITIAL_VALUES.NONCART[i];
		inc++;
	}

}

uint8_t * GBA_Get_Stack()
{
	return GBA_STACK;
}

uint8_t GBA_Get_Stack_ITEM(int loc)
{
	return GBA_STACK[loc];
}

void GBA_Set_Stack_ITEM(int value, int loc)
{
	GBA_STACK[loc] = value;
}
