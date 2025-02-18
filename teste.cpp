#include <iostream>
#include <bitset>

int main()
{
    unsigned char b1 = 0b11110000;
    unsigned char b2 = 0xF0;
    std::cout << "original bits " << std::bitset<8>(b1) << " new bits " << std::bitset<8>(((~(b1 ^ b2)) & b1))  << "\n";
    return 0;
}