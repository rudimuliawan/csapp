#ifndef LATIHAN_H
#define LATIHAN_H

#include <stdint.h>
#include <stddef.h>

/* ===== Level 1: dasar byte (aturan "puzzle") ===== */
uint32_t get_byte(uint32_t x, int n);
uint32_t set_byte(uint32_t x, int n, uint32_t b);
uint32_t byte_swap(uint32_t x);
uint32_t swap_two_bytes(uint32_t x, int n, int m);
int32_t  sign_extend_byte(uint32_t x);

/* ===== Level 2: trik bit (aturan "puzzle") ===== */
uint32_t rotate_left(uint32_t x, int n);
uint32_t reverse_bits_byte(uint32_t b);
int      has_zero_byte(uint32_t x);
uint32_t round_up_pow2(uint32_t x);
int      count_ones(uint32_t x);

/* ===== Level 3: memori dan endianness (C bebas) ===== */
int      is_little_endian(void);
uint32_t read_be32(const uint8_t *p);
void     write_le32(uint8_t *p, uint32_t v);
void     reverse_bytes(void *obj, size_t len);

/* ===== Level 4: parsing protokol biner (C bebas) ===== */
struct header {
    uint8_t  version;
    uint8_t  flags;
    uint16_t length;
    uint32_t seq;
};

int    parse_header(const uint8_t *buf, size_t len, struct header *out);
size_t build_header(const struct header *h, uint8_t *buf, size_t cap);

#endif
