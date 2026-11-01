#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    int i;
    for (i = 0; i < len; i++) {
        printf(" %.2x", start[i]);
    }

    printf("\n");
}

void show_int(int x) {
    printf("%d : ", x);
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
    printf("%f : ", x);
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
    printf("%d : ", x);
    show_bytes((byte_pointer) &x, sizeof(void *));
}

void show_chars(char *x) {
    printf("%s : ", x);
    show_bytes((byte_pointer) x, strlen(x) + 10);
}

int main(void) {
    int i = 10;
    show_int(i);

    float j = 10;
    show_float(j);

    int k = 12;
    show_pointer(&k);

    char *m = "mnopqr";
    show_chars(m);

    return 0;
}
