#include <stdint.h>

struct Memory
{
    private:
        uint8_t mem[64*32] = {0};

    public:
        void SetPtrs(uint8_t start, uint8_t end, uint8_t* list[]);
        uint8_t Fetch(uint8_t addr);
};