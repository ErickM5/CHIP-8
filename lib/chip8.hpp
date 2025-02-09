#include <memory.hpp>
#include <IOdevices.hpp>

#include <cstdlib>
#include <string>
#include <iomanip>

#define Opcodes(o) \
    o("SYS", "0nnn", op==0x0 && nnn!=0 ,    PC=nnn; AddPC = false;) \
    o("CLS", "0E00", op==0x0 && y==0xE,     for(auto& i : FrameBuffer){i=0x0;}) \
    o("RET", "00EE", op==0x0 && kk==0xEE,   PC=0x200; AddPC = false;) \
    o("JP",  "1nnn", op==0x1,               PC=nnn; AddPC = false;) \
    o("CALL","2nnn", op==0x2,               SP++; PC=nnn; AddPC = false;) \
    o("SE",  "3xkk", op==0x3,               PC+= V[x] == kk ? 2 : 0;) \
    o("SNE", "4xkk", op==0x4,               PC+= V[x] != kk ? 2 : 0;) \
    o("SE",  "5xy0", op==0x5,               PC+= V[x] == V[y] ? 2 : 0;) \
    o("LD",  "6xkk", op==0x6,               V[x]=kk;) \
    o("ADD", "7xkk", op==0x7,               V[x]+=kk;) \
    o("LD",  "8xy0", op==0x8 && n==0x0,     V[x]=V[y];) \
    o("OR",  "8xy1", op==0x8 && n==0x1,     V[x]|=V[y];) \
    o("AND", "8xy2", op==0x8 && n==0x2,     V[x]&=V[y];) \
    o("XOR", "8xy3", op==0x8 && n==0x3,     V[x]^=V[y];) \
    o("ADD", "8xy4", op==0x8 && n==0x4,     V[x]+=V[y]; V[0xF]=((V[x]+=V[y])>>8)!=0;) \
    o("SUB", "8xy5", op==0x8 && n==0x5,     V[x]-=V[y]; V[0xF]=V[x]>V[y];) \
    o("SHR", "8xy6", op==0x8 && n==0x6,     V[x]=(V[x] >> 1);V[0xF]=(V[x]<<7)&0x1;) \
    o("SUBN","8xy7", op==0x8 && n==0x7,     V[x]=V[x]-V[y]; V[0xF]=V[x]<V[y];) \
    o("SHL", "8xyE", op==0x8 && n==0xE,     V[x]=(V[x] << 1);V[0xF]=(V[x]>>7)&0x1;) \
    o("SNE", "9xy0", op==0x9,               PC+= V[x]!=V[y] ? 2 : 0; AddPC = false;) \
    o("LD",  "Annn", op==0xA,               I = nnn;) \
    o("JP",  "Bnnn", op==0xB,               PC = V[0x0] + nnn; AddPC = false;) \
    o("RND", "Cxkk", op==0xC,               V[x]=(rand()%256)&kk;) \
    o("DRW", "Dxyn", op==0xD,               WindowActive = true;)

#define Debugger(d) \
    d(op==0x0 && nnn!=0 ,   SetDebugText("SYS");) \
    d(op==0x0 && y==0xE,    SetDebugText("CLS");) \
    d(op==0x0 && kk==0xEE,  SetDebugText("RET");) \
    d(op==0x1,              SetDebugText("JP");) \
    d(op==0x2,              SetDebugText("CALL");) \
    d(op==0x3,              SetDebugText("SE");) \
    d(op==0x4,              SetDebugText("SNE");) \
    d(op==0x5,              SetDebugText("SE");) \
    d(op==0x6,              SetDebugText("LD");) \
    d(op==0x7,              SetDebugText("ADD");) \
    d(op==0x8 && n==0x0,    SetDebugText("LD");) \
    d(op==0x8 && n==0x1,    SetDebugText("OR");) \
    d(op==0x8 && n==0x2,    SetDebugText("AND");) \
    d(op==0x8 && n==0x3,    SetDebugText("XOR");) \
    d(op==0x8 && n==0x4,    SetDebugText("ADD");) \
    d(op==0x8 && n==0x5,    SetDebugText("SUB");) \
    d(op==0x8 && n==0x6,    SetDebugText("SHR");) \
    d(op==0x8 && n==0x7,    SetDebugText("SUBN");) \
    d(op==0x8 && n==0xE,    SetDebugText("SHL");) \
    d(op==0x9,              SetDebugText("SNE");) \
    d(op==0xA,              SetDebugText("LD");) \
    d(op==0xB,              SetDebugText("JP");) \
    d(op==0xC,              SetDebugText("RND");) \
    d(op==0xD,              SetDebugText("DRW");)

class Chip8
{
    private:
        const uint8_t memStart = 0x200; // Start point at memory insert

        uint8_t V[16]; // V0 -> VF registers
        uint16_t I; // Index register
        uint8_t SP; // Stack pointer register
        uint8_t TR; // Time register
        uint8_t SR; // Sound register
        uint16_t PC; // Program counter register

        uint8_t* FrameBuffer[64*32/8]; // 64:32 length but each byte in horizontal represents 8 pixels (1 pixel -> 1 bit)
        uint8_t* Stack[96]; // Stack pointer list pointing to memory

        Memory* mem = nullptr; // memory object pointer
        IODevices* devcs = nullptr; // devices (sound and display) pointers
        
        bool WindowActive = false; // Tells if is necessary to call display device
        bool SoundActive = true; // Tells if is necessary to call Sound device

        bool AddPC = true; // Garantee if is necessary step next instruction

        void SetDebugText(std::string Text); // Prepare the text to generate the 

        bool Terminal = false; // Debug on the Terminal
        bool Screen = false; // Debug on the Screen

    public:
        uint16_t nnn; // the address from last 3 bytes (0x1234 -> 0x234)
        uint8_t kk; // the address from last 2 bytes (0x1234 -> 0x34)
        uint8_t x; // the first register or X register (x=1 -> V[1])
        uint8_t y; // the second register or Y register (y=1 -> V[1])
        uint8_t n; // last nibble register to indenfy differents opcodes

        uint8_t op; // opcode check

        uint16_t content = 0x1;

        bool ON_OFF = true; // Set state of it is to be on or off
        bool PAUSE = false; // Set state of it is to be pause or not

        Chip8(const char* path); // Constructor

        void Start(bool KeepAlive); // Main function which keep tracking the states and other devices

        void Execute(); // Execute the current opcode
        void Fetch(); // Fetch infos from program the informations

        // if Terminal then print infos in terminal, if also screen then print in the screen game also
        void Debug();
};