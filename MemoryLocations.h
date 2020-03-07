#pragma once

#ifndef _MEMORYLOCATIONS_H_ 
#define _MEMORYLOCATIONS_H_


constexpr auto VIDEO_RAM			= 0x8000	- 0x8000;
constexpr auto SWITCH_RAM_BNK		=	0xA000	- 0x8000;
constexpr auto INTERNAL_RAM			=	0xC000	- 0x8000;
constexpr auto INTERNAL_RAM_ECHO	=	0xC000	- 0x8000;
constexpr auto SPRITE_MEM_OAM		=	0xC000	- 0x8000;
constexpr auto IO_PORT_START		=	0xff00	- 0x8000;
constexpr auto IO_PORT_END			=	0xff4c	- 0x8000;
constexpr auto START_OF_INTERNAL_RAM=	0xFF80	- 0x8000;
constexpr auto END_OF_INTERNAL_RAM	=	0xFFFE	- 0x8000;
constexpr auto INTERRUPT_ENABLE		=	0xFFFF	- 0x8000;
constexpr auto IO_LCDC = 0xFF40;
constexpr auto IO_STAT = 0xFF41;
constexpr auto IO_SCX = 0xFF42;
constexpr auto IO_LY= 0xFF43;
constexpr auto IO_LYC =  0xFF44;
constexpr auto IO_DMA=   0xFF45;
constexpr auto IO_BGP =  0xFF46;
constexpr auto IO_SCY =  0xFF47;
constexpr auto IO_OBP0 = 0xFF48;
constexpr auto IO_OBP1 = 0xFF49;
constexpr auto IO_WY =   0xFF4A;
constexpr auto IO_WX =   0xFF4B;



#include <stdint.h>


static uint8_t GBA_STACK[END_OF_INTERNAL_RAM - START_OF_INTERNAL_RAM];

void GB_INIT_STACK(); 
uint8_t* GBA_Get_Stack();
uint8_t* GBA_Set_Stack();
uint8_t GBA_Get_Stack_ITEM(int loc);
void GBA_Set_Stack_ITEM(int value, int loc);

struct myTest {

	uint8_t RST00 : 8; //0000
	uint8_t RST08 : 2; //0008
	uint8_t RST10 : 8; //0010
	uint8_t RST18 : 2; //0018
	uint8_t RST20 : 8; //0020
	uint8_t RST28 : 2; //0028
	uint8_t RST30 : 8; //0030
	uint8_t RST38 : 2; //0038
	uint8_t VerticalBlankInterrupt_Start : 8; //0040
	uint8_t LCDC_Start : 2; //0048
	uint8_t TimerOverflow_Start : 8; //0050 
	uint8_t SerialTransfer_Start : 2; //0058 
	uint8_t HTL_Start : 8; //0060
	uint8_t BEGIN_CODE : 3; //0100 - 0103
	char* BIOS_GRAPHIC; //0104 - 0133 This is where the nintendo graphic is placed
	uint8_t GAME_TITLE : 8; //0134 - 0142
	uint8_t GAMEBOY_COL : 1; // 0143
	uint8_t LIC_CODE_HIGH : 1; //0144
	uint8_t LIC_CODE_LOW : 1; //0145
	uint8_t GAMEBOY_INDICATOR : 1; //0146
	uint8_t CART_TYPE : 1; //0147
	uint8_t ROM_SIZE : 1; //0148
	uint8_t RAM_SIZE : 1; //0149
	uint8_t DEST_CODE : 1; //014A
	uint8_t LICENSEE_CODE : 1; //014B
	uint8_t MASKROM_VER : 1; //014C
	uint8_t COMPLEMENT_C : 1; //014D
	uint8_t CHECKSUM : 1;

	uint8_t ROM_EXE[8000] = { 0 } ;

	uint8_t NONCART[0xffff] = { 0 };
};

#endif