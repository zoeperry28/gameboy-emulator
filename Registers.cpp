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
    uint8_t temp[2];
    /* First break down the hex number into digits */
    current_bin = ""; 
    temp[0] = hexa & 0xF0; 
    temp[0] = temp[0] >> 4;
    temp[1] = hexa & 0x0F;
    temp[1] = temp[1];

    printf("\nBits available: %x", hexa);
    printf("\nHEX1,: %x, HEX2,: %x", temp[0], temp[1]);

    while (i < 2)
    {
        switch (temp[i])
        {
        case 0x0:
            current_bin+= "0000";
            break;
        case 0x1:
        case 0x10:
            current_bin+= "0001";
            break;
        case 0x2:
        case 0x20:
            current_bin+= "0010";
            break;
        case 0x3:
        case 0x30:
            current_bin += "0011";
            break;
        case 0x4:
        case 0x40:
            current_bin += "0100";
            break;
        case 0x5:
        case 0x50:
            current_bin += "0101";
            break;
        case 0x6:
        case 0x60:
            current_bin += "0110";
            break;
        case 0x7:
        case 0x70:
            current_bin += "0111";
            break;
        case 0x8:
        case 0x80:
            current_bin += "1000";
            break;
        case 0x9:
        case 0x90:
            current_bin += "1001";
            break;
        case 0xA:
        case 0xA0:
            current_bin += "1010";
            break;
        case 0xB:
        case 0xB0:
            current_bin += "1011";
            break;
        case 0xC:
        case 0xC0:
            current_bin += "1100";
            break;
        case 0xD:
        case 0xD0:
            current_bin += "1101";
            break;
        case 0xE:
        case 0xE0:
            current_bin += "1110";
            break;
        case 0xF:
        case 0xF0:
            current_bin += "1111";
            break;
        default:
            current_bin = +"FFS";
        }
        i++;
    
    }
    std::cout << current_bin << endl;
    return current_bin;
}