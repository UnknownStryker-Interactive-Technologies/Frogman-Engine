#include <iostream>
#include <FE/experimental/asm/__x86_64_memcpy.h>


int main()
{
    {
        alignas(32) char dest[128] {};
        alignas(32) char source[128] { "Frogman Engine SIMD memcpy implementation written in AT&T assembly code." };
        std::cout << "right before memcpy:\n" << dest << '\n';
        __x86_64_memcpy(dest, source, 19);
        std::cout << "\nright after memcpy:\n" << dest << '\n';
    }
        std::cout << "\n\n";

    
    return 0;
}