<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
// =============================================================================
-->

# How to Compile Executables on Windows

If you are developing or modifying the Calc-Avg project on a Windows machine, follow these steps to build your own `.exe` files.

## 1. Install GCC
You need a C compiler. For Windows, **MinGW-w64** is highly recommended.
1. Download and install MinGW-w64.
2. Add the `bin/` directory of your MinGW installation to your Windows System `PATH`.
3. Verify installation by opening PowerShell or Command Prompt and typing: `gcc --version`.

## 2. Compiling the Project
Open PowerShell or Command Prompt, navigate to the root directory of the Calc-Avg project, and run this exact command:
```powershell
gcc -Iinclude src/main.c src/average.c src/utils.c -o calc_avg.exe -lm
```

**Explanation:**
- `-Iinclude`: Tells GCC to look in the `include/` folder for header files.
- `src/main.c src/average.c src/utils.c`: The three core source code files that must be linked.
- `-o calc_avg.exe`: Names the final compiled output executable.
- `-lm`: Links the mathematical library (required for `sqrt` and `pow` functions used in standard deviation).

## 3. Organizing Versions
Once compiled, you can move your new `calc_avg.exe` into a specific version folder (e.g. `versions/2.0/`) to keep your builds organized.
