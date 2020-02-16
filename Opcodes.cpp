#include "Opcodes.h"
#include "Registers.h"
#include <iostream>

void GB_interpretOpcode(uint8_t opcode)
{
    Registers::CPUREG * R = new Registers::CPUREG;
 
    uint8_t lsb = 0u;
    uint8_t msb = 0u;

    printf("\nOPCODE: %x", opcode);

    switch (opcode)
    {
        // JUMPS
         case 0xC3: 
             uint16_t nn;
             lsb = R->PC++;
             msb = R->PC++;
             nn = ((uint16_t)msb << 8) | lsb;
             std::cout << "RESULT:" << nn; 
             R->PC = nn;
             break; 
        // 16 BIT ARTHMETIC
        
        // ADD HL, N
        case 0x09:
            //R->HL.PAIR = R->BC.PAIR + R->HL.PAIR;
            break;
        case 0x19:
            R->HL.PAIR = R->DE.PAIR + R->HL.PAIR;
            break; 
        case 0x29:
            R->HL.PAIR = R->HL.PAIR + R->HL.PAIR;
            break; 
        case 0x39:
            R->HL.PAIR = R->SP + R->HL.PAIR;
            break; 
        //ADD SP,n
        case 0xE8:
            //TODO
            break;
        //INC nn
        case 0x03:
            R->BC.PAIR++;
            break;
        case 0x13:
            R->DE.PAIR++;
            break;
        case 0x23:
            R->HL.PAIR++;
            break;
        case 0x33:
            R->SP++;
            break;
            //DEC nn
        case 0x0B:
            R->BC.PAIR--;
            break;
        case 0x1B:
            R->DE.PAIR--;
            break;
        case 0x2B:
            R->HL.PAIR--;
            break;
        case 0x3B:
            R->SP--;
            break;
       


    }  
}

