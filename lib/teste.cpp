#include <iostream>
#include <iomanip>
#include <fstream>
#include <ios>

int main(int argc, char* argv[])
{
    char* path = argv[1];
    FILE* f = fopen(path, "rb");

    fseek(f, 0, SEEK_END);
    auto l = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    uint8_t content[l+10];

    fread(&content[10], sizeof(uint8_t), l, f);

    for (int i = 0; i < sizeof(content); i+=2)
    {
        std::cout << "0x" << std::hex << std::setfill('0') << std::setw(4) << ((content[i] << 8) | (content[i+1])) << "\n";
    }
    
    return 0;
}