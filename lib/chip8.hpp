#include <memory.hpp>
#include <IOdevices.hpp>

#include <cstdlib>
#include <string>
#include <iomanip>

#define Opcodes(o) \
    o("CLS", "00E0", "a", op==0x0 && y==0xE,    for(auto& i : FrameBuffer){i=0x0;}) \
    o("RET", "00EE", "a", op==0x0 && kk==0xEE,  PC=0x200; AddPC = false;) \
    o("JP",  "1nnn", "a", op==0x1,              PC=nnn; AddPC = false;) \
    o("CALL","2nnn", "a", op==0x2,              SP++; PC=nnn; AddPC = false;) \
    o("SE",  "3xkk", "a", op==0x3,              PC+= V[x] == kk ? 2 : 0;) \
    o("SNE", "4xkk", "a", op==0x4,              PC+= V[x] != kk ? 2 : 0;) \
    o("SE",  "5xy0", "a", op==0x5,              PC+= V[x] == V[y] ? 2 : 0;) \
    o("LD",  "6xkk", "a", op==0x6,              V[x]=kk;) \
    o("ADD", "7xkk", "a", op==0x7,              V[x]+=kk;) \
    o("LD",  "8xy0", "a", op==0x8 && n==0x0,    V[x]=V[y];) \
    o("OR",  "8xy1", "a", op==0x8 && n==0x1,    V[x]|=V[y];) \
    o("AND", "8xy2", "a", op==0x8 && n==0x2,    V[x]&=V[y];) \
    o("XOR", "8xy3", "a", op==0x8 && n==0x3,    V[x]^=V[y];) \
    o("ADD", "8xy4", "a", op==0x8 && n==0x4,    V[x]+=V[y]; V[0xF]=((V[x]+=V[y])>>8)!=0;) \
    o("SUB", "8xy5", "a", op==0x8 && n==0x5,    V[x]-=V[y]; V[0xF]=V[x]>V[y];) \
    o("SHR", "8xy6", "a", op==0x8 && n==0x6,    V[x]=(V[x] >> 1);V[0xF]=(V[x]<<7)&0x1;) \
    o("SUBN","8xy7", "a", op==0x8 && n==0x7,    V[x]=V[x]-V[y]; V[0xF]=V[x]<V[y];) \
    o("SHL", "8xyE", "a", op==0x8 && n==0xE,    V[x]=(V[x] << 1);V[0xF]=(V[x]>>7)&0x1;) \
    o("SNE", "9xy0", "a", op==0x9,              PC+= V[x]!=V[y] ? 2 : 0; AddPC = false;) \
    o("LD",  "Annn", "a", op==0xA,              I = nnn;) \
    o("JP",  "Bnnn", "a", op==0xB,              PC = V[0x0] + nnn; AddPC = false;) \
    o("RND", "Cxkk", "a", op==0xC,              V[x]=(rand()%256)&kk;) \
    o("DRW", "Dxyn", "a", op==0xD,              ) \
    o("SKP", "Ex9E", "a", op==0xE && kk==0x9E,  AddPC=keyb->CheckPressedKey(V[x]); PC+=AddPC ? 2 : 0;) \
    o("SKNP","ExA1", "a", op==0xE && kk==0xA1,  AddPC=!keyb->CheckPressedKey(V[x]); PC+=!AddPC ? 2 : 0;) \
    o("LD",  "Fx07", "a", op==0xF && kk==0x07,  V[x]=DT;) \
    o("LD",  "Fx0A", "a", op==0xF && kk==0x0A,  WAITKEY=true;) \
    o("LD",  "Fx15", "a", op==0xF && kk==0x15,  DT=V[x];) \
    o("LD",  "Fx18", "a", op==0xF && kk==0x18,  SR=V[x];) \
    o("ADD", "Fx1E", "a", op==0xF && kk==0x1E,  I+=V[x];) \
    o("LD",  "Fx29", "a", op==0xF && kk==0x29,  I=V[x];) \
    o("LD",  "Fx33", "a", op==0xF && kk==0x33,  mem->mem[I&0xFF]=(V[x]%10); \
                                                mem->mem[(I+1)&&0xFF]=((V[x]-mem->mem[I&0xFF])%100); \
                                                mem->mem[(I+2)&&0xFF]=(V[x]-(mem->mem[I&0xFF]+mem->mem[(I+1)&&0xFF]));) \
    o("LD",  "Fx55", "a", op==0xF && kk==0x55,  int i=0; for(auto& vl : V) {mem->mem[I+(i++)]=vl;}) \
    o("LD",  "Fx65", "a", op==0xF && kk==0x65,  int i=0; for(auto& vl : V) {vl=mem->mem[I+(i++)];})

class Chip8
{
    private:
        uint8_t V[16]; // V0 -> VF registers
        uint16_t I; // Index register
        uint16_t PC; // Program counter register
        uint8_t SP; // Stack pointer register
        uint8_t TR; // Time register
        uint8_t SR; // Sound register
        uint8_t DT; // Delay Timer

        uint8_t* FrameBuffer[64*32/8]; // 64:32 length but each byte in horizontal represents 8 pixels (1 pixel -> 1 bit)
        uint8_t* Stack[96]; // Stack pointer list pointing to memory

        Memory* mem = nullptr; // memory object pointer
        Sounder* sound = nullptr; // device (sound) pointer
        Keyboard* keyb = nullptr; // device (keybord) pointer
        Display* disp = nullptr; // device (window) pointer
        
        bool WindowActive = false; // Tells if is necessary to call display device
        bool SoundActive = true; // Tells if is necessary to call Sound device

        bool AddPC = true; // Garantee if is necessary step next instruction

        void SetDebugText(std::string Text); // Prepare the text to generate the 

        unsigned content; // content will handle everything

        bool Terminal = false; // Debug on the Terminal
        bool Screen = false; // Debug on the Screen

    public:
        uint16_t nnn; // the address from last 3 bytes (0x1234 -> 0x234)
        uint8_t kk; // the address from last 2 bytes (0x1234 -> 0x34)
        uint8_t x; // the first register or X register (x=1 -> V[1])
        uint8_t y; // the second register or Y register (y=1 -> V[1])
        uint8_t n; // last nibble register to indenfy differents opcodes

        uint8_t op; // opcode check

        bool ON_OFF = true; // Set state of it is to be on or off
        bool PAUSE = false; // Set state of it is to be pause or not
        bool WAITKEY = false; // Wait till a key be press

        Chip8(const char* path); // Constructor

        void Start(bool KeepAlive); // Main function which keep tracking the states and other devices

        void Execute(); // Execute the current opcode
        void Fetch(); // Fetch infos from program the informations

        // if Terminal then print infos in terminal, if also screen then print in the screen game also
        void Debug();
};