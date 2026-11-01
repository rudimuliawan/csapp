/*
 * SOLUSI LATIHAN BYTE MANIPULATION
 * Jangan dibuka sebelum mencoba sendiri!
 * Cek solusi ini lolos semua test:  make solusi
 */

#include "latihan.h"

/* ===== LEVEL 1 ===== */

uint32_t get_byte(uint32_t x, int n) {
    /* Geser byte ke-n ke posisi paling bawah, lalu mask. n*8 == n<<3. */
    return (x >> (n << 3)) & 0xFF;                       /* 3 op */
}

uint32_t set_byte(uint32_t x, int n, uint32_t b) {
    int s = n << 3;
    /* Simpan 0xFF ke variabel uint32_t dulu: literal 0xFF bertipe int,
     * dan 0xFF << 24 overflow int (undefined behavior). */
    uint32_t mask = 0xFF;
    /* Kosongkan byte ke-n, lalu isi dengan b yang sudah digeser. */
    return (x & ~(mask << s)) | ((b & mask) << s);       /* 7 op */
}

uint32_t byte_swap(uint32_t x) {
    uint32_t m8 = 0xFF << 8;
    uint32_t m16 = 0xFF << 16;
    return (x << 24)            /* byte 0 -> byte 3 */
         | ((x << 8) & m16)     /* byte 1 -> byte 2 */
         | ((x >> 8) & m8)      /* byte 2 -> byte 1 */
         | (x >> 24);           /* byte 3 -> byte 0 */  /* 11 op */
}

uint32_t swap_two_bytes(uint32_t x, int n, int m) {
    int sn = n << 3;
    int sm = m << 3;
    /* t = byte_n XOR byte_m. Meng-XOR t ke kedua posisi menukar isinya:
     * byte_n ^ t == byte_m, dan byte_m ^ t == byte_n.
     * Jika n == m, t = 0 sehingga x tidak berubah. */
    uint32_t t = ((x >> sn) ^ (x >> sm)) & 0xFF;
    return x ^ (t << sn) ^ (t << sm);                    /* 10 op */
}

int32_t sign_extend_byte(uint32_t x) {
    /* Trik (v ^ 0x80) - 0x80: membalik sign bit lalu menguranginya
     * memetakan 0x00..0x7F ke 0..127 dan 0x80..0xFF ke -128..-1.
     * Pengurangan 0x80 ditulis sebagai + ~0x7F, karena -0x80 == ~0x7F. */
    return ((x & 0xFF) ^ 0x80) + ~0x7F;                  /* 4 op */
}

/* ===== LEVEL 2 ===== */

uint32_t rotate_left(uint32_t x, int n) {
    /* (32 - n) & 31 ditulis sebagai (~n + 33) & 31.
     * Saat n == 0, geser kanan menjadi >> 0 (bukan >> 32 yang UB),
     * dan x | x == x, jadi tetap benar. */
    return (x << n) | (x >> ((~n + 33) & 31));           /* 6 op */
}

uint32_t reverse_bits_byte(uint32_t b) {
    b = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4);   /* tukar nibble      */
    b = ((b & 0xCC) >> 2) | ((b & 0x33) << 2);   /* tukar pasangan    */
    b = ((b & 0xAA) >> 1) | ((b & 0x55) << 1);   /* tukar bit tetangga */
    return b;                                            /* 15 op */
}

int has_zero_byte(uint32_t x) {
    uint32_t ones = 0x01 | (0x01 << 8);
    uint32_t highs;
    ones = ones | (ones << 16);          /* 0x01010101 */
    highs = ones << 7;                   /* 0x80808080 */
    /* x - 0x01010101 menyalakan bit tertinggi sebuah byte jika byte itu
     * 0x00 (karena borrow) atau >= 0x81. Bagian & ~x membuang kasus
     * byte yang memang sudah >= 0x80. Hasilnya bukan 0 tepat ketika ada
     * byte nol. */
    return !!((x + ~ones + 1) & ~x & highs);             /* 13 op */
}

uint32_t round_up_pow2(uint32_t x) {
    /* Kurangi 1 dulu supaya pangkat 2 tidak naik. Lalu "sebarkan" bit 1
     * tertinggi ke semua bit di bawahnya, sehingga x menjadi 2^k - 1. */
    x = x + ~0;                          /* x - 1 */
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;                                        /* 13 op */
}

int count_ones(uint32_t x) {
    uint32_t m1 = 0x55 | (0x55 << 8);
    uint32_t m2 = 0x33 | (0x33 << 8);
    uint32_t m4 = 0x0F | (0x0F << 8);
    m1 = m1 | (m1 << 16);                /* 0x55555555 */
    m2 = m2 | (m2 << 16);                /* 0x33333333 */
    m4 = m4 | (m4 << 16);                /* 0x0F0F0F0F */

    /* Tiap pasangan bit berisi jumlah bit 1-nya (0..2).
     * x - ((x >> 1) & m1), dengan pengurangan via ~y + 1. */
    x = x + ~((x >> 1) & m1) + 1;
    /* Tiap kelompok 4 bit berisi jumlahnya (0..4). */
    x = (x & m2) + ((x >> 2) & m2);
    /* Tiap byte berisi jumlahnya (0..8). */
    x = (x + (x >> 4)) & m4;
    /* Jumlahkan keempat byte ke byte terendah. */
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x3F;                                     /* 29 op */
}

/* ===== LEVEL 3 ===== */

int is_little_endian(void) {
    uint32_t v = 1;
    /* unsigned char boleh meng-alias objek apa pun (aman dari strict
     * aliasing). Di little endian, byte di alamat terendah adalah LSB. */
    return *(const unsigned char *)&v == 1;
}

uint32_t read_be32(const uint8_t *p) {
    /* Cast ke uint32_t sebelum shift: tanpa cast, p[0] dipromosikan ke
     * int, dan 0x80 << 24 akan overflow int (undefined behavior). */
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16)
         | ((uint32_t)p[2] << 8)  |  (uint32_t)p[3];
}

void write_le32(uint8_t *p, uint32_t v) {
    /* Shift bekerja pada NILAI, jadi kode ini benar di endian apa pun. */
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

void reverse_bytes(void *obj, size_t len) {
    unsigned char *p = obj;
    size_t i, j;
    if (len < 2) return;
    for (i = 0, j = len - 1; i < j; i++, j--) {
        unsigned char t = p[i];
        p[i] = p[j];
        p[j] = t;
    }
}

/* ===== LEVEL 4 ===== */

int parse_header(const uint8_t *buf, size_t len, struct header *out) {
    if (len < 8 || buf[0] != 1) return -1;
    out->version = buf[0];
    out->flags   = buf[1];
    out->length  = (uint16_t)((buf[2] << 8) | buf[3]);
    out->seq     = read_be32(buf + 4);
    return 0;
}
/* Kenapa tidak menyalin struct langsung dari buf?
 *  1. Endianness: field multi-byte di buf big endian, sedangkan x86
 *     little endian, jadi length dan seq akan terbalik.
 *  2. Padding dan layout struct ditentukan compiler, tidak dijamin sama
 *     dengan format di jaringan.
 *  3. buf bisa tidak aligned, dan cast (struct header *)buf melanggar
 *     strict aliasing.                                                  */

size_t build_header(const struct header *h, uint8_t *buf, size_t cap) {
    if (cap < 8) return 0;
    buf[0] = h->version;
    buf[1] = h->flags;
    buf[2] = (uint8_t)(h->length >> 8);
    buf[3] = (uint8_t)h->length;
    buf[4] = (uint8_t)(h->seq >> 24);
    buf[5] = (uint8_t)(h->seq >> 16);
    buf[6] = (uint8_t)(h->seq >> 8);
    buf[7] = (uint8_t)h->seq;
    return 8;
}

/* =====================================================================
 * PEMBAHASAN prediksi.c (urutan sesuai nomor soal)
 * =====================================================================
 *  1. 0      -1 dikonversi ke unsigned (UMax), jadi -1 < 0U bernilai false.
 *  2. 300    Kedua unsigned char dipromosikan ke int sebelum dijumlahkan.
 *  3. 44     Disimpan kembali ke uint8_t: 300 mod 256 = 44.
 *  4. 78     Little endian: byte di alamat terendah adalah LSB (0x78).
 *  5. -128   char bertipe signed di x86; 0x80 dibaca sebagai -128.
 *            (Di ARM Linux, char unsigned sehingga hasilnya 128.)
 *  6. ffffffff  short -1 di-sign-extend ke 32 bit, lalu dibaca unsigned.
 *  7. ffff   unsigned short 0xFFFF di-zero-extend: tidak ada sign bit.
 *  8. -4     Right shift signed di GCC/x86 adalah arithmetic shift.
 *  9. 7ffffffc  Unsigned memakai logical shift: bit kiri diisi 0.
 * 10. 4      Di C, literal karakter 'a' bertipe int, bukan char
 *            (di C++ hasilnya 1).
 * 11. 16777216  float hanya punya 24 bit significand; 2^24 + 1 dibulatkan.
 * 12. 0      uint8_t 255 + 1 wrap ke 0 (unsigned, bukan UB).
 * 13. 0      ff dipromosikan ke int 0x000000FF sebelum ~ dihitung, jadi
 *            ~ff == 0xFFFFFF00 (int -256), bukan 0. Jebakan ini sering
 *            muncul saat membandingkan checksum bertipe uint8_t.
 * 14. 255    (unsigned char)-1 == 255.
 */
