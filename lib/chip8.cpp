#include <iostream>
#include "chip8.hpp"

int main(int argc, char* argv[])
{
    Chip8 cpu(argv[1]);
    
    return 0;
}

Chip8::Chip8(const char* path)
{
    InsertProgram(path);

    mem->SetPtrs(0xF00, 0xFFF, FrameBuffer);
    mem->SetPtrs(0xEA0, 0xEFF, Stack);

    PC = 0x200;
}

void Chip8::InsertProgram(const char* path)
{
    FILE* f = fopen(path, "rb");

    if(f == NULL)
        return;

    fseek(f, 0, SEEK_END);
    unsigned fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    for (int i = 0; i<fsize; i++)
        mem->Write(Chip8::memStart + i, fgetc(f));
}

void Chip8::Fetch()
{
    content = (mem->Fetch(PC & 0xFFF)*0x100 + mem->Fetch((PC+1) & 0xFFF));

    nnn = (content >> 0) & 0xFFF;
    kk = (content >> 0) & 0xFF;
    x = (content >> 8) & 0x8;
    y = (content << 8) & 0x8;
    n = (content << 12) & 0xF;
    op = (content >> 12) & 0xF;
}
void Chip8::Execute(bool KeepAlive = false)
{
    while (content != 0x0)
    {
        while (!PAUSE){continue;}

        Fetch();

        #define o(name, mnemonic, params, func) if(params){func;} else
        Opcodes(o) {}
        #undef o

        Debug();

        if (KeepAlive && OFF)
            break;
    }
}
void Chip8::Debug()
{
    #define d(params, print) if(params){print;} else
    Debugger(d) {}
    #undef o   
}

void Chip8::SetDebugText(std::string text)
{
    std::cout << text << "\n";
}