#pragma once

#ifndef __BIOS_H__
#define __BIOS_H__

#include <string>
#include "MemoryLocations.h"

static uint8_t* MEMORY_MAP = { 0 };
static myTest t;
bool check_logo(char* BIOS_ROM, char* GAME_ROM);

class BIOS_SET
{
    public:

		char* get_bios(std::string BIOSNAME);

		char* get_rom(std::string ROMNAME);

		void startup_sequence();
};

#endif

