#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t x = 2;
    uint32_t y = 0b1;

    uint32_t i = 0;
    uint32_t idx = 2;

    printf("%d\n", x | y);
    printf("%d\n", i | (1u << idx));

    return 0;
}