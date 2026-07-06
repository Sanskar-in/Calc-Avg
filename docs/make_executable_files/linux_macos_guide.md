<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
// =============================================================================
-->

# How to Compile Executables on Linux & macOS

Compiling Calc-Avg on Unix-based systems is extremely straightforward thanks to native C compilers and the included `Makefile`.

## 1. Prerequisites
- **Linux:** Install build tools. On Ubuntu/Debian, run: `sudo apt install build-essential`
- **macOS:** Install Apple's Command Line Tools by running: `xcode-select --install` (This includes Clang/GCC and Make).

## 2. Using the Makefile (Recommended)
The easiest way to compile on macOS and Linux is to use the provided Makefile. Simply open your terminal, navigate to the project directory, and type:
```bash
make
```
This will automatically parse the Makefile, locate the `src/` files, link the headers in `include/`, link the `-lm` math library, and output a clean executable named `calc_avg`.

To clean up and delete the compiled files, simply run:
```bash
make clean
```

## 3. Manual Compilation
If you prefer to compile manually without Make, run:
```bash
gcc -Iinclude src/main.c src/average.c src/utils.c -o calc_avg -lm
```

After compilation, you can run the executable natively:
```bash
./calc_avg
```
