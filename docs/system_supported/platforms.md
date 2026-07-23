<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/sanskarIN/Calc-Avg"
// =============================================================================
-->

# Supported Systems

Calc-Avg is written in **pure standard C** without relying on external GUI libraries, making it 100% cross-platform.

## Officially Supported Operating Systems

1. **Windows (7, 8, 10, 11)**
   - Pre-compiled binaries available in the `versions/` folder (e.g. `calc_avg.exe`).
   - GitHub redirection utilizes the native Windows `system("start ...")` command.
   
2. **macOS**
   - Easily compiled via GCC or Clang using the provided Makefile.
   - GitHub redirection utilizes the native macOS `system("open ...")` command.

3. **Linux (Ubuntu, Debian, Arch, Fedora, etc.)**
   - Easily compiled via GCC.
   - GitHub redirection utilizes the native Linux `system("xdg-open ...")` command.

Because standard ANSI escape codes are used for colors, almost all modern terminal emulators across these three platforms will natively render the premium UI perfectly!
