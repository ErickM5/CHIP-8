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
    mem = new Memory(path);

    std::cout << "Starting IODevices" << "\n";
    devcs = new IODevices(&ON_OFF, &PAUSE);
    
    std::cout << "Setting memory pointers" << "\n";
    mem->SetPtrs(0xEA0, 0xEFF, Stack);
    mem->SetPtrs(0xF00, 0xFFF, FrameBuffer);
    
    std::cout << "Inserting the program" << "\n";
    
    PC = 0x200;
}


void Chip8::Fetch()
{   
    unsigned content = mem->mem[PC & 0xFFF]*0x100 + mem->mem[(PC+1) & 0xFFF];
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(4) << content << "\n";

    nnn = content & 0xFFF;
    kk = content & 0xFF;
    x = (content >> 8) & 0xF;
    y = (content >> 4) & 0xF;
    op = (content >> 12) & 0xF;

    if (AddPC) // Checks if is necessary go to next opcode because some opcodes jump to some parts of memorys
        PC+=2;
    
    AddPC = true; // Always reset the variable to goto next instruction
}
void Chip8::Execute()
{
    #define o(name, mnemonic, params, func) if(params){func;} else
    Opcodes(o) {}
    #undef o
}
void Chip8::Debug()
{
    #define d(params, print) if(params){print;} else
    Debugger(d) {}
    #undef d
}

void Chip8::SetDebugText(std::string text)
{
    std::cout << text << "\n";
}

void Chip8::Start(bool KeepAlive)
{
    ON_OFF = true;
    PC = 0x200;

    while (ON_OFF)
    {
        while (PAUSE) // Start a loop if is pause and keep checking if needs to change anything in IO devices
        {
            devcs->StartAll(WindowActive, SoundActive);
            continue;
        }
        
        devcs->StartAll(WindowActive, SoundActive);

        Fetch();
        Execute();
        Debug();

        if (!KeepAlive || !ON_OFF)
            break;
    }

    std::cout << "Turning off CHIP-8..." << "\n";
    devcs->~IODevices();
    mem->~Memory();
}