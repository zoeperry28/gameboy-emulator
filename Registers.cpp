#include "Registers.h"
#include <cstdint>
#include <string>
#include <bitset>
#include <iostream>
using namespace std;

string current_bin = ""; 
uint8_t OP = 0u; 



std::string GBA_ConvertHextoBin(uint8_t hexa)
{
    int bits = 0u;
    int i = 0;
    uint8_t edit_hex = hexa; 
    uint8_t temp = 0u; 
    uint8_t temp_2 = 0u;
    /* First break down the hex number into digits */
    bits = sizeof(hexa)* CHAR_BIT / 4;

    printf("\nBits available: %x", bits);
    printf("\nHEX: %x", edit_hex);

    for (i = 0; i < bits; i++)
    {
        if (i == 1)
        {
            temp = edit_hex & 0x0f;
            printf("\n1 : NEW HEX: %x", temp);
        }
        if (i == 0)
        {
            temp = edit_hex & 0xF0;
            printf("\n0: NEW HEX: %x", temp);
        }

        switch (temp) 
        {
        case 0x0:
            current_bin = current_bin + "0000";
            break;
        case 0x1:
        case 0x10:
            current_bin = current_bin + "0001";
            break;
        case 0x2:
        case 0x20:
            current_bin = current_bin + "0010";
            break;
        case 0x3:
        case 0x30:
            current_bin = current_bin + "0011";
            break;
        case 0x4:
        case 0x40:
            current_bin = current_bin + "0100";
            break;
        case 0x5:
        case 0x50:
            current_bin = current_bin + "0101";
            break;
        case 0x6:
        case 0x60:
            current_bin = current_bin + "0110";
            break;
        case 0x7:
        case 0x70:
            current_bin = current_bin + "0111";
            break;
        case 0x8:
        case 0x80:
            current_bin = current_bin + "1000";
            break;
        case 0x9:
        case 0x90:
            current_bin = current_bin + "1001";
            break;
        case 0xA:
        case 0xA0:
            current_bin = current_bin + "1010";
            break;
        case 0xB:
        case 0xB0:
            current_bin = current_bin + "1011";
            break;
        case 0xC:
        case 0xC0:
            current_bin = current_bin + "1100";
            break;
        case 0xD:
        case 0xD0:
            current_bin = current_bin + "1101";
            break;
        case 0xE:
        case 0xE0:
            current_bin = current_bin + "1110";
            break;
        case 0xF:
        case 0xF0:
            current_bin = current_bin + "1111";
            break;
        default : 
            current_bin = +"FFS";
        }
       
    }
    return current_bin;
}