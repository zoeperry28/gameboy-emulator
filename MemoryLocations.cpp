#include "MemoryLocations.h"
#include <cstdint>

myTest INITIAL_VALUES;

int inc = 0;

void GB_INIT_STACK()
{
	for (int i = 0; i < sizeof(INITIAL_VALUES.NONCART); i++)
	{
		INITIAL_VALUES.NONCART[i] = 0x0u;
	}

	for (int i = 0; i < sizeof(GBA_STACK); i++)
	{
		GBA_STACK[i] = 0x0u;
	}

	GBA_Set_Stack();
}

uint8_t * GBA_Get_Stack()
{
	return GBA_STACK;
}

uint8_t* GBA_Set_Stack()
{
	for (int i = START_OF_INTERNAL_RAM; i < END_OF_INTERNAL_RAM; i++)
	{
		GBA_STACK[inc] = INITIAL_VALUES.NONCART[i];
		inc++;
	}

	return GBA_STACK;
}

uint8_t GBA_Get_Stack_ITEM(int loc)
{
	return INITIAL_VALUES.NONCART[loc];
}

void GBA_Set_Stack_ITEM(int value, int loc)
{
	INITIAL_VALUES.NONCART[loc] = value;
}
