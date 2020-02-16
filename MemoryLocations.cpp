#include "MemoryLocations.h"
#include <cstdint>

void SETMEMMAP()
{
	myTest INITIAL_VALUES;
	
	INITIAL_VALUES.RST00 =						   0u;
	INITIAL_VALUES.RST08 =                         0u;
	INITIAL_VALUES.RST10 =                         0u;
	INITIAL_VALUES.RST18 =                         0u;
	INITIAL_VALUES.RST20 =                         0u;
	INITIAL_VALUES.RST28 =                         0u;
	INITIAL_VALUES.RST30 =                         0u;
	INITIAL_VALUES.RST38 =                         0u;
	INITIAL_VALUES.VerticalBlankInterrupt_Start =  0u;
	INITIAL_VALUES.LCDC_Start  =                   0u;
	INITIAL_VALUES.TimerOverflow_Start  =          0u;
	INITIAL_VALUES.SerialTransfer_Start =          0u;
	INITIAL_VALUES.HTL_Start  =                    0u;
	INITIAL_VALUES.BEGIN_CODE =                    0u;
	INITIAL_VALUES.BIOS_GRAPHIC =                  0u;
	INITIAL_VALUES.GAME_TITLE  =                   0u;
	INITIAL_VALUES.GAMEBOY_COL =                   0u;
	INITIAL_VALUES.LIC_CODE_HIGH =                 0u;
	INITIAL_VALUES.LIC_CODE_LOW =                  0u;
	INITIAL_VALUES.GAMEBOY_INDICATOR =             0u;
	INITIAL_VALUES.CART_TYPE =                     0u;
	INITIAL_VALUES.ROM_SIZE  =                     0u;
	INITIAL_VALUES.RAM_SIZE  =                     0u;
	INITIAL_VALUES.DEST_CODE =                     0u;
	INITIAL_VALUES.LICENSEE_CODE =                 0u;
	INITIAL_VALUES.MASKROM_VER  =                  0u;
	INITIAL_VALUES.COMPLEMENT_C =                  0u;
	INITIAL_VALUES.CHECKSUM =                      0u;
}


