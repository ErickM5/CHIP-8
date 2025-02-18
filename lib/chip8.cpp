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
    disp = new Display(VF);
    disp->Init();

    sound = new Sounder();
    keyb = new Keyboard(&ON_OFF, &PAUSE, &WAITKEY);

    PC = 0x200;
}

void Chip8::Fetch()
{
    oldcontent = content;
    content = mem->mem[PC & 0xFFF]*0x100 + mem->mem[(PC+1) & 0xFFF];

    nnn = content & 0xFFF;
    kk = content & 0xFF;
    x = (content >> 8) & 0xF;
    y = (content >> 4) & 0xF;
    op = (content >> 12) & 0xF;

    if (AddPC)
        PC+=2;

    AddPC = true;
}
void Chip8::Execute()
{
    #define o(name, mnemonic, description, params, func) if(params){func;} else
    Opcodes(o) {}
    #undef o
}

void Chip8::Debug()
{
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(4) << content << "\n";

    unsigned Type = Terminal;

    if(Terminal)
    {
        // Debug in terminal texts
    }
    if(Screen)
    {
        // Debug in screen texts
    }
}

void Chip8::Start(bool KeepAlive)
{
    ON_OFF = true;
    PC = 0x200;

    VF = &V[0xF];
    Vx = &V[x];
    Vy = &V[y];

    while (ON_OFF)
    {
        keyb->HandleEvent();
        if (WindowActive) {disp->Print(); WindowActive=false;}
        SoundActive ? sound->Play(false) : sound->Stop();

        if (PAUSE || WAITKEY) continue;

        Fetch();
        Execute();
        // Debug();

        if (!KeepAlive || !ON_OFF)
            break;
    }

    std::cout << "Turning off CHIP-8..." << "\n";

    disp->~Display();
    mem->~Memory();
}