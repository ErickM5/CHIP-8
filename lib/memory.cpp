#include "memory.hpp"

Memory::Memory()
{
    memcpy(&mem[0], font, sizeof(font));
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
uint16_t Memory::Fetch(uint16_t PC)
{
    return mem[PC&0xFFF]*0x100 + mem[(PC+1)&0xFFF];
}
void Memory::CopyProgram(const char* path)
{
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

    std::cout << "Program inserted successfully!" << "\n";
}