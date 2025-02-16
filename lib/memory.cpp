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
    file_length = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::cout << "The file length is: " << file_length << "\n";

    fread(&mem[0x200], sizeof(uint8_t), file_length, f);   
}