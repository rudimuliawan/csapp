# Latihan Byte Manipulation dengan C

Pendamping CSAPP Bab 2. Butuh Linux, `gcc`, `make`, dan `python3`.

## Isi

| File | Fungsi |
| --- | --- |
| `latihan.c` | **Soal.** Isi setiap fungsi di sini. |
| `prediksi.c` | 14 soal tebak output. Tebak dulu, baru jalankan. |
| `test.c` | Test otomatis (tidak perlu diubah). |
| `cek.py` | Pemeriksa aturan puzzle dan jumlah operator. |
| `solusi.c` | Solusi dan pembahasan. **Jangan dibuka dulu.** |

## Level

1. **Dasar byte** – get/set byte, byte swap, tukar dua byte, sign extension
2. **Trik bit** – rotate, reverse bit, deteksi byte nol, pangkat 2, popcount
3. **Memori dan endianness** – cek endian, baca big endian, tulis little endian, balik byte
4. **Parsing protokol biner** – parse dan build header jaringan 8 byte

Level 1–2 memakai aturan puzzle ala Data Lab (hanya `! ~ & ^ | + << >>`, tanpa if/loop, konstanta ≤ 0xFF). Level 3–4 bebas.

## Perintah

```sh
make prediksi   # jalankan soal tebak output
make            # uji jawabanmu di latihan.c
make cek        # periksa aturan puzzle dan jumlah operator
make solusi     # uji solusi referensi
```

Saran urutan: `make prediksi` dulu sebagai pemanasan, lalu kerjakan `latihan.c` level demi level sambil menjalankan `make`.
