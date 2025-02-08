#include "memory.hpp"


void Memory::SetPtrs(uint8_t start, uint8_t end, uint8_t* list[])
{
    for (int i = 0; i < sizeof(list); i++)
        list[i] = &mem[i + start];
}

uint8_t Memory::Fetch(uint8_t addr)
{
    if (addr < 0x1000)
        return mem[addr];
}