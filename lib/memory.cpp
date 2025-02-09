#include "memory.hpp"


Memory::Memory()
{
    memcpy(&font, &mem[0], sizeof(font));
}

void Memory::SetPtrs(uint8_t start, uint8_t end, uint8_t* list[])
{
    for (int i = 0; i < sizeof(list); i++)
        list[i] = &mem[i + start];
}
void Memory::Write(uint8_t addr, uint8_t data)
{
    if (addr < 0x1000)
        mem[addr] = data;
}
uint8_t Memory::Fetch(uint8_t addr)
{
    if (addr < 0x1000)
        return mem[addr];
}