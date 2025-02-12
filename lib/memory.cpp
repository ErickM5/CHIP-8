#include "memory.hpp"

Memory::Memory(const char* path)
{
    memcpy(&mem[0], font, sizeof(font));

    std::cout << "Program path " << path << "\n";
    FILE* f = fopen(path, "rb");

    if(f == NULL)
    {
        return;
        std::cout << "Program not inserted!" << "\n";
    }
    
    fseek(f, 0, SEEK_END);
    unsigned fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::cout << "The file length is: " << fsize << "\n";

    fread(&mem[0x200], sizeof(uint8_t), fsize, f);
    // for (int i = 0; i < sizeof(mem) - 0x200; i+=2)
    // {
    //     // std::cout << "0x" << std::hex << std::setfill('0') << std::setw(4) << ((mem[i+0x200] << 8) | (mem[i+1+0x200])) << "\n";
    //     // std::cout << "PC test " << i + 0x200 << "\n";
    // }
}

void Memory::SetPtrs(uint16_t start, uint16_t end, uint8_t* list[])
{
    for (int i = 0; i < sizeof(list); i++)
        list[i] = &mem[i + start];
}
void Memory::Write(uint8_t addr, uint8_t data)
{
    if (addr < 0x1000)
        mem[addr] = data;
}
uint8_t* Memory::Fetch(uint8_t addr)
{
    // std::cout << "Opcode value 0x" << std::hex << std::setfill('0') << std::setw(4) << ((mem[addr] << 8) | (mem[addr+1])) << "\n";
    return &mem[addr];
}