#include <iostream>
<<<<<<< HEAD
#include <FE/experimental/asm/__x86_64_memcpy.h>
=======
#include <FE/asm/__x86_64_memcpy.h>
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948


int main()
{
    {
        alignas(32) char dest[128] {};
        alignas(32) char source[128] { "Frogman Engine SIMD memcpy implementation written in AT&T assembly code." };
        std::cout << "right before memcpy:\n" << dest << '\n';
        __x86_64_memcpy(dest, source, 19);
        std::cout << "\nright after memcpy:\n" << dest << '\n';
    }
<<<<<<< HEAD
        std::cout << "\n\n";

=======
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
    
    return 0;
}