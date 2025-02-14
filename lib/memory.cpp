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
void Memory::Fetch(uint8_t addr, uint8_t& content)
{
    content= mem[addr];
}