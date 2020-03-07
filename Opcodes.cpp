#include "Opcodes.h"
#include "Registers.h"
#include <iostream>
#include <Windows.h>
#include "MemoryLocations.h"
#include "GraphicsInterpreter.h"
#include <fstream>
#include <vector>
bool res;

auto a     = 0 ;
uint8_t a7 = 0 ;
uint8_t ca = 0 ;
int inc2 = 0;
Registers::CPUREG* R = new Registers::CPUREG;
uint8_t n;
int argc ;
char** argv ;
uint16_t nn;
uint8_t* MEMORY_STATUS;
unsigned int temp;
static bool REGISTERS_INIT = false; 
uint8_t lsb, msb;
std::string TESTER_BIT; 
using namespace std;

void GB_INITIALIZE_REGS()
{


}

uint8_t GB_JP_nn(uint8_t nn)
{
    temp = (nn & 0x0F);
    R->PC = R->PC + temp;
    return temp;
}

void GB_JMP_Condition_NN(uint8_t FLAG, int SET_CHECK)
{
    if (FLAG == SET_CHECK)
    {
        R->PC = GB_GET_nn();
    }
}

void GB_LD_nn_n(std::string LD_nn)
{
   
    GB_RESOLVE_REG(LD_nn, MEMORY_STATUS[R->PC + 1], "NULL");
    printf("\n C VALUE AFTER LD nn n : %x, FOUND : %x",R->BC.C, MEMORY_STATUS[R->PC + 1]);
    R->PC+=2;

}

void GB_LD_r1_r2(std::string R1, uint8_t R2)
{
    GB_RESOLVE_REG(R1, R2, "NULL");
    R->PC++;
}

void GB_LD_A_n(uint16_t n)
{
    R->AF.A = R->DE.PAIR; 
    R->HL.PAIR--; 
    R->PC++;
}

void GB_LD_n_A(std::string n)
{
    GB_RESOLVE_REG(n, R->AF.A, "NULL");
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
    R->PC++;
    uint8_t msb = MEMORY_STATUS[R->PC];
    nn = lsb + msb;
    return nn;
}

uint16_t GB_GET_NN_ALT()
{
    lsb = MEMORY_STATUS[R->PC + 1];
    msb = MEMORY_STATUS[R->PC + 2];
    nn = ((msb) << 8) | (lsb);

    return nn;
}

uint8_t GB_GET_n()
{    
    n = MEMORY_STATUS[R->PC++];
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
    R->PC++;
}
void GB_XOR_n(uint8_t n)
{
    R->AF.A = R->AF.A ^ n;
    R->PC++;
}

void GB_LD_n_nn(std::string n)
{
    lsb = MEMORY_STATUS[R->PC + 1];
    msb = MEMORY_STATUS[R->PC + 2];
    nn = ((msb) << 8) | (lsb);
    if (nn == 0x104)
    {
        printf("TEST : %x, %x, %x", lsb, msb, nn);
    }
    GB_RESOLVE_REG(n, nn, "NULL");

    R->PC++;
}

void GB_RST_n(uint8_t n)
{
    MEMORY_STATUS[R->SP - 1] = (((R->PC & 0xFF00) >> 8));
    MEMORY_STATUS[R->SP - 2] = ((R->PC & 0xFF));
    
    R->SP = R->SP -= 2;
    R->PC+=2;   
}

void GB_POP_nn(std::string REG1, std::string REG2)
{
    GB_RESOLVE_REG(REG2, MEMORY_STATUS[R->SP], "NULL");
    GB_RESOLVE_REG(REG1, MEMORY_STATUS[R->SP], "NULL");
    R->SP += 2;
    R->PC++;

}

void GB_PUSH_nn(uint8_t REG1, uint8_t REG2)
{
    MEMORY_STATUS[R->SP - 2] = REG2;
    MEMORY_STATUS[R->SP - 1] = REG1;
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
    msb = MEMORY_STATUS[R->PC];
    R->PC++;

    MEMORY_STATUS[R->SP - 2] = (lsb & 0xFF);
    MEMORY_STATUS[R->SP - 1] = (msb & 0xFF00 >> 8);

    R->SP = R->SP - 2;
    R->PC = msb;

    //break;

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

void GB_LDD_HL_A()
{
    printf("\nHL: %x, A: %x", R->HL.PAIR, R->AF.A);
    // R->HL.PAIR = R->AF.A; 
    MEMORY_STATUS[R->HL.PAIR] = R->AF.A;
    R->HL.PAIR--;
    R->PC++;
}

void GB_INC_n(std::string n)
{
    GB_RESOLVE_REG(n, 1, "inc");
    R->PC++;
}

void GB_DEC_n(std::string n)
{
    GB_RESOLVE_REG(n, -1, "dec");
    R->PC++;
}
void GB_CALL_cc_nn(uint8_t FLAG, int SET_CHECK)
{
    if (FLAG == SET_CHECK)
    {
        MEMORY_STATUS[R->SP] = GB_GET_n();
    }
    R->PC+=3;

}

void GB_RESOLVE_REG(std::string REGNAME, uint16_t value, std::string op)
{
    if (REGNAME == "A")
    {
        if (op == "dec")
        {
            R->AF.A--;
        }
        else if (op == "inc")
        {
            R->AF.A++;
        }
        else if (op == "and")
        {
            R->AF.A && value;
        }
        else
        {
            R->AF.A =value;
        }
    }
    else if (REGNAME == "B")
    {
        if (op == "dec")
        {
            R->BC.B--;
        }
        else if (op == "inc")
        {
            R->BC.B++;
        }
        else
        {
            R->BC.B = value;
        }
    }

    else if (REGNAME == "C")
    {
        if (op == "dec")
        {
            R->BC.C--;
        }
        else if (op == "inc")
        {
            R->BC.C++;
        }
        else
        {
            R->BC.C = value;
        }
    }

    else if (REGNAME == "D")
    {
        if (op == "dec")
        {
            R->DE.D--;
        }
        else if (op == "inc")
        {
            R->DE.D++;
        }
        else
        {
            R->DE.D = value;
        }
    }

    else if (REGNAME == "E")
    {
        if (op == "dec")
        {
            R->DE.E--;
        }
        else if (op == "inc")
        {
            R->DE.E++;
        }
        else
        {
            R->DE.E = value;
        }
    }
    else if (REGNAME == "H")
    {
        if (op == "dec")
        {
            R->HL.H--;
        }
        else if (op == "inc")
        {
            R->HL.H++;
        }
        else {
            R->HL.H = value;
        }
    }
    else if (REGNAME == "L")
    {
        if (op == "dec")
        {
            R->HL.L--;
        }
        else if (op == "inc")
        {
            R->HL.L++;
        }
        else {
            R->HL.L = value;
        }
    }


    else if (REGNAME == "BC") {
        R->BC.PAIR = value;
    }
    else if (REGNAME == "DE") {
        R->DE.PAIR = value;
    }

    else if (REGNAME == "HL") {
        if (op == "dec")
        {
            R->HL.PAIR--;
        }
        else if (op == "inc")
        {
            R->HL.PAIR++;
        }
        else {
            R->HL.PAIR = value;
        }
    }

    else if (REGNAME == "SP") {
        R->SP = value;
    }
}

uint8_t GB_AF_Form()
{
    R->AF.PAIR = ((R->AF.A) << 8) | (R->AF.F);
    return R->AF.PAIR;
}

uint8_t GB_BC_Form()
{
    R->BC.PAIR = ((R->BC.B) << 8) | (R->BC.C);
    return R->BC.PAIR;
}
uint8_t GB_DE_Form()
{
    R->DE.PAIR = ((R->DE.D) << 8) | (R->DE.E);
    return R->DE.PAIR;
}
uint8_t GB_HL_Form()
{
    R->HL.PAIR = ((R->HL.H) << 8) | (R->HL.L);
    return R->HL.PAIR;
}

void GB_retrieveOpcodes(uint8_t* MEMORY_MAP)
{
    GB_INIT_STACK();   
    R->AF.PAIR = 0x01;
    R->AF.F = 0xB0;
    R->BC.PAIR = 0x0013;
    R->DE.PAIR = 0x00D8;
    R->HL.PAIR = 0x014D;
    R->SP = 0xFFFE;
    R->PC = 0x100;
    R->PC = 0x0;
    R->SP = 0xFFFE;
    draw(argc, argv);

    MEMORY_STATUS = MEMORY_MAP;

    while (1)
    {
        GB_interpretOpcode(MEMORY_STATUS[R->PC]);

        //DO A MEMORY DUMP 
        std::ofstream outfile("MEMDUMP.bin", std::ofstream::binary);
        outfile.write((char*)MEMORY_STATUS, 0xffff);

        printf("\n\n######## - OPCODE : %x -########", MEMORY_STATUS[R->PC]);

        printf("\nPC: %x, SP: %x, SPVAL : %x, STACKVAL: %x", R->PC, R->SP, MEMORY_STATUS[R->PC], MEMORY_STATUS[R->SP]);
        printf("\nHL:%x , H:%x , L:%x", R->HL.PAIR, R->HL.H, R->HL.L);
        printf("\nAF:%x , A: %x, F: %x", R->AF.PAIR, R->AF.A, R->AF.F);
        printf("\nBC:%x , B: %x, C: %x", R->BC.PAIR, R->BC.B, R->BC.C);
        printf("\nDE:%x , D: %x, E: %x", R->DE.PAIR, R->DE.D, R->DE.E);
        printf("\nHL:%x , H: %x, L: %x", R->HL.PAIR, R->HL.H, R->HL.L);
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


        switch (opcode)
        {
            //GB_DEC_n
            case 0x3d: GB_DEC_n("A");     break;
            case 0x05: GB_DEC_n("B");     break;
            case 0x0d: GB_DEC_n("C");     break;
            case 0x15: GB_DEC_n("D");     break;
            case 0x1d: GB_DEC_n("E");     break;
            case 0x25: GB_DEC_n("H");     break;
            case 0x2d: GB_DEC_n("L");     break;
            case 0x35: GB_DEC_n("HL");  break;


            case 0x3C: GB_INC_n("A" );     break;
            case 0x04: GB_INC_n("B" );     break;
            case 0x0C: GB_INC_n("C" );     break;
            case 0x14: GB_INC_n("D" );     break;
            case 0x1c: GB_INC_n("E" );     break;
            case 0x24: GB_INC_n("H" );     break;
            case 0x2c: GB_INC_n("L" );     break;
            case 0x34: GB_INC_n("HL");    break;
                //8 BIT LOADS

            case 0x22:
                R->HL.PAIR = R->AF.A;
                R->HL.PAIR++;
                R->PC++;
                break;

            case 0x32: GB_LDD_HL_A(); break;

            case 0xE2:
                temp = (0xff00 + (R->BC.C));
                MEMORY_STATUS[temp] = R->AF.A;
                R->PC++;
                break;

            case 0xE0: 
                temp = MEMORY_STATUS[R->PC + 1]; 
                MEMORY_STATUS[0xFF00 + temp] = R->AF.A;  
                R->PC+=2;  
                break;
                // LD nn, n
            case 0x06:GB_LD_nn_n("B"); break;
            case 0x0E:GB_LD_nn_n("C"); break;
            case 0x16:GB_LD_nn_n("D"); break;
            case 0x1E:GB_LD_nn_n("E"); break;
            case 0x26:GB_LD_nn_n("H"); break;
            case 0x2E:GB_LD_nn_n("L"); break;

                //LD r1, r2
            case 0x7F:GB_LD_r1_r2("A", R->AF.A); break;
            case 0x78:GB_LD_r1_r2("A", R->BC.B); break;
            case 0x79:GB_LD_r1_r2("A", R->BC.C); break;
            case 0x7A:GB_LD_r1_r2("A", R->DE.D); break;
            case 0x7B:GB_LD_r1_r2("A", R->DE.E); break;
            case 0x7C:GB_LD_r1_r2("A", R->HL.H); break;
            case 0x7D:GB_LD_r1_r2("A", R->HL.L); break;

            case 0x40:GB_LD_r1_r2("B", R->BC.B);    break;
            case 0x41:GB_LD_r1_r2("B", R->BC.C);    break;
            case 0x42:GB_LD_r1_r2("B", R->DE.D);    break;
            case 0x43:GB_LD_r1_r2("B", R->DE.E);    break;
            case 0x44:GB_LD_r1_r2("B", R->HL.H);    break;
            case 0x45:GB_LD_r1_r2("B", R->HL.L);    break;
            case 0x46:GB_LD_r1_r2("B", R->HL.PAIR); break;

            case 0x48:GB_LD_r1_r2("C", R->BC.B);    break;
            case 0x49:GB_LD_r1_r2("C", R->BC.C);    break;
            case 0x4A:GB_LD_r1_r2("C", R->DE.D);    break;
            case 0x4B:GB_LD_r1_r2("C", R->DE.E);    break;
            case 0x4C:GB_LD_r1_r2("C", R->HL.H);    break;
            case 0x4D:GB_LD_r1_r2("C", R->HL.L);    break;
            case 0x4E:GB_LD_r1_r2("C", R->HL.PAIR); break;

            case 0x50:GB_LD_r1_r2("D", R->BC.B);    break;
            case 0x51:GB_LD_r1_r2("D", R->BC.C);    break;
            case 0x52:GB_LD_r1_r2("D", R->DE.D);    break;
            case 0x53:GB_LD_r1_r2("D", R->DE.E);    break;
            case 0x54:GB_LD_r1_r2("D", R->HL.H);    break;
            case 0x55:GB_LD_r1_r2("D", R->HL.L);    break;
            case 0x56:GB_LD_r1_r2("D", R->HL.PAIR); break;

            case 0x58:GB_LD_r1_r2("E", R->BC.B);    break;
            case 0x59:GB_LD_r1_r2("E", R->BC.C);    break;
            case 0x5A:GB_LD_r1_r2("E", R->DE.D);    break;
            case 0x5B:GB_LD_r1_r2("E", R->DE.E);    break;
            case 0x5C:GB_LD_r1_r2("E", R->HL.H);    break;
            case 0x5D:GB_LD_r1_r2("E", R->HL.L);    break;
            case 0x5E:GB_LD_r1_r2("E", R->HL.PAIR); break;

            case 0x60:GB_LD_r1_r2("H", R->BC.B);    break;
            case 0x61:GB_LD_r1_r2("H", R->BC.C);    break;
            case 0x62:GB_LD_r1_r2("H", R->DE.D);    break;
            case 0x63:GB_LD_r1_r2("H", R->DE.E);    break;
            case 0x64:GB_LD_r1_r2("H", R->HL.H);    break;
            case 0x65:GB_LD_r1_r2("H", R->HL.L);    break;
            case 0x66:GB_LD_r1_r2("H", R->HL.PAIR); break;

            case 0x68:GB_LD_r1_r2("L", R->BC.B);    break;
            case 0x69:GB_LD_r1_r2("L", R->BC.C);    break;
            case 0x6A:GB_LD_r1_r2("L", R->DE.D);    break;
            case 0x6B:GB_LD_r1_r2("L", R->DE.E);    break;
            case 0x6C:GB_LD_r1_r2("L", R->HL.H);    break;
            case 0x6D:GB_LD_r1_r2("L", R->HL.L);    break;
            case 0x6E:GB_LD_r1_r2("L", R->HL.PAIR); break;

            case 0x70:GB_LD_r1_r2("HL", R->BC.B);    break;
            case 0x71:GB_LD_r1_r2("HL", R->BC.C);    break;
            case 0x72:GB_LD_r1_r2("HL", R->DE.D);    break;
            case 0x73:GB_LD_r1_r2("HL", R->DE.E);    break;
            case 0x74:GB_LD_r1_r2("HL", R->HL.H);    break;
            case 0x75:GB_LD_r1_r2("HL", R->HL.L);    break;
            case 0x76:GB_LD_r1_r2("HL", GB_GET_n()); break;

                //LD A,n   
            case 0x1a: GB_LD_A_n(R->DE.PAIR); break;
            case 0x7e: GB_LD_A_n(R->HL.PAIR); break;
            case 0xfa: GB_LD_A_n(R->DE.D);    break;
            case 0x0a: GB_LD_A_n(R->BC.PAIR); break;
            case 0x3e: GB_LD_A_n(MEMORY_STATUS[R->PC + 1]); R->PC++; break;

            case 0x08: R->SP = GB_GET_NN_ALT(); R->PC++; break;

                //LD n,A
            case 0x47: GB_LD_n_A("B"); break;
            case 0x4F: GB_LD_n_A("C"); break;
            case 0x57: GB_LD_n_A("D"); break;
            case 0x5f: GB_LD_n_A("E"); break;
            case 0x67: GB_LD_n_A("H"); break;
            case 0x6f: GB_LD_n_A("L"); break;

            case 0x02: GB_LD_n_A("BC"); break;
            case 0x12: GB_LD_n_A("DE"); break;
            case 0x77: GB_LD_n_A("HL"); break;
            case 0xEA: R->AF.A = GB_GET_nn(); R->PC++; break;

                //LDH A, n
            case 0xF0:GB_LDH_A_n(GB_GET_n()); break;
                // JUMPS
            case 0xC3: GB_JP_nn(opcode); break;


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
            case 0x20:

                printf("\nPC %x", R->PC);
                a = R->PC;
                R->PC += 2;
                if (R->FLAG.Z == 0)
                {
                    R->PC = R->PC + (int8_t)MEMORY_STATUS[a+1];
                    printf("\nPC %x", R->PC);
                }
                
                printf("\nPC %x", R->PC);
                break;

            case 0x28: GB_JMP_Condition(R->FLAG.Z, 1); break;
            case 0x30: GB_JMP_Condition(R->FLAG.C, 1); break;
            case 0x38: GB_JMP_Condition(R->FLAG.C, 0); break;

            case 0xC4: GB_CALL_cc_nn(R->FLAG.Z, 1); R->PC+=2; break;
            case 0xCc: GB_CALL_cc_nn(R->FLAG.Z, 0); R->PC+=2; break;
            case 0xD4: GB_CALL_cc_nn(R->FLAG.C, 1); R->PC+=2; break;
            case 0xDC: GB_CALL_cc_nn(R->FLAG.C, 1); R->PC+=2; break;


                //BIT OPCODES
            /*
             * this instruction tests bit "b" in register "r" and sets the Z (zero) flag accordingly. 
             * For example, if the bit 2 in register B contains a 0, at execution of BIT 2, B the Z flag in 
             * the F register contains 1 and bit 2 in register B remains 0.
             * 
             * In this case, we are checking bit 7 of the H register. If it is zero (unset), then we need 
             * to set the Zero (Z) flag. If it is one (set), then we need to clear the Zero (Z) flag.
            */
            case 0xCB:
                
                a = (R->HL.PAIR & 0xFF00);
                a = a >> 8;
                TESTER_BIT = GBA_ConvertHextoBin(a);
                std::cout << TESTER_BIT << endl;

                TESTER_BIT = TESTER_BIT.at(0);
                if (TESTER_BIT == "0")
                {
                    R->FLAG.Z = 1;

                }
                else if (TESTER_BIT == "1")
                {
                    R->FLAG.Z = 0;
                }
                R->PC++;
                break;

            //16 Bit Loads

            case 0x01: GB_LD_n_nn("BC"); R->PC+=2; break;
            case 0x11: GB_LD_n_nn("DE"); R->PC+=2; break;
            case 0x21: GB_LD_n_nn("HL"); R->PC+=2; break;
            case 0x31: GB_LD_n_nn("SP"); R->PC+=2; break;
                                     
            case 0xF1: GB_POP_nn("A", "F"); break;
            case 0xC1: GB_POP_nn("B", "C"); break;
            case 0xD1: GB_POP_nn("D", "E"); break;
            case 0xE1: GB_POP_nn("H", "L"); break;

            case 0xF5: GB_PUSH_nn(R->AF.A, R->AF.F); break;
            case 0xC5: GB_PUSH_nn(R->BC.B, R->BC.C); break;
            case 0xD5: GB_PUSH_nn(R->DE.D, R->DE.E); break;
            case 0xE5: GB_PUSH_nn(R->HL.H, R->HL.L); break;

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
            case 0x8f: GB_ADC_A_n(R->AF.A);   R->PC++; break;
            case 0x88: GB_ADC_A_n(R->BC.B);   R->PC++; break;
            case 0x89: GB_ADC_A_n(R->BC.C);   R->PC++; break;
            case 0x8a: GB_ADC_A_n(R->DE.D);   R->PC++; break;
            case 0x8b: GB_ADC_A_n(R->DE.E);   R->PC++; break;
            case 0x8c: GB_ADC_A_n(R->HL.H);   R->PC++; break;
            case 0x8d: GB_ADC_A_n(R->HL.L);   R->PC++; break;
            case 0x8e: GB_ADC_A_n(R->HL.PAIR);R->PC++; break;
                //TF is #

            //SUB n
            case 0x97: GB_SUB_n(R->AF.A);    R->PC++;    break;
            case 0x90: GB_SUB_n(R->BC.B);    R->PC++;    break;
            case 0x91: GB_SUB_n(R->BC.C);    R->PC++;    break;
            case 0x92: GB_SUB_n(R->DE.D);    R->PC++;    break;
            case 0x93: GB_SUB_n(R->DE.E);    R->PC++;    break;
            case 0x94: GB_SUB_n(R->HL.H);    R->PC++;    break;
            case 0x95: GB_SUB_n(R->HL.L);    R->PC++;    break;
            case 0x96: GB_SUB_n(R->HL.PAIR); R->PC++; break;

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
                if (R->PC != 0)
                {
                    R->PC = R->PC++;
                }
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
            case 0x33: R->SP++; R->PC++;  break;
                //DEC nn
            case 0x0B: R->BC.PAIR--; R->PC++; break;
            case 0x1B: R->DE.PAIR--; R->PC++; break;
            case 0x2B: R->HL.PAIR--; R->PC++; break;
            case 0x3B: R->SP--;      R->PC++; break;

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
            case 0xcd:
                GB_CALL_nn();
                break;

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
                lsb = MEMORY_STATUS[R->SP];
                msb = MEMORY_STATUS[R->SP + 1];
                GB_JP_nn(lsb + msb);
                R->PC++;
                break;

            case 0x17:
                a  = R->AF.A;
                a7 = (a >> 6) & 0xb1;
                ca = R->AF.F;

                a <<= 1;
                a |= ca;
                
                R->PC++;
                break;
            //INTERRUPTS - might not be here for long but u kno 
            case 0xFB: MEMORY_STATUS[0xFFFF] = 1; 
                       R->PC++;
                       exit(99);
                       break;


            case 0xCE: GB_ADC_A_n(R->MISC.HASH);
                
                R->PC+=2;  
                break;

            default: 
                printf("\n################# UNIMPLEMENTED OPCODE : %x #################", opcode);
                exit(-1);
        }
    }
}


