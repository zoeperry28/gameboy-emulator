#ifndef __REGISTERS__H__
#define __REGISTERS_H__
#include <stdint.h>
#include <string>

std::string GBA_ConvertHextoBin(uint8_t hexa);
void GB_interpretOpcode(uint8_t opcode); 
uint8_t GBA_BinToHex(std::string binary);
 
class Registers
{
	public : 

		typedef struct
		{
			unsigned int nn;
			unsigned int n;
			unsigned int x;
			unsigned int y; 
			unsigned int z; 
			unsigned int p; 
			unsigned int q; 

		} OPI;

		typedef enum
		{
			REGISTER_B,
			REGISTER_C,
			REGISTER_D,
			REGISTER_E,
			REGISTER_H,
			REGISTER_L

		}OPCODE_REG;

		typedef struct
		{	
			unsigned int SP : 16;
			unsigned int PC : 16;

			union
			{
				uint8_t A : 8;
				uint8_t F : 8;
				uint16_t PAIR : 16;

			} AF;

			union
			{
				uint8_t B : 8;
				uint8_t C : 8;
				uint16_t PAIR : 16;

			} BC;

			union
			{
				uint8_t D : 8;
				uint8_t E : 8;
				uint16_t PAIR : 16;

			} DE;

			union
			{
				uint8_t H : 8;
				uint8_t L : 8;
				uint16_t PAIR : 16;

			} HL;

			union
			{
				uint8_t HASH; 
			} MISC;

			union
			{
				uint8_t Z : 1;
				uint8_t N : 1;
				uint8_t H : 1;
				uint8_t C : 1;
				uint8_t F3 : 1;
				uint8_t F2 : 1;
				uint8_t F1 : 1;
				uint8_t F0 : 1;

			} FLAG;
		} CPUREG;

		
} ;

#endif