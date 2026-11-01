/*
 * Test otomatis. Tidak perlu diubah.
 * Jawabanmu dibandingkan dengan implementasi referensi yang sengaja ditulis
 * secara "naif" (loop, array), memakai nilai tepi dan 200.000 input acak.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "latihan.h"

#define N_RANDOM 200000

static int total_pass = 0, total_fail = 0;

/* ---------- RNG sederhana (xorshift32), deterministik ---------- */
static uint32_t rng_state = 0x9E3779B9u;
static uint32_t rnd(void) {
    uint32_t x = rng_state;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    return rng_state = x;
}

static const uint32_t EDGE[] = {
    0x00000000, 0x00000001, 0x0000007F, 0x00000080, 0x000000FF,
    0x00000100, 0x0000FFFF, 0x00FF00FF, 0xFF00FF00, 0x12345678,
    0x87654321, 0xDEADBEEF, 0x7FFFFFFF, 0x80000000, 0x80000001,
    0xFFFFFFFE, 0xFFFFFFFF, 0x01010101, 0x80808080, 0x7F7F7F7F,
};
#define N_EDGE (sizeof EDGE / sizeof EDGE[0])

static void report(const char *name, int ok, const char *detail) {
    if (ok) {
        printf("  [PASS] %s\n", name);
        total_pass++;
    } else {
        printf("  [FAIL] %s\n         %s\n", name, detail);
        total_fail++;
    }
}

/* ---------- Referensi naif ---------- */
static void to_bytes(uint32_t x, uint8_t b[4]) {
    for (int i = 0; i < 4; i++) b[i] = (uint8_t)((x / (1u << (8 * i))) % 256);
}
static uint32_t from_bytes(const uint8_t b[4]) {
    uint32_t x = 0;
    for (int i = 3; i >= 0; i--) x = x * 256 + b[i];
    return x;
}
static uint32_t ref_get_byte(uint32_t x, int n) { uint8_t b[4]; to_bytes(x, b); return b[n]; }
static uint32_t ref_set_byte(uint32_t x, int n, uint32_t v) {
    uint8_t b[4]; to_bytes(x, b); b[n] = (uint8_t)(v % 256); return from_bytes(b);
}
static uint32_t ref_byte_swap(uint32_t x) {
    uint8_t b[4], r[4]; to_bytes(x, b);
    for (int i = 0; i < 4; i++) r[i] = b[3 - i];
    return from_bytes(r);
}
static uint32_t ref_swap_two(uint32_t x, int n, int m) {
    uint8_t b[4]; to_bytes(x, b);
    uint8_t t = b[n]; b[n] = b[m]; b[m] = t;
    return from_bytes(b);
}
static int32_t ref_sign_extend(uint32_t x) {
    int v = (int)(x % 256);
    return v >= 128 ? v - 256 : v;
}
static uint32_t ref_rotate(uint32_t x, int n) {
    for (int i = 0; i < n; i++) x = (x * 2) + (x >= 0x80000000u ? 1 : 0);
    return x;
}
static uint32_t ref_reverse_bits(uint32_t b) {
    uint32_t r = 0;
    for (int i = 0; i < 8; i++) if (b & (1u << i)) r |= 1u << (7 - i);
    return r;
}
static int ref_has_zero(uint32_t x) {
    uint8_t b[4]; to_bytes(x, b);
    for (int i = 0; i < 4; i++) if (b[i] == 0) return 1;
    return 0;
}
static uint32_t ref_round_up(uint32_t x) {
    uint32_t p = 1;
    while (p < x) p *= 2;
    return p;
}
static int ref_count(uint32_t x) {
    int c = 0;
    for (int i = 0; i < 32; i++) if (x & (1u << i)) c++;
    return c;
}

/* ---------- Test Level 1 & 2 ---------- */
static void test_get_byte(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        for (int n = 0; n < 4 && ok; n++) {
            uint32_t got = get_byte(x, n), exp = ref_get_byte(x, n);
            if (got != exp) { ok = 0; snprintf(d, sizeof d, "get_byte(0x%08x, %d) = 0x%x, seharusnya 0x%x", x, n, got, exp); }
        }
    }
    report("1.1 get_byte", ok, d);
}

static void test_set_byte(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        uint32_t v = i < N_EDGE ? EDGE[N_EDGE - 1 - i] : rnd();
        for (int n = 0; n < 4 && ok; n++) {
            uint32_t got = set_byte(x, n, v), exp = ref_set_byte(x, n, v);
            if (got != exp) { ok = 0; snprintf(d, sizeof d, "set_byte(0x%08x, %d, 0x%x) = 0x%08x, seharusnya 0x%08x", x, n, v, got, exp); }
        }
    }
    report("1.2 set_byte", ok, d);
}

static void test_byte_swap(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        uint32_t got = byte_swap(x), exp = ref_byte_swap(x);
        if (got != exp) { ok = 0; snprintf(d, sizeof d, "byte_swap(0x%08x) = 0x%08x, seharusnya 0x%08x", x, got, exp); }
    }
    report("1.3 byte_swap", ok, d);
}

static void test_swap_two(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM / 4 && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        for (int n = 0; n < 4 && ok; n++)
            for (int m = 0; m < 4 && ok; m++) {
                uint32_t got = swap_two_bytes(x, n, m), exp = ref_swap_two(x, n, m);
                if (got != exp) { ok = 0; snprintf(d, sizeof d, "swap_two_bytes(0x%08x, %d, %d) = 0x%08x, seharusnya 0x%08x", x, n, m, got, exp); }
            }
    }
    report("1.4 swap_two_bytes", ok, d);
}

static void test_sign_extend(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        int32_t got = sign_extend_byte(x), exp = ref_sign_extend(x);
        if (got != exp) { ok = 0; snprintf(d, sizeof d, "sign_extend_byte(0x%08x) = %d, seharusnya %d", x, got, exp); }
    }
    report("1.5 sign_extend_byte", ok, d);
}

static void test_rotate(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM / 8 && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        for (int n = 0; n < 32 && ok; n++) {
            uint32_t got = rotate_left(x, n), exp = ref_rotate(x, n);
            if (got != exp) { ok = 0; snprintf(d, sizeof d, "rotate_left(0x%08x, %d) = 0x%08x, seharusnya 0x%08x", x, n, got, exp); }
        }
    }
    report("2.1 rotate_left", ok, d);
}

static void test_reverse_bits(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t b = 0; b < 256 && ok; b++) {
        uint32_t got = reverse_bits_byte(b), exp = ref_reverse_bits(b);
        if (got != exp) { ok = 0; snprintf(d, sizeof d, "reverse_bits_byte(0x%02x) = 0x%x, seharusnya 0x%02x", b, got, exp); }
    }
    report("2.2 reverse_bits_byte", ok, d);
}

static void test_has_zero(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        /* sebagian input sengaja diberi byte nol agar kasus "1" sering muncul */
        if (i >= N_EDGE && (i & 1)) x &= ~(0xFFu << (8 * (rnd() % 4)));
        int got = has_zero_byte(x), exp = ref_has_zero(x);
        if (got != exp) { ok = 0; snprintf(d, sizeof d, "has_zero_byte(0x%08x) = %d, seharusnya %d", x, got, exp); }
    }
    report("2.3 has_zero_byte", ok, d);
}

static void test_round_up(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_RANDOM && ok; i++) {
        uint32_t x;
        if (i < 64) x = i + 1;                             /* 1..64          */
        else if (i < 96) x = 1u << (i - 64);               /* semua 2^k      */
        else if (i < 127) x = (1u << (i - 96)) + 1;        /* 2^k + 1        */
        else x = (rnd() % 0x80000000u) + 1;                /* acak 1..2^31   */
        uint32_t got = round_up_pow2(x), exp = ref_round_up(x);
        if (got != exp) { ok = 0; snprintf(d, sizeof d, "round_up_pow2(%u) = %u, seharusnya %u", x, got, exp); }
    }
    report("2.4 round_up_pow2", ok, d);
}

static void test_count(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        int got = count_ones(x), exp = ref_count(x);
        if (got != exp) { ok = 0; snprintf(d, sizeof d, "count_ones(0x%08x) = %d, seharusnya %d", x, got, exp); }
    }
    report("2.5 count_ones", ok, d);
}

/* ---------- Test Level 3 ---------- */
static void test_endian(void) {
    char d[160] = "";
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    int exp = 1;
#else
    int exp = 0;
#endif
    int got = is_little_endian();
    snprintf(d, sizeof d, "is_little_endian() = %d, seharusnya %d", got, exp);
    report("3.1 is_little_endian", got == exp, d);
}

static void test_read_be32(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        uint8_t le[4], be[4]; to_bytes(x, le);
        for (int k = 0; k < 4; k++) be[k] = le[3 - k];
        uint32_t got = read_be32(be);
        if (got != x) { ok = 0; snprintf(d, sizeof d, "read_be32({%02x %02x %02x %02x}) = 0x%08x, seharusnya 0x%08x", be[0], be[1], be[2], be[3], got, x); }
    }
    report("3.2 read_be32", ok, d);
}

static void test_write_le32(void) {
    char d[160] = ""; int ok = 1;
    for (uint32_t i = 0; i < N_EDGE + N_RANDOM && ok; i++) {
        uint32_t x = i < N_EDGE ? EDGE[i] : rnd();
        uint8_t exp[4], got[6] = {0xAA, 0, 0, 0, 0, 0xAA};
        to_bytes(x, exp);
        write_le32(got + 1, x);
        if (memcmp(got + 1, exp, 4) != 0 || got[0] != 0xAA || got[5] != 0xAA) {
            ok = 0;
            snprintf(d, sizeof d, "write_le32(0x%08x) menghasilkan {%02x %02x %02x %02x}, seharusnya {%02x %02x %02x %02x} (atau menulis di luar 4 byte)",
                     x, got[1], got[2], got[3], got[4], exp[0], exp[1], exp[2], exp[3]);
        }
    }
    report("3.3 write_le32", ok, d);
}

static void test_reverse_bytes(void) {
    char d[160] = ""; int ok = 1;
    for (size_t len = 0; len <= 33 && ok; len++) {
        uint8_t buf[40], exp[40];
        for (size_t k = 0; k < sizeof buf; k++) buf[k] = (uint8_t)rnd();
        memcpy(exp, buf, sizeof buf);
        for (size_t k = 0; k < len; k++) exp[k + 2] = buf[2 + len - 1 - k];
        reverse_bytes(buf + 2, len);
        if (memcmp(buf, exp, sizeof buf) != 0) { ok = 0; snprintf(d, sizeof d, "reverse_bytes salah untuk len = %zu (atau menulis di luar batas)", len); }
    }
    if (ok) {
        double v = 1.5, w; unsigned char *pv = (unsigned char *)&v, *pw = (unsigned char *)&w;
        for (int k = 0; k < 8; k++) pw[k] = pv[7 - k];
        reverse_bytes(&v, sizeof v);
        if (memcmp(&v, &w, sizeof v) != 0) { ok = 0; snprintf(d, sizeof d, "reverse_bytes salah untuk sebuah double"); }
    }
    report("3.4 reverse_bytes", ok, d);
}

/* ---------- Test Level 4 ---------- */
static void test_parse(void) {
    char d[200] = ""; int ok = 1;
    struct header h;
    const uint8_t v1[] = {0x01, 0x05, 0x01, 0x2C, 0x00, 0x00, 0x00, 0x2A};
    const uint8_t v2[] = {0x01, 0xFF, 0xFF, 0xFE, 0xDE, 0xAD, 0xBE, 0xEF, 0x99};
    const uint8_t bad_ver[] = {0x02, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01};

    memset(&h, 0, sizeof h);
    if (parse_header(v1, sizeof v1, &h) != 0 || h.version != 1 || h.flags != 5 || h.length != 300 || h.seq != 42) {
        ok = 0; snprintf(d, sizeof d, "01 05 01 2C 00 00 00 2A -> ret/version/flags/length/seq = ?/%u/%u/%u/%u, seharusnya 0/1/5/300/42",
                         h.version, h.flags, h.length, h.seq);
    }
    memset(&h, 0, sizeof h);
    if (ok && (parse_header(v2, sizeof v2, &h) != 0 || h.flags != 0xFF || h.length != 0xFFFE || h.seq != 0xDEADBEEF)) {
        ok = 0; snprintf(d, sizeof d, "01 FF FF FE DE AD BE EF -> flags/length/seq = 0x%x/0x%x/0x%x, seharusnya 0xff/0xfffe/0xdeadbeef",
                         h.flags, h.length, h.seq);
    }
    if (ok && parse_header(v1, 7, &h) != -1) { ok = 0; snprintf(d, sizeof d, "len = 7 seharusnya mengembalikan -1"); }
    if (ok && parse_header(bad_ver, sizeof bad_ver, &h) != -1) { ok = 0; snprintf(d, sizeof d, "version = 2 seharusnya mengembalikan -1"); }
    report("4.1 parse_header", ok, d);
}

static void test_build(void) {
    char d[200] = ""; int ok = 1;
    struct header h = {1, 5, 300, 42}, back;
    const uint8_t exp[8] = {0x01, 0x05, 0x01, 0x2C, 0x00, 0x00, 0x00, 0x2A};
    uint8_t buf[10];

    memset(buf, 0xAA, sizeof buf);
    size_t n = build_header(&h, buf + 1, 8);
    if (n != 8 || memcmp(buf + 1, exp, 8) != 0 || buf[0] != 0xAA || buf[9] != 0xAA) {
        ok = 0; snprintf(d, sizeof d, "{1,5,300,42} -> %02x %02x %02x %02x %02x %02x %02x %02x (ret %zu), seharusnya 01 05 01 2C 00 00 00 2A (ret 8)",
                         buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], n);
    }
    if (ok && build_header(&h, buf, 7) != 0) { ok = 0; snprintf(d, sizeof d, "cap = 7 seharusnya mengembalikan 0"); }
    for (int i = 0; i < 10000 && ok; i++) {
        uint32_t r = rnd();
        struct header src = {1, (uint8_t)r, (uint16_t)(r >> 8), rnd()};
        memset(&back, 0, sizeof back);
        if (build_header(&src, buf, sizeof buf) != 8 || parse_header(buf, 8, &back) != 0 ||
            back.flags != src.flags || back.length != src.length || back.seq != src.seq) {
            ok = 0; snprintf(d, sizeof d, "round trip build -> parse gagal untuk flags=0x%x length=0x%x seq=0x%x", src.flags, src.length, src.seq);
        }
    }
    report("4.2 build_header (+ round trip dengan parse)", ok, d);
}

int main(void) {
    printf("Level 1: dasar byte\n");
    test_get_byte(); test_set_byte(); test_byte_swap(); test_swap_two(); test_sign_extend();
    printf("Level 2: trik bit\n");
    test_rotate(); test_reverse_bits(); test_has_zero(); test_round_up(); test_count();
    printf("Level 3: memori dan endianness\n");
    test_endian(); test_read_be32(); test_write_le32(); test_reverse_bytes();
    printf("Level 4: parsing protokol biner\n");
    test_parse(); test_build();

    printf("\nSkor: %d / %d\n", total_pass, total_pass + total_fail);
    if (total_fail == 0) printf("Semua lolos! Jalankan 'make cek' untuk memeriksa aturan dan jumlah operator.\n");
    return total_fail != 0;
}
