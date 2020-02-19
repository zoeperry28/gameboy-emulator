#include "Opcodes.h"
#include "Registers.h"
#include <iostream>
Registers::CPUREG* R = new Registers::CPUREG;
unsigned int temp;
static bool REGISTERS_INIT = false; 
void GB_INITIALIZE_REGS()
{   
    R->AF.PAIR = 0x01;
    R->AF.F = 0xB0;
    R->BC.PAIR = 0x0013;
    R->DE.PAIR = 0x00D8;
    R->HL.PAIR = 0x014D;
    R->SP = 0xFFFE;
    R->PC = 0x100;
}
void GB_JMP_Condition_NN(uint8_t FLAG, int SET_CHECK)
{
    if (FLAG == SET_CHECK)
    {
        R->PC = GB_GET_nn();
    }
}

void GB_JMP_Condition(uint8_t FLAG, int SET_CHECK)
{
    if (FLAG == SET_CHECK)
    {
        temp = R->PC;
        R->PC =+ temp;
    }
}

uint16_t GB_GET_nn()
{
    uint16_t nn;
    uint8_t lsb = R->PC++;
    printf("\nLSB: %x", lsb);
    uint8_t msb = R->PC++;
    printf("\nMSB: %x", msb);
    nn = (((msb * 256) + lsb));
    return nn;
}

uint8_t GB_GET_n()
{
    uint8_t n; 
    n = R->PC++;
    return n;
}

void GB_interpretOpcode(uint8_t opcode)
{
    if (REGISTERS_INIT == false)
    {
        GB_INITIALIZE_REGS();
        REGISTERS_INIT = true;
    }
    else
    {
        uint8_t lsb = 0u;
        uint8_t msb = 0u;

        printf("\nOPCODE: %x", opcode);

        switch (opcode)
        {
        // JUMPS
        case 0xC3:
            R->PC = GB_GET_nn();
            break;
        case 0xC2:
            GB_JMP_Condition_NN(R->FLAG.Z, 0);
            break; 
        case 0xCA:
            GB_JMP_Condition_NN(R->FLAG.Z, 1);
            break; 
        case 0xD2:
            GB_JMP_Condition_NN(R->FLAG.C, 0);
            break;
        case 0xDA:
            GB_JMP_Condition_NN(R->FLAG.C, 1);
            break;
        case 0xE9:
            R->PC = R->HL.PAIR;
            break;
        case 0x18:
            temp = R->PC;
            temp =+ R->PC++;
            break; 
        case 0x20: 
            GB_JMP_Condition(R->FLAG.Z, 0);
        case 0x28:
            GB_JMP_Condition(R->FLAG.Z, 1);
            break;
        case 0x30:
            GB_JMP_Condition(R->FLAG.C, 0);
            break;
        case 0x38:
            GB_JMP_Condition(R->FLAG.C, 1);
            break;
            // 16 BIT ARTHMETIC
        case 0x00: 
            std::cout << "\nPC BEFORE NOP:" << R->PC;
            if (R->PC != 0)
            {
                R->PC = R->PC++;
            }
            std::cout << "\nPC AFTER NOP:" << R->PC;
            break;
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
    }


