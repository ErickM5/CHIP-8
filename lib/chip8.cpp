#include <iostream>
#include "chip8.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Warming up chip 8 ..." << "\n";
    Chip8 cpu(argv[1]);
    
    std::cout << "Starting chip 8!" << "\n";
    cpu.Start(true);

    return 0;
}

Chip8::Chip8(const char* path)
{
    std::cout << "Starting Memory device" << "\n";
    mem = new Memory();

    std::cout << "Starting IODevices" << "\n";
    devcs = new IODevices(&ON_OFF, &PAUSE);
    
    std::cout << "Setting memory pointers" << "\n";
    mem->SetPtrs(0xEA0, 0xEFF, Stack);
    mem->SetPtrs(0xF00, 0xFFF, FrameBuffer);
    
    std::cout << "Inserting the program" << "\n";
    mem->CopyProgram(path);
    
    PC = 0x200;
}


void Chip8::Fetch()
{
    content = mem->Fetch(PC);

    nnn = (content >> 0) & 0xFFF;
    kk = (content >> 0) & 0xFF;
    x = (content >> 8) & 0xF;
    y = (content >> 4) & 0xF;
    n = (content >> 0) & 0xF;
    op = (content >> 12) & 0xF;
}
void Chip8::Execute()
{
    #define o(name, mnemonic, params, func) if(params){func;} else
    Opcodes(o) {}
    #undef o

    PC+=2;
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

void Chip8::Start(bool KeepAlive)
{
    ON_OFF = true;
    while (ON_OFF)
    {
        while (PAUSE){continue;}

        Fetch();
        Execute();
        
        // std::cout << "Hexadecimal value: 0x" 
        //     << std::setw(4) << std::setfill('0') << std::hex << std::uppercase << content
        //     << std::endl;
        Debug();

        devcs->StartAll(WindowActive, SoundActive);

        if ((!KeepAlive && content==0x0) || !ON_OFF)
            break;
    }

    std::cout << "Turning off CHIP-8..." << "\n";
    devcs->~IODevices();
    mem->~Memory();
}