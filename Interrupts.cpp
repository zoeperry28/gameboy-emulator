#include "Interrupts.h"
#include "MemoryLocations.h"
// The V - Blank interrupt occurs ~59.7 times a second
// on a regular GB and ~61.1 times a second on a Super
// GB(SGB).This interrupt occurs at the beginning of
// the V - Blank period.During this period video
// hardware is not using video ram so it may be freely
// accessed.This period lasts approximately 1.1 ms.

void GB_PROCESS_INTERRUPTS()
{

}
/*
 * is the only way the tiles in vram should be initialised. 
*/
void GB_VBLANK(uint8_t * NON__CART)
{
	uint8_t STAT_REGISTER = IO_STAT;
	if ((NON__CART[STAT_REGISTER] & 0x000F0000) != 0)
	{
		
	}
}
void GB_LCDC_STATUS()
{
	uint8_t REGISTER_START = IO_LCDC;
	uint8_t STAT_REGISTER = IO_STAT;


}
void GB_TIMER_OVERFLOW()
{

}
void GB_SERIAL()
{

}
void GB_HITOLO()
{

}