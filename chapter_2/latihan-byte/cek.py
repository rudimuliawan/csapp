#!/usr/bin/env python3
"""
Pemeriksa aturan "puzzle" untuk Level 1 dan 2 (mirip 'dlc' di Data Lab).
Memeriksa operator terlarang, konstanta > 0xFF, kontrol alur, dan
menghitung jumlah operator di setiap fungsi.

Pemakaian:  python3 cek.py [file.c]     (default: latihan.c)
"""
import re
import sys

PUZZLES = {
    "get_byte": 3, "set_byte": 8, "byte_swap": 15, "swap_two_bytes": 16,
    "sign_extend_byte": 4, "rotate_left": 8, "reverse_bits_byte": 15,
    "has_zero_byte": 20, "round_up_pow2": 13, "count_ones": 40,
}

TYPES = r"(?:unsigned|signed|int|char|short|long|uint\d+_t|int\d+_t|size_t)"
FORBIDDEN_KW = ["if", "else", "for", "while", "do", "switch", "goto", "sizeof"]
# urutan penting: operator panjang dicocokkan lebih dulu
TOKEN_RE = re.compile(
    r"<<=|>>=|&&|\|\||<<|>>|\+\+|--|->|[&|^+\-*/%]=|==|!=|<=|>=|"
    r"0[xX][0-9a-fA-F]+[uUlL]*|\d+[uUlL]*|[A-Za-z_]\w*|\S"
)
ALLOWED_OPS = {"!", "~", "&", "^", "|", "+", "<<", ">>",
               "&=", "^=", "|=", "+=", "<<=", ">>="}


def strip_comments(src):
    src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
    return re.sub(r"//[^\n]*", " ", src)


def extract_body(src, name):
    m = re.search(r"\b" + name + r"\s*\([^)]*\)\s*\{", src)
    if not m:
        return None
    depth, i = 1, m.end()
    while i < len(src) and depth:
        depth += {"{": 1, "}": -1}.get(src[i], 0)
        i += 1
    return src[m.end():i - 1]


def check(name, body, limit):
    errors, ops = [], 0
    if re.search(r"\(\s*" + TYPES + r"[\s\w*]*\)", body):
        errors.append("cast tidak diizinkan")
    tokens = TOKEN_RE.findall(body)
    for i, t in enumerate(tokens):
        nxt = tokens[i + 1] if i + 1 < len(tokens) else ""
        if t in FORBIDDEN_KW:
            errors.append(f"'{t}' tidak diizinkan")
        elif re.match(r"[A-Za-z_]\w*$", t) and nxt == "(" and t != "return":
            errors.append(f"pemanggilan fungsi '{t}' tidak diizinkan")
        elif re.match(r"\d|0[xX]", t):
            value = int(re.sub(r"[uUlL]+$", "", t), 0)
            if value > 0xFF:
                errors.append(f"konstanta {t} > 0xFF")
        elif t in ALLOWED_OPS:
            ops += 1
        elif t in ("?", "*", "/", "%", "-", "&&", "||", "==", "!=", "<", ">",
                   "<=", ">=", "++", "--", "[", "->", "-=", "*=", "/=", "%="):
            if t == "*" and i > 0 and re.match(TYPES + "$", tokens[i - 1]):
                errors.append("pointer tidak diizinkan")
            else:
                errors.append(f"operator '{t}' tidak diizinkan")
    if ops == 0 and not errors:
        print(f"  -- {name:<18}  belum diisi")
        return False
    status = "OK " if not errors and ops <= limit else "!! "
    note = "" if ops <= limit else f"  (melebihi batas {limit})"
    print(f"  {status}{name:<18} {ops:>2} op{note}")
    for e in sorted(set(errors)):
        print(f"       - {e}")
    return not errors and ops <= limit


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "latihan.c"
    src = strip_comments(open(path, encoding="utf-8").read())
    print(f"Memeriksa aturan puzzle di {path}")
    good = 0
    for name, limit in PUZZLES.items():
        body = extract_body(src, name)
        if body is None:
            print(f"  ?? {name}: fungsi tidak ditemukan")
        elif check(name, body, limit):
            good += 1
    print(f"\n{good} / {len(PUZZLES)} fungsi memenuhi aturan.")
    print("Catatan: pemeriksa ini sederhana; jawaban tetap harus lolos 'make'.")


if __name__ == "__main__":
    main()
