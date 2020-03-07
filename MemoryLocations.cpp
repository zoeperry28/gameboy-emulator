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

	//GBA_Set_Stack();
}
