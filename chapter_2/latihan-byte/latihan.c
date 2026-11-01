/*
 * LATIHAN BYTE MANIPULATION
 * =========================
 * Isi setiap fungsi di file ini, lalu jalankan:  make
 * Test akan menampilkan PASS/FAIL beserta contoh input yang gagal.
 *
 * ATURAN "PUZZLE" (Level 1 dan 2), mirip Data Lab CSAPP:
 *   - Operator yang boleh:  ! ~ & ^ | + << >>
 *   - TIDAK boleh: - (pengurangan/negasi), * / %, if, ?:, loop,
 *     pemanggilan fungsi, cast, array, pointer.
 *   - Konstanta hanya 0x00 sampai 0xFF. Konstanta lebih besar harus
 *     dibangun sendiri, misalnya 0xFF00 = 0xFF << 8.
 *   - Boleh memakai variabel lokal dan assignment.
 *   - "Maks op" = batas jumlah operator. Coba penuhi, tapi yang
 *     terpenting jawabanmu benar dan kamu paham alasannya.
 *   - Tips: -x sama dengan ~x + 1.
 *   - AWAS: literal seperti 0xFF bertipe int, jadi 0xFF << 24 overflow
 *     int (undefined behavior). Simpan dulu ke variabel uint32_t,
 *     misalnya  uint32_t m = 0xFF;  lalu geser m << 24.
 *     Test dikompilasi dengan -fsanitize=undefined, jadi UB seperti ini
 *     akan dilaporkan sebagai "runtime error" walaupun hasilnya benar.
 *
 * ATURAN Level 3 dan 4: C bebas (loop, if, pointer boleh), tetapi
 *   jangan memakai memcpy, htonl/ntohl, atau __builtin_*.
 *   Kodenya harus benar di mesin little endian MAUPUN big endian.
 *
 * Penomoran byte: byte 0 = byte paling tidak signifikan (LSB).
 * Contoh: x = 0x12345678 -> byte 0 = 0x78, byte 3 = 0x12.
 */

#include "latihan.h"

/* =====================================================================
 * LEVEL 1: dasar byte
 * ===================================================================== */

/*
 * 1.1 get_byte: ambil byte ke-n dari x (0 <= n <= 3).
 *   get_byte(0x12345678, 1) == 0x56
 * Maks op: 3
 */
uint32_t get_byte(uint32_t x, int n) {
    uint32_t m = 0xFF;
    x = x >> (n << 3);
    x = x & m;
    return x;
}

/*
 * 1.2 set_byte: ganti byte ke-n dari x dengan b (0 <= n <= 3).
 *   Hanya 8 bit terendah dari b yang dipakai.
 *   set_byte(0x12345678, 2, 0xAB) == 0x12AB5678
 *   set_byte(0x12345678, 0, 0x1FF) == 0x123456FF
 * Maks op: 8
 */
uint32_t set_byte(uint32_t x, int n, uint32_t b) {
    uint32_t m = 0xFF;
    m = ~(m << (n << 3));

    x &= m;

    b &= 0xFF;
    b = b << (n << 3);

    x ^= b;

    return x;
}

/*
 * 1.3 byte_swap: balik urutan keempat byte (konversi endianness).
 *   byte_swap(0x12345678) == 0x78563412
 * Maks op: 15
 */
uint32_t byte_swap(uint32_t x) {
    uint32_t m1 = 0xFF << 8;
    uint32_t m2 = 0xFF << 16;

    uint32_t b0 = x >> 24;
    uint32_t b1 = (x >> 8) & m1;
    uint32_t b2 = (x << 8) & m2;
    uint32_t b3 = x << 24; 

    return b0 | b1 | b2 | b3;
}

/*
 * 1.4 swap_two_bytes: tukar byte ke-n dan byte ke-m (0 <= n, m <= 3).
 *   swap_two_bytes(0x12345678, 1, 3) == 0x56341278
 *   swap_two_bytes(0xDEADBEEF, 0, 2) == 0xDEEFBEAD
 * Maks op: 16
 * Petunjuk: XOR punya sifat a ^ b ^ b == a.
 */
uint32_t swap_two_bytes(uint32_t x, int n, int m) {
    uint32_t n_ = n << 3;
    uint32_t m_ = m << 3;

    uint32_t x1 = x >> n_;
    uint32_t x2 = x >> m_;

    uint32_t x_or = x1 ^ x2;
    x_or = x_or & 0xFF;

    x = x ^ (x_or << n_);
    x = x ^ (x_or << m_);

    return x;
}

/*
 * 1.5 sign_extend_byte: tafsirkan byte 0 dari x sebagai bilangan
 *   signed 8 bit (two's complement), lalu kembalikan nilainya.
 *   sign_extend_byte(0x0000007F) == 127
 *   sign_extend_byte(0x00000080) == -128
 *   sign_extend_byte(0x123456FF) == -1
 * Maks op: 4
 * Konversi implisit dari uint32_t ke int32_t saat return diizinkan.
 */
int32_t sign_extend_byte(uint32_t x) {
    return 0;
}

/* =====================================================================
 * LEVEL 2: trik bit
 * ===================================================================== */

/*
 * 2.1 rotate_left: putar bit x ke kiri sebanyak n (0 <= n <= 31).
 *   Bit yang keluar di kiri masuk lagi di kanan.
 *   rotate_left(0x87654321, 4) == 0x76543218
 *   rotate_left(0x12345678, 0) == 0x12345678
 * Maks op: 8
 * Awas: x >> 32 adalah undefined behavior.
 */
uint32_t rotate_left(uint32_t x, int n) {
    return 0;
}

/*
 * 2.2 reverse_bits_byte: balik urutan 8 bit dalam b (0 <= b <= 255).
 *   reverse_bits_byte(0x01) == 0x80
 *   reverse_bits_byte(0xB0) == 0x0D    (1011 0000 -> 0000 1101)
 * Maks op: 15
 */
uint32_t reverse_bits_byte(uint32_t b) {
    return 0;
}

/*
 * 2.3 has_zero_byte: kembalikan 1 jika salah satu dari 4 byte x
 *   bernilai 0x00, selain itu 0.
 *   has_zero_byte(0x12005678) == 1
 *   has_zero_byte(0x01010101) == 0
 * Maks op: 20
 * Konteks: trik ini dipakai implementasi strlen() yang cepat.
 */
int has_zero_byte(uint32_t x) {
    return 0;
}

/*
 * 2.4 round_up_pow2: pangkat 2 terkecil yang >= x (1 <= x <= 2^31).
 *   round_up_pow2(1) == 1
 *   round_up_pow2(5) == 8
 *   round_up_pow2(64) == 64
 * Maks op: 13
 */
uint32_t round_up_pow2(uint32_t x) {
    return 0;
}

/*
 * 2.5 count_ones: hitung jumlah bit 1 di x (tanpa loop!).
 *   count_ones(0) == 0
 *   count_ones(0xF0F0) == 8
 *   count_ones(0xFFFFFFFF) == 32
 * Maks op: 40
 * Petunjuk: hitung per pasangan bit, lalu per 4 bit, lalu per byte.
 */
int count_ones(uint32_t x) {
    return 0;
}

/* =====================================================================
 * LEVEL 3: memori dan endianness
 * ===================================================================== */

/*
 * 3.1 is_little_endian: kembalikan 1 jika mesin ini little endian.
 */
int is_little_endian(void) {
    return 0;
}

/*
 * 3.2 read_be32: baca 4 byte di p sebagai integer big endian.
 *   p = {0x12, 0x34, 0x56, 0x78}  ->  0x12345678
 */
uint32_t read_be32(const uint8_t *p) {
    return 0;
}

/*
 * 3.3 write_le32: tulis v ke 4 byte di p dalam urutan little endian.
 *   v = 0x12345678  ->  p = {0x78, 0x56, 0x34, 0x12}
 */
void write_le32(uint8_t *p, uint32_t v) {
}

/*
 * 3.4 reverse_bytes: balik urutan len byte di obj, in place.
 *   Berlaku untuk objek apa pun (int, double, struct, array...).
 */
void reverse_bytes(void *obj, size_t len) {
}

/* =====================================================================
 * LEVEL 4: parsing protokol biner
 * =====================================================================
 * Format header di jaringan (8 byte, semua field big endian):
 *
 *   offset 0     : version  (1 byte, harus bernilai 1)
 *   offset 1     : flags    (1 byte)
 *   offset 2..3  : length   (2 byte, big endian)
 *   offset 4..7  : seq      (4 byte, big endian)
 *
 * Contoh: 01 05 01 2C 00 00 00 2A
 *   -> version=1, flags=5, length=300, seq=42
 */

/*
 * 4.1 parse_header: isi *out dari buf.
 *   Kembalikan 0 jika sukses, -1 jika len < 8 atau version != 1.
 *   Jangan menyalin struct langsung dari buf (kenapa?).
 */
int parse_header(const uint8_t *buf, size_t len, struct header *out) {
    return -1;
}

/*
 * 4.2 build_header: tulis *h ke buf dalam format di atas.
 *   Kembalikan jumlah byte yang ditulis (8), atau 0 jika cap < 8.
 */
size_t build_header(const struct header *h, uint8_t *buf, size_t cap) {
    return 0;
}
