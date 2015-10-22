tests = (
    " \t\r\n\f",
    "0123456789",
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-*/^%<>=~_|&$#@!,.?:;[]{}\\`"
)

chars = []

for i in range(0, 256):
    char = chr(i)
    flag = 0
    for j, test in enumerate(tests):
        if char in test:
            flag |= 1 << j
    chars.append(str(flag))

def transform((i, f)):
    if i % 16 == 0:
        return "\n  " + f
    return f

print("static char char_flags[256] = {" + ", ".join(map(transform, enumerate(chars))) + "\n};")
