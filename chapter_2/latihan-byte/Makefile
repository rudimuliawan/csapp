CC      ?= gcc
CFLAGS  = -std=c11 -O2 -Wall -Wextra -Wno-unused-parameter -fsanitize=undefined

.PHONY: test cek solusi prediksi clean

# Uji jawabanmu di latihan.c
test: latihan.h latihan.c test.c
	$(CC) $(CFLAGS) -o run_test test.c latihan.c
	./run_test

# Periksa aturan puzzle dan jumlah operator (Level 1 dan 2)
cek:
	python3 cek.py latihan.c

# Uji solusi referensi
solusi: latihan.h solusi.c test.c
	$(CC) $(CFLAGS) -o run_solusi test.c solusi.c
	./run_solusi
	python3 cek.py solusi.c

# Soal prediksi output
prediksi: prediksi.c
	$(CC) -std=c11 -O0 -o run_prediksi prediksi.c
	./run_prediksi

clean:
	rm -f run_test run_solusi run_prediksi
