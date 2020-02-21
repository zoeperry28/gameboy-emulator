#ifndef __OPCODES_H__ 
#define __OPCODES_H__

#include <stdint.h>

uint16_t GB_GET_nn();
uint8_t GB_GET_n();
void GB_retrieveOpcodes(uint8_t* MEMORY_MAP);
class Opcodes
{
	public :
		void GB_INITIALIZE_REGS();
		void GB_interpretOpcode(uint8_t opcode);
	private: 
		
};

#endif

