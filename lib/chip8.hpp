#include <stdint.h>
#include <memory.hpp>

struct Chip8
{
    private:
        uint8_t V[16]; // V0 -> VF registers
        uint16_t I; // Index register
        uint8_t SP; // Stack pointer register
        uint8_t TR; // Time register
        uint8_t SR; // Sound register
        uint16_t PC; // Program counter register

        uint8_t* FrameBuffer[64*32/8]; // 64:32 length but each byte in horizontal represents 8 pixels (1 pixel -> 1 bit)
        uint8_t* Stack[32]; // Stack pointer list pointing to memory

        Memory* mem; // memory object pointer        

    public:
        uint16_t nnn; // the address from last 3 bytes (0x1234 -> 0x234)
        uint8_t kk; // the address from last 2 bytes (0x1234 -> 0x34)
        uint8_t x; // the first register or X register (x=1 -> V[1])
        uint8_t y; // the second register or Y register (y=1 -> V[1])
        uint8_t n; // last nibble register to indenfy differents opcodes

        Chip8(); // Constructor

        void InsertProgram(char* path);
        void Execute(); // Execute the current opcode
        void Fetch(); // Fetch infos from program the informations

        // if Terminal then print infos in terminal, if also screen then print in the screen game also
        void Debug(bool Terminal = false, bool Screen = false);
};