#include "Opcodes.h"
#include "Registers.h"
#include <iostream>
#include <Windows.h>
#include "MemoryLocations.h"
#include "GraphicsInterpreter.h"

Registers::CPUREG* R = new Registers::CPUREG;
uint8_t n;
int argc ;
char** argv ;
uint16_t nn;
uint8_t* MEMORY_STATUS;
unsigned int temp;
static bool REGISTERS_INIT = false; 
uint8_t lsb, msb;

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
    R->PC++;
}

void GB_LD_r1_r2(uint8_t R1, uint8_t R2)
{
    R1 = R2; 
    R->PC++;
}

void GB_LD_A_n(uint8_t n)
{
    R->AF.A = n; 
}

void GB_LD_n_A(uint8_t n)
{
    n = R->AF.A;
    R->PC++;
}

void GB_JMP_Condition(uint8_t FLAG, uint8_t SET_CHECK)
{
    if (FLAG == SET_CHECK)
    {
        temp = R->PC;
        R->PC =+ temp + 1;
    }

    R->PC++;
}

uint16_t GB_GET_nn()
{   
    R->PC++;
    uint8_t lsb = MEMORY_STATUS[R->PC];
    std::cout << "\nLSB: " << lsb;
    R->PC++;
    uint8_t msb = MEMORY_STATUS[R->PC];
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
    R->PC++;
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
    if (temp < n)
    {
        R->FLAG.C = 1;
    }
    printf("\nZ = %x", R->FLAG.Z);
    printf("\nN = %x", R->FLAG.N);
    printf("\nH = %x", R->FLAG.H);
    printf("\nC = %x", R->FLAG.C);
    R->PC++;
}
void GB_XOR_n(uint8_t n)
{
    R->AF.A = R->AF.A ^ n;
    R->PC++;
}



// ------ ALU ------

//RST n
/* Description:
    Push present address onto stack.
        Jump to address $0000 + n.
        Use with :
    n = $00, $08, $10, $18, $20, $28, $30, $38 */

void GB_RST_n(uint8_t n)
{
    GBA_Set_Stack_ITEM(((R->PC & 0xFF00) >> 8), (R->SP - 1));
    GBA_Set_Stack_ITEM((R->PC & 0xFF), (R->SP - 2));
    R->SP = R->SP -= 2;
    std::cout << "STACK POINTER: " << R->SP;
    R->PC+=2;   
}

void GB_POP_nn(uint8_t REG1, uint8_t REG2)
{
    REG2 = GBA_Get_Stack_ITEM(R->SP);
    REG1 = GBA_Get_Stack_ITEM(R->SP + 1);
    R->SP += 2;
    R->PC++;

}

void GB_PUSH_nn(uint8_t REG1, uint8_t REG2)
{
    GBA_Set_Stack_ITEM((R->SP - 2), REG2);
    GBA_Set_Stack_ITEM((R->SP - 1) , REG1);
    R->SP = R->SP - 2;
    R->PC++;
}

void GB_AND_n(uint8_t n)
{
    R->AF.A = R->AF.A && n;
    R->PC++;
}

void GB_CALL_nn()
{
    lsb = MEMORY_STATUS[R->PC];
    R->PC++;
    printf("well.......%x", lsb);
    msb = MEMORY_STATUS[R->PC];
    R->PC++;
    printf("well.......%x", msb);
    //writeMem(SP - 1, ((PC & 0xFF00) >> 8), gbcpu.mem);

    //writeMem(SP - 2, (PC & 0xFF), gbcpu.mem);

    GBA_Set_Stack_ITEM((R->SP - 2), (MEMORY_STATUS[R->PC] & 0xFF));
    GBA_Set_Stack_ITEM((R->SP - 1), (MEMORY_STATUS[R->PC] & 0xFF00 >> 8));

    //SP -= 2;
    R->SP = R->SP - 2;
    //PC = opAux.Word;
    R->PC++;

    std::cout << "HELLOOOOOOOOOOO : " << R->PC;

    //break;

}


void ex(int &argc, char** argv)
{
    
}


void GB_retrieveOpcodes(uint8_t* MEMORY_MAP)
{
    GB_INIT_STACK(); 
    R->PC = 0x0;
    R->SP = 0xFFFE;
    draw(argc, argv);
    while (1)
    {
        std::cout << "\nCURRENT PC ISSSSS ....... " << (uint32_t) R->PC;
        MEMORY_STATUS = MEMORY_MAP;
        GB_interpretOpcode(MEMORY_STATUS[R->PC]);
       // Sleep(1000);
        
    }
}

void GB_RET_cc(uint8_t FLAG, uint8_t SET_CHECK)
{
    if (FLAG == SET_CHECK)
    {
        
    }
    R->PC++;
}
//case 0xF0: GB_LD_A_n((0xFF00 + GB_GET_n())); break;
void GB_LDH_A_n(uint8_t n)
{
    MEMORY_STATUS[0xFF00 + n] = R->AF.A;
    R->PC++;
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

        case 0xE0: MEMORY_STATUS[0xFF00 + GB_GET_n()] = R->AF.A; R->PC++;  break;
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

        //LDH A, n
        case 0xF0:GB_LDH_A_n(GB_GET_n()); break;
        // JUMPS
        case 0xC3:GB_JP_nn(opcode); break;

        case 0xC2:GB_JMP_Condition_NN(R->FLAG.Z, 1); break; 
        case 0xCA:GB_JMP_Condition_NN(R->FLAG.Z, 1); break; 
        case 0xD2:GB_JMP_Condition_NN(R->FLAG.C, 0); break;
        case 0xDA:GB_JMP_Condition_NN(R->FLAG.C, 1); break;

       // case 0xE9: R->PC = R->HL.PAIR; break;
        case 0xE9: R->PC++; break;

        case 0x18: // temp = R->PC; 
                   // temp =+ R->PC++; 
                    R->PC += 2;
                    break;
        case 0x20: GB_JMP_Condition(R->FLAG.Z, 1); break;
        case 0x28: GB_JMP_Condition(R->FLAG.Z, 1); break;
        case 0x30: GB_JMP_Condition(R->FLAG.C, 1); break;
        case 0x38: GB_JMP_Condition(R->FLAG.C, 0); break;
        
        //16 Bit Loads

        case 0xF1: GB_POP_nn(R->AF.A, R->AF.F);break;
        case 0xC1: GB_POP_nn(R->BC.B, R->BC.C);break;
        case 0xD1: GB_POP_nn(R->DE.D, R->DE.E);break;
        case 0xE1: GB_POP_nn(R->HL.H, R->HL.L);break;

        case 0xF5: GB_PUSH_nn(R->AF.A, R->AF.F);break;
        case 0xC5: GB_PUSH_nn(R->BC.B, R->BC.C);break;
        case 0xD5: GB_PUSH_nn(R->DE.D, R->DE.E);break;
        case 0xE5: GB_PUSH_nn(R->HL.H, R->HL.L);break;

        //8 BIT ARITHMETIC
        
        // XOR n 
        case 0xAf:GB_XOR_n(R->AF.A); break;
        case 0xA8:GB_XOR_n(R->BC.B); break;
        case 0xA9:GB_XOR_n(R->BC.C); break;
        case 0xAa:GB_XOR_n(R->DE.D); break;
        case 0xAb:GB_XOR_n(R->DE.E); break;
        case 0xAc:GB_XOR_n(R->HL.H); break;
        case 0xAd:GB_XOR_n(R->HL.L); break;
        case 0xAe:GB_XOR_n(R->HL.PAIR); break;

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
        case 0x9f: GB_SBC_n(R->AF.A); R->PC++; break;
        case 0x98: GB_SBC_n(R->BC.B); R->PC++; break;
        case 0x99: GB_SBC_n(R->BC.C); R->PC++; break;
        case 0x9a: GB_SBC_n(R->DE.D); R->PC++; break;
        case 0x9b: GB_SBC_n(R->DE.E); R->PC++; break;
        case 0x9c: GB_SBC_n(R->HL.H); R->PC++; break;
        case 0x9d: GB_SBC_n(R->HL.L); R->PC++; break;
        case 0x9e: GB_SBC_n(R->HL.PAIR); R->PC++; break;

        //CP n 
        case 0xBf: GB_CP_n(R->AF.A); break;
        case 0xB8: GB_CP_n(R->BC.B); break;
        case 0xB9: GB_CP_n(R->BC.C); break;
        case 0xBa: GB_CP_n(R->DE.D); break;
        case 0xBb: GB_CP_n(R->DE.E); break;
        case 0xBc: GB_CP_n(R->HL.H); break;
        case 0xBd: GB_CP_n(R->HL.L); break;
        case 0xBe: GB_CP_n(R->HL.PAIR); break;
        case 0xFe: GB_CP_n(R->MISC.HASH); R->PC++; break;

        //GB_AND_n
        case 0xA7: GB_AND_n(R->AF.A); break;
        case 0xA0: GB_AND_n(R->BC.B); break;
        case 0xA1: GB_AND_n(R->BC.C); break;
        case 0xA2: GB_AND_n(R->DE.D); break;
        case 0xA3: GB_AND_n(R->DE.E); break;
        case 0xA4: GB_AND_n(R->HL.H); break;
        case 0xA5: GB_AND_n(R->HL.L); break;
        case 0xA6: GB_AND_n(R->HL.PAIR); break;
            
            
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
        case 0x09: R->HL.PAIR = R->BC.PAIR + R->HL.PAIR; R->PC++; break;
        case 0x19: R->HL.PAIR = R->DE.PAIR + R->HL.PAIR; R->PC++;  break;
        case 0x29: R->HL.PAIR = R->HL.PAIR + R->HL.PAIR; R->PC++; break;
        case 0x39: R->HL.PAIR = R->SP + R->HL.PAIR; R->PC++; break;
            //ADD SP,n
        case 0xE8:
            //TODO
            break;
            //INC nn
        case 0x03: R->BC.PAIR++; R->PC++; break;
        case 0x13: R->DE.PAIR++; R->PC++; break;
        case 0x23: R->HL.PAIR++; R->PC++; break;
        case 0x33: R->SP++; R->PC ++;  break;
            //DEC nn
        case 0x0B: R->BC.PAIR--; break;
        case 0x1B: R->DE.PAIR--; break;
        case 0x2B: R->HL.PAIR--; break;
        case 0x3B: R->SP--; break;

        //RESTARTS
        //RST n
        /* Description:
            Push present address onto stack.
                Jump to address $0000 + n.
                Use with :
            n = $00, $08, $10, $18, $20, $28, $30, $38 */ 

        case 0xC7: GB_RST_n(0x00); break;
        case 0xCf: GB_RST_n(0x08); break;
        case 0xd7: GB_RST_n(0x10); break;
        case 0xdf: GB_RST_n(0x18); break;
        case 0xe7: GB_RST_n(0x20); break;
        case 0xef: GB_RST_n(0x28); break;
        case 0xf7: GB_RST_n(0x30); break;
        case 0xff: GB_RST_n(0x38); break;
        
        //CALLS
        // CALL nn 
        case 0xcd: GB_CALL_nn(); break;

        //RETURNS
        //RET
        case 0xC9: R->PC++; break;
        //RET CC
        case 0xC0:GB_RET_cc(R->FLAG.Z, 0);  break;
        case 0xC8:GB_RET_cc(R->FLAG.Z, 1); break;
        case 0xD0:GB_RET_cc(R->FLAG.C, 0); break;
        case 0xD8:GB_RET_cc(R->FLAG.C, 1); break;
        //RETI
        case 0xD9: 
            lsb = GBA_Get_Stack_ITEM(R->SP);
            msb = GBA_Get_Stack_ITEM(R->SP + 1);
            GB_JP_nn(lsb + msb);
            R->PC++;
            break;

        //INTERRUPTS - might not be here for long but u kno 
        case 0xFB: MEMORY_STATUS[0xFFFF] = 1; R->PC++;  break;

        case 0xCE:

            std::cout << "\n";
           
            ex(argc, argv);
            exit(22);
            break;
        default: 
            printf("\n################# UNIMPLEMENTED OPCODE : %x #################", opcode);
            exit(-1);
        }
    }
}


