# 📌 Build Steps in C

When you compile and run a C program, several steps happen **before you get an executable**.

## 1. Preprocessing

- The preprocessor ``(cpp)`` handles directives starting with ``#`` (like ``#include``, ``#define``, ``#ifdef``).

- Expands macros, includes header files, and removes comments.

- Output: a “pure” C code with no preprocessor directives.

Example
```c
#include <stdio.h>
#define PI 3.14

int main() {
    printf("%f\n", PI);
    return 0;
}
```
**After preprocessing →**
```c
int main() {
    printf("%f\n", 3.14);
    return 0;
}
```
## 2. Compilation

- The compiler converts the preprocessed code into assembly code (machine-readable instructions in human-friendly form).

- Checks for **syntax errors**.

- Output: ``.s``file (assembly).

## 3. Assembly

- The assembler translates the assembly code into **machine code (object file).**

- Output: ``.o`` (object file, contains binary code).

## 4. Linking

- The linker combines your object file(s) with libraries (``printf``, ``scanf``, etc. from libc).

- Resolves function calls and addresses.

- Output: final **executable file** (e.g., a.out in Linux, .exe in Windows).

## ✅ Summary of Steps

| Step          | Tool         | Input              | Output        |
| ------------- | ------------ | ------------------ | ------------- |
| Preprocessing | Preprocessor | Source code `.c`   | Expanded code |
| Compilation   | Compiler     | Preprocessed code  | Assembly `.s` |
| Assembly      | Assembler    | Assembly `.s`      | Object `.o`   |
| Linking       | Linker       | Object + libraries | Executable    |

## 🔎 Example using GCC

If your program is ``main.c``:
```cpp
gcc -E main.c -o main.i   # Preprocessing
gcc -S main.i -o main.s   # Compilation
gcc -c main.s -o main.o   # Assembly
gcc main.o -o main        # Linking -> Executable
```

## ⚡ Key takeaway:

**Preprocessing → Compilation → Assembly → Linking → Executable.**
