#include <iostream>
#include <string.h>

int main(int argc, char* argv[])
{
    uint8_t a = 0b11110000;
    bool b[8] = {false};

    std::cout << "Before insertion, size of a? " << sizeof(a) << " size of b " << sizeof(b)  << "\n";
    for (bool& bb:b)
        std::cout << (bool)bb << "\n";

    memcpy(&b[0], &a, sizeof(a));

    std::cout << "Before insertion, size of a? " << sizeof(a) << " size of b " << sizeof(b)  << "\n";
    std::cout << "After insertion" << "\n";
    for (bool& bb:b)
        std::cout << (bool)bb << "\n";
    
    return 0;
}