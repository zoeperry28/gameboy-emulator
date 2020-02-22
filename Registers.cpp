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
    
    printf("\nBits available: %x", bits);
    printf("\nHEX: %x", edit_hex);

        switch (hexa) 
        {
        case 0x0:
            return "0000";
            break;
        case 0x1:
        case 0x10:
            return "0001";
            break;
        case 0x2:
        case 0x20:
            return "0010";
            break;
        case 0x3:
        case 0x30:
            return "0011";
            break;
        case 0x4:
        case 0x40:
            return "0100";
            break;
        case 0x5:
        case 0x50:
            return "0101";
            break;
        case 0x6:
        case 0x60:
            return "0110";
            break;
        case 0x7:
        case 0x70:
            return "0111";
            break;
        case 0x8:
        case 0x80:
            return "1000";
            break;
        case 0x9:
        case 0x90:
            return "1001";
            break;
        case 0xA:
        case 0xA0:
            return "1010";
            break;
        case 0xB:
        case 0xB0:
            return "1011";
            break;
        case 0xC:
        case 0xC0:
            return "1100";
            break;
        case 0xD:
        case 0xD0:
            return "1101";
            break;
        case 0xE:
        case 0xE0:
            return "1110";
            break;
        case 0xF:
        case 0xF0:
            return "1111";
            break;
        default : 
            current_bin = +"FFS";
        }
       
    
    return current_bin;
}