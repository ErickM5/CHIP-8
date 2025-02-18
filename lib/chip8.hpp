#include <memory.hpp>
#include <IOdevices.hpp>

#include <cstdlib>
#include <string>
#include <iomanip>

#define Opcodes(o) \
    o("CLS", "00E0", "a", op==0x0 && y==0xE,    for(auto i : disp->FrameBuffer){i=0x0;}) \
    o("RET", "00EE", "a", op==0x0 && kk==0xEE,  PC=mem->stack[SP--]; AddPC = false;) \
    o("JP",  "1nnn", "a", op==0x1,              PC=nnn; AddPC = false;) \
    o("CALL","2nnn", "a", op==0x2,              mem->stack[++SP] = PC; PC = nnn; AddPC = false;) \
    o("SE",  "3xkk", "a", op==0x3,              PC+= *Vx == kk ? 2 : 0;) \
    o("SNE", "4xkk", "a", op==0x4,              PC+= *Vx != kk ? 2 : 0;) \
    o("SE",  "5xy0", "a", op==0x5,              PC+= *Vx == *Vy ? 2 : 0;) \
    o("LD",  "6xkk", "a", op==0x6,              *Vx=kk;) \
    o("ADD", "7xkk", "a", op==0x7,              *Vx+=kk;) \
    o("LD",  "8xy0", "a", op==0x8 && n==0x0,    *Vx=*Vy;) \
    o("OR",  "8xy1", "a", op==0x8 && n==0x1,    *Vx|=*Vy;) \
    o("AND", "8xy2", "a", op==0x8 && n==0x2,    *Vx&=*Vy;) \
    o("XOR", "8xy3", "a", op==0x8 && n==0x3,    *Vx^=*Vy;) \
    o("ADD", "8xy4", "a", op==0x8 && n==0x4,    *Vx+=*Vy; *VF=((*Vx+*Vy)>>8)!=0;*Vx+=*Vy) \
    o("SUB", "8xy5", "a", op==0x8 && n==0x5,    *Vx-=*Vy; *VF=*Vx>*Vy;) \
    o("SHR", "8xy6", "a", op==0x8 && n==0x6,    *VF=(*Vx & 0b1); *Vx=(*Vx >> 1);) \
    o("SUBN","8xy7", "a", op==0x8 && n==0x7,    *VF=*Vx>*Vy ? 1 : 0; *Vx-=*Vy) \
    o("SHL", "8xyE", "a", op==0x8 && n==0xE,    *VF=((*Vx >> 7) & 0b1); *Vx=(*Vx << 1);) \
    o("SNE", "9xy0", "a", op==0x9,              PC+= *Vx!=*Vy ? 2 : 0; AddPC = false;) \
    o("LD",  "Annn", "a", op==0xA,              I = nnn;) \
    o("JP",  "Bnnn", "a", op==0xB,              PC = V[0x0] + nnn; AddPC = false;) \
    o("RND", "Cxkk", "a", op==0xC,              *Vx=(rand()%256)&kk;) \
    o("DRW", "Dxyn", "a", op==0xD,              \
            for (unsigned i=0, ind=0x0, bb=0x0, nn=n; i<n; i++, ind = ((*Vx+i)%64 + ((*Vy+nn--)%32 * 64)) / 8, bb=disp->FrameBuffer[ind]) \
                *VF = (byte = (byte << 8) | ((~(disp->FrameBuffer[ind] ^= mem->mem[I+i]))&bb)) > 0; \
            WindowActive=true;) \
    o("SKP", "Ex9E", "a", op==0xE && kk==0x9E,  AddPC=keyb->CheckPressedKey(*Vx); PC+=AddPC ? 2 : 0;) \
    o("SKNP","ExA1", "a", op==0xE && kk==0xA1,  AddPC=!keyb->CheckPressedKey(*Vx); PC+=!AddPC ? 2 : 0;) \
    o("LD",  "Fx07", "a", op==0xF && kk==0x07,  *Vx=DT;) \
    o("LD",  "Fx0A", "a", op==0xF && kk==0x0A,  WAITKEY=true;) \
    o("LD",  "Fx15", "a", op==0xF && kk==0x15,  DT=*Vx;) \
    o("LD",  "Fx18", "a", op==0xF && kk==0x18,  SR=*Vx;) \
    o("ADD", "Fx1E", "a", op==0xF && kk==0x1E,  I+=*Vx;) \
    o("LD",  "Fx29", "a", op==0xF && kk==0x29,  I=mem->mem[(*Vx * 5)];) \
    o("LD",  "Fx33", "a", op==0xF && kk==0x33,  mem->mem[I&0xFF]=(*Vx%10); \
                                                mem->mem[(I+1)&0xFF]=((*Vx-mem->mem[I&0xFF])%100); \
                                                mem->mem[(I+2)&0xFF]=(*Vx-(mem->mem[I&0xFF]+mem->mem[(I+1)&0xFF]));) \
    o("LD",  "Fx55", "a", op==0xF && kk==0x55,  for(int i=0; i<x; i++) {mem->mem[I+i]=V[i];}) \
    o("LD",  "Fx65", "a", op==0xF && kk==0x65,  for(int i=0; i<x; i++) {V[i]=mem->mem[I+i];})

class Chip8
{
    private:
        uint8_t     V[16];  // V0 -> *VF registers
        uint16_t    I;      // Index register
        uint16_t    PC;     // Program counter register
        uint8_t     SP;     // Stack pointer register
        uint8_t     TR;     // Time register
        uint8_t     SR;     // Sound register
        uint8_t     DT;     // Delay Timer

        Memory*     mem = nullptr;      // memory object pointer
        Sounder*    sound = nullptr;    // device (sound) pointer
        Keyboard*   keyb = nullptr;     // device (keybord) pointer
        Display*    disp = nullptr;     // device (window) pointer

        bool        WindowActive = false;   // Tells if is necessary to call display device
        bool        SoundActive = true;     // Tells if is necessary to call Sound device

        bool        AddPC = true;   // Garantee if is necessary step next instruction

        unsigned    content;        // content will handle everything
        unsigned    oldcontent;     // old content will handle last content

        bool        Terminal = false;   // Debug on the Terminal
        bool        Screen = false;     // Debug on the Screen
    public:
        uint16_t    nnn;    // the address from last 3 bytes (0x1234 -> 0x234)
        uint8_t     kk;     // the address from last 2 bytes (0x1234 -> 0x34)
        uint8_t     x;      // the first register or X register (x=1 -> V[1])
        uint8_t     y;      // the second register or Y register (y=1 -> V[1])
        uint8_t     n;      // last nibble register to indenfy differents opcodes
        uint8_t     op;     // opcode value holder
        unsigned    byte;   // variable designed to handle more informations for macros

        uint8_t*    Vx = nullptr; // pre loaded register V[x]
        uint8_t*    Vy = nullptr; // pre loaded register V[y]
        uint8_t*    VF = nullptr; // pre loaded register V[0xF]

        bool        ON_OFF = true;      // Set state of it is to be on or off
        bool        PAUSE = false;      // Set state of it is to be pause or not
        bool        WAITKEY = false;    // Wait till a key be press

        Chip8(const char* path); // Constructor

        void Start(bool KeepAlive); // Main function which keep tracking the states and other devices

        void Execute();     // Execute the current opcode
        void Fetch();       // Fetch infos from program the informations

        // if Terminal then print infos in terminal, if also screen then print in the screen game also
        void Debug();
};