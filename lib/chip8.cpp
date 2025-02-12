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
    content = (mem->mem[PC] << 8) | (mem->mem[PC+1]);
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(4) << ((mem->mem[PC] << 8) | (mem->mem[PC+1])) << "\n";
    std::cout << "PC address content: " << PC << "\n";
    
    nnn = content & 0x08FF;
    kk = content & 0x00FF;
    x = (content << 4) & 0x800;
    y = (content >> 4) & 0x008;
    op = (content >> 12) & 0xF;

    PC+=2;
    // if (AddPC) // Checks if is necessary go to next opcode because some opcodes jump to some parts of memorys
    //     PC+=2;
    
    // AddPC = true; // Always reset the variable to goto next instruction
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

    // for (int i = 0; i < sizeof(mem->mem) - 0x200; i+=2)
    // {
    //     std::cout << "0x" << std::hex << std::setfill('0') << std::setw(4) << ((mem->mem[i+0x200] << 8) | (mem->mem[i+1+0x200])) << "\n";
    //     std::cout << "PC test " << i + 0x200 << "\n";
    // }; return;

    while (ON_OFF)
    {
        while (PAUSE){continue;}

        Fetch();
        // Execute();
        
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