#include "Opcodes.h"
#include "Registers.h"
#include <iostream>
#include <Windows.h>

Registers::CPUREG* R = new Registers::CPUREG;
uint8_t n;
uint16_t nn;
uint8_t* MEMORY_STATUS;
unsigned int temp;
static bool REGISTERS_INIT = false; 


uint8_t GB_JP_nn(uint8_t nn)
{
    temp = (nn & 0x0F);
    R->PC = R->PC + temp;
    return temp;
}

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

void GB_LD_nn_n(uint8_t LD_nn)
{
    R->PC++;
    MEMORY_STATUS[R->PC] = LD_nn;
}

void GB_LD_r1_r2(uint8_t R1, uint8_t R2)
{
    R1 = R2; 
}

void GB_LD_A_n(uint8_t n)
{
    R->AF.A = n; 
}

void GB_LD_n_A(uint8_t n)
{
    n = R->AF.A;
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
    R->PC++;
    uint8_t lsb = MEMORY_STATUS[R->PC++];
    std::cout << "\nLSB: " << lsb;
    uint8_t msb = MEMORY_STATUS[R->PC++];
    std::cout << "\nMSB: " << msb;
    nn = lsb + msb;
    std::cout << "\nNN: " << nn;
    return nn;
}

uint8_t GB_GET_n()
{   
    n = R->PC++;
    return n;
}

// ------ ALU ------

void GB_ADD_A_n(uint8_t n)
{
    R->AF.A = R->AF.A + n;
}

void GB_ADC_A_n(uint8_t n)
{
    R->AF.A = n + R->FLAG.C;
    R->FLAG.N = 0; 
    if (R->AF.A == 0)
    {
        R->FLAG.Z = 1;
    }

    //tf is a carry
}

void GB_SUB_n(uint8_t n)
{
    R->AF.A = R->AF.A - n;
}

void GB_SBC_n(uint8_t n)
{
    R->AF.A = R->AF.A - (n + R->FLAG.C);
}

void GB_CP_n(uint8_t n)
{
    temp = R->AF.A - n;
    R->FLAG.N = 1;
    if (temp == 0)
    {
        R->FLAG.Z = 1; 
    }
    if (R->AF.A < n)
    {
        R->FLAG.C = 1;
    }

    R->PC++;
}

// ------ ALU ------

void GB_retrieveOpcodes(uint8_t* MEMORY_MAP)
{
    R->PC = 0x0;
    std::cout << "I GOT THIS ONE: " << MEMORY_MAP[R->PC];
    std::cout << "I GOT THIS ONE: " << MEMORY_MAP[R->PC+1];
    while (1)
    {
        MEMORY_STATUS = MEMORY_MAP;
        GB_interpretOpcode(MEMORY_STATUS[R->PC]);
        Sleep(5000);
    }
}

/*
 * Theres not really an elegant way to do this... 
 */
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
        //8 BIT LOADS

        // LD nn, n
        case 0x06:GB_LD_nn_n(R->BC.B); break; 
        case 0x0E:GB_LD_nn_n(R->BC.C); break;
        case 0x16:GB_LD_nn_n(R->DE.D); break;
        case 0x1E:GB_LD_nn_n(R->DE.E); break;
        case 0x26:GB_LD_nn_n(R->HL.H); break;
        case 0x2E:GB_LD_nn_n(R->HL.L); break;
        
         //LD r1, r2
        case 0x7F:GB_LD_r1_r2(R->AF.A, R->AF.A); break; 
        case 0x78:GB_LD_r1_r2(R->AF.A, R->BC.B); break;
        case 0x79:GB_LD_r1_r2(R->AF.A, R->BC.C); break;
        case 0x7A:GB_LD_r1_r2(R->AF.A, R->DE.D); break;                  
        case 0x7B:GB_LD_r1_r2(R->AF.A, R->DE.E); break;
        case 0x7C:GB_LD_r1_r2(R->AF.A, R->HL.H); break;
        case 0x7D:GB_LD_r1_r2(R->AF.A, R->HL.L); break;
        
        case 0x40:GB_LD_r1_r2(R->BC.B, R->BC.B);    break;
        case 0x41:GB_LD_r1_r2(R->BC.B, R->BC.C);    break;
        case 0x42:GB_LD_r1_r2(R->BC.B, R->DE.D);    break;
        case 0x43:GB_LD_r1_r2(R->BC.B, R->DE.E);    break;
        case 0x44:GB_LD_r1_r2(R->BC.B, R->HL.H);    break;
        case 0x45:GB_LD_r1_r2(R->BC.B, R->HL.L);    break;
        case 0x46:GB_LD_r1_r2(R->BC.B, R->HL.PAIR); break;

        case 0x48:GB_LD_r1_r2(R->BC.C, R->BC.B);    break;
        case 0x49:GB_LD_r1_r2(R->BC.C, R->BC.C);    break;
        case 0x4A:GB_LD_r1_r2(R->BC.C, R->DE.D);    break;
        case 0x4B:GB_LD_r1_r2(R->BC.C, R->DE.E);    break;
        case 0x4C:GB_LD_r1_r2(R->BC.C, R->HL.H);    break;
        case 0x4D:GB_LD_r1_r2(R->BC.C, R->HL.L);    break;
        case 0x4E:GB_LD_r1_r2(R->BC.C, R->HL.PAIR); break;

        case 0x50:GB_LD_r1_r2(R->DE.D, R->BC.B);    break;
        case 0x51:GB_LD_r1_r2(R->DE.D, R->BC.C);    break;
        case 0x52:GB_LD_r1_r2(R->DE.D, R->DE.D);    break;
        case 0x53:GB_LD_r1_r2(R->DE.D, R->DE.E);    break;
        case 0x54:GB_LD_r1_r2(R->DE.D, R->HL.H);    break;
        case 0x55:GB_LD_r1_r2(R->DE.D, R->HL.L);    break;
        case 0x56:GB_LD_r1_r2(R->DE.D, R->HL.PAIR); break;

        case 0x58:GB_LD_r1_r2(R->DE.E, R->BC.B);    break;
        case 0x59:GB_LD_r1_r2(R->DE.E, R->BC.C);    break;
        case 0x5A:GB_LD_r1_r2(R->DE.E, R->DE.D);    break;
        case 0x5B:GB_LD_r1_r2(R->DE.E, R->DE.E);    break;
        case 0x5C:GB_LD_r1_r2(R->DE.E, R->HL.H);    break;
        case 0x5D:GB_LD_r1_r2(R->DE.E, R->HL.L);    break;
        case 0x5E:GB_LD_r1_r2(R->DE.E, R->HL.PAIR); break;

        case 0x60:GB_LD_r1_r2(R->HL.H, R->BC.B);    break;
        case 0x61:GB_LD_r1_r2(R->HL.H, R->BC.C);    break;
        case 0x62:GB_LD_r1_r2(R->HL.H, R->DE.D);    break;
        case 0x63:GB_LD_r1_r2(R->HL.H, R->DE.E);    break;
        case 0x64:GB_LD_r1_r2(R->HL.H, R->HL.H);    break;
        case 0x65:GB_LD_r1_r2(R->HL.H, R->HL.L);    break;
        case 0x66:GB_LD_r1_r2(R->HL.H, R->HL.PAIR); break;
   
        case 0x68:GB_LD_r1_r2(R->HL.L, R->BC.B);    break;
        case 0x69:GB_LD_r1_r2(R->HL.L, R->BC.C);    break;
        case 0x6A:GB_LD_r1_r2(R->HL.L, R->DE.D);    break;
        case 0x6B:GB_LD_r1_r2(R->HL.L, R->DE.E);    break;
        case 0x6C:GB_LD_r1_r2(R->HL.L, R->HL.H);    break;
        case 0x6D:GB_LD_r1_r2(R->HL.L, R->HL.L);    break;
        case 0x6E:GB_LD_r1_r2(R->HL.L, R->HL.PAIR); break;

        case 0x70:GB_LD_r1_r2(R->HL.PAIR, R->BC.B);    break;
        case 0x71:GB_LD_r1_r2(R->HL.PAIR, R->BC.C);    break;
        case 0x72:GB_LD_r1_r2(R->HL.PAIR, R->DE.D);    break;
        case 0x73:GB_LD_r1_r2(R->HL.PAIR, R->DE.E);    break;
        case 0x74:GB_LD_r1_r2(R->HL.PAIR, R->HL.H);    break;
        case 0x75:GB_LD_r1_r2(R->HL.PAIR, R->HL.L);    break;
        case 0x76:GB_LD_r1_r2(R->HL.PAIR, GB_GET_n()); break;

         //LD A,n   
        case 0x0a: GB_LD_A_n(R->BC.PAIR); break;
        case 0x1a: GB_LD_A_n(R->DE.PAIR); break;
        case 0x7e: GB_LD_A_n(R->HL.PAIR); break;
        case 0xfa: GB_LD_A_n(R->DE.D); break;
        case 0x3e: GB_LD_A_n(GB_GET_nn()); break;
            

        //LD n,A
        case 0x47: GB_LD_n_A(R->BC.B); break;
        case 0x4F: GB_LD_n_A(R->BC.C); break;
        case 0x57: GB_LD_n_A(R->DE.D); break;
        case 0x5f: GB_LD_n_A(R->DE.E); break;
        case 0x67: GB_LD_n_A(R->HL.H); break;
        case 0x6f: GB_LD_n_A(R->HL.L); break;
        
        case 0x02: GB_LD_n_A(R->BC.PAIR); break;
        case 0x12: GB_LD_n_A(R->DE.PAIR); break;
        case 0x77: GB_LD_n_A(R->HL.PAIR); break;
        case 0xEA: GB_LD_n_A(GB_GET_nn()); break;

        // JUMPS
        case 0xC3:GB_JP_nn(opcode); break;

        case 0xC2:GB_JMP_Condition_NN(R->FLAG.Z, 0); break; 
        case 0xCA:GB_JMP_Condition_NN(R->FLAG.Z, 1); break; 
        case 0xD2:GB_JMP_Condition_NN(R->FLAG.C, 0); break;
        case 0xDA:GB_JMP_Condition_NN(R->FLAG.C, 1); break;
        case 0xE9: R->PC = R->HL.PAIR;break;
        case 0x18:  temp = R->PC; 
                    temp =+ R->PC++; break; 
        case 0x20: GB_JMP_Condition(R->FLAG.Z, 0); break;
        case 0x28: GB_JMP_Condition(R->FLAG.Z, 1); break;
        case 0x30: GB_JMP_Condition(R->FLAG.C, 0); break;
        case 0x38: GB_JMP_Condition(R->FLAG.C, 1); break;
        
        //16 Bit Loads


        //8 BIT ARITHMETIC
        
         //ADD A,n
        case 0x87: GB_ADD_A_n(R->AF.A); break;
        case 0x80: GB_ADD_A_n(R->BC.B); break;
        case 0x81: GB_ADD_A_n(R->BC.C); break;
        case 0x82: GB_ADD_A_n(R->DE.D); break;
        case 0x83: GB_ADD_A_n(R->DE.E); break;
        case 0x84: GB_ADD_A_n(R->HL.H); break;
        case 0x85: GB_ADD_A_n(R->HL.L); break;
        case 0x86: GB_ADD_A_n(R->HL.PAIR); break;
        //TF is #

        //ADC A,n
        case 0x8f: GB_ADC_A_n(R->AF.A); break;
        case 0x88: GB_ADC_A_n(R->BC.B); break;
        case 0x89: GB_ADC_A_n(R->BC.C); break;
        case 0x8a: GB_ADC_A_n(R->DE.D); break;
        case 0x8b: GB_ADC_A_n(R->DE.E); break;
        case 0x8c: GB_ADC_A_n(R->HL.H); break;
        case 0x8d: GB_ADC_A_n(R->HL.L); break;
        case 0x8e: GB_ADC_A_n(R->HL.PAIR); break;
            //TF is #

        //SUB n
        case 0x97: GB_SUB_n(R->AF.A); break;
        case 0x90: GB_SUB_n(R->BC.B); break;
        case 0x91: GB_SUB_n(R->BC.C); break;
        case 0x92: GB_SUB_n(R->DE.D); break;
        case 0x93: GB_SUB_n(R->DE.E); break;
        case 0x94: GB_SUB_n(R->HL.H); break;
        case 0x95: GB_SUB_n(R->HL.L); break;
        case 0x96: GB_SUB_n(R->HL.PAIR); break;

        //SBC A,n
        case 0x9f: GB_SBC_n(R->AF.A); break;
        case 0x98: GB_SBC_n(R->BC.B); break;
        case 0x99: GB_SBC_n(R->BC.C); break;
        case 0x9a: GB_SBC_n(R->DE.D); break;
        case 0x9b: GB_SBC_n(R->DE.E); break;
        case 0x9c: GB_SBC_n(R->HL.H); break;
        case 0x9d: GB_SBC_n(R->HL.L); break;
        case 0x9e: GB_SBC_n(R->HL.PAIR); break;

        //CP n 
        case 0xBf: GB_CP_n(R->AF.A); break;
        case 0xB8: GB_CP_n(R->BC.B); break;
        case 0xB9: GB_CP_n(R->BC.C); break;
        case 0xBa: GB_CP_n(R->DE.D); break;
        case 0xBb: GB_CP_n(R->DE.E); break;
        case 0xBc: GB_CP_n(R->HL.H); break;
        case 0xBd: GB_CP_n(R->HL.L); break;
        case 0xBe: GB_CP_n(R->HL.PAIR); break;

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
        case 0x09: R->HL.PAIR = R->BC.PAIR + R->HL.PAIR; break;
        case 0x19: R->HL.PAIR = R->DE.PAIR + R->HL.PAIR; break;
        case 0x29: R->HL.PAIR = R->HL.PAIR + R->HL.PAIR; break;
        case 0x39: R->HL.PAIR = R->SP + R->HL.PAIR; break;
            //ADD SP,n
        case 0xE8:
            //TODO
            break;
            //INC nn
        case 0x03: R->BC.PAIR++; break;
        case 0x13: R->DE.PAIR++; break;
        case 0x23: R->HL.PAIR++; break;
        case 0x33: R->SP++; break;
            //DEC nn
        case 0x0B: R->BC.PAIR--; break;
        case 0x1B: R->DE.PAIR--; break;
        case 0x2B: R->HL.PAIR--; break;
        case 0x3B: R->SP--; break;

            
        }
    }
}


