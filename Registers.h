#ifndef __REGISTERS__H__
#define __REGISTERS_H__
#include <stdint.h>
#include <string>

std::string GBA_ConvertHextoBin(uint8_t hexa);
void GB_interpretOpcode(uint8_t opcode); 
unsigned int GBA_BinToHex(std::string binary);
 
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
				unsigned int A : 16;
				unsigned int F : 16;
				uint16_t PAIR : 16;

			} AF;

			union
			{
				unsigned int B : 16;
				unsigned int C : 16;
				uint16_t PAIR :16;

			} BC;

			union
			{
				unsigned int D : 16;
				unsigned int E : 16;
				uint16_t PAIR :16;

			} DE;

			union
			{
				unsigned int H : 16;
				unsigned int L : 16;
				uint16_t PAIR :16;

			} HL;

			union
			{
				unsigned int HASH; 
			} MISC;

			union
			{
				unsigned int Z : 1;
				unsigned int N : 1;
				unsigned int H : 1;
				unsigned int C : 1;
				unsigned int F3 : 1;
				unsigned int F2 : 1;
				unsigned int F1 : 1;
				unsigned int F0 : 1;

			} FLAG;
		} CPUREG;

		
} ;

#endif