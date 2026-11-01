/*
 * SOAL PREDIKSI OUTPUT
 * ====================
 * Tebak output setiap baris SEBELUM menjalankan program.
 * Tulis tebakanmu di komentar, lalu jalankan:  make prediksi
 * Pembahasan ada di bagian bawah solusi.c.
 *
 * Asumsi: Linux x86-64, GCC/Clang.
 */

#include <stdio.h>
#include <stdint.h>

int main(void) {
    /* 1. Tebakan: ____ */
    printf(" 1: %d\n", -1 < 0U);

    /* 2. Tebakan: ____ */
    unsigned char a = 200, b = 100;
    printf(" 2: %d\n", a + b);

    /* 3. Tebakan: ____ */
    uint8_t c = a + b;
    printf(" 3: %d\n", c);

    /* 4. Tebakan: ____ */
    uint32_t x = 0x12345678;
    unsigned char *p = (unsigned char *)&x;
    printf(" 4: %02x\n", p[0]);

    /* 5. Tebakan: ____ */
    char ch = (char)0x80;
    printf(" 5: %d\n", ch);

    /* 6. Tebakan: ____ */
    short s = -1;
    unsigned int u = s;
    printf(" 6: %x\n", u);

    /* 7. Tebakan: ____ */
    unsigned short us = 0xFFFF;
    unsigned int u2 = us;
    printf(" 7: %x\n", u2);

    /* 8. Tebakan: ____ */
    int neg = -8;
    printf(" 8: %d\n", neg >> 1);

    /* 9. Tebakan: ____ */
    printf(" 9: %x\n", (unsigned int)neg >> 1);

    /* 10. Tebakan: ____ */
    printf("10: %zu\n", sizeof('a'));

    /* 11. Tebakan: ____ */
    float f = 16777217;
    printf("11: %.0f\n", f);

    /* 12. Tebakan: ____ */
    uint8_t v = 255;
    v++;
    printf("12: %d\n", v);

    /* 13. Tebakan: ____ */
    unsigned char ff = 0xFF;
    printf("13: %d\n", ~ff == 0);

    /* 14. Tebakan: ____ */
    printf("14: %d\n", (int)(unsigned char)-1);

    return 0;
}
