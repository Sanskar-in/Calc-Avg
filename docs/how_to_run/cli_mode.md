<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/sanskarIN/Calc-Avg"
// =============================================================================
-->

# How to Run: Command-Line (CLI) Mode

For power users, Calc-Avg can be executed instantly bypassing the UI.

## Steps to Run

1. Open your terminal.
2. Navigate to the project directory.

### Passing Arrays directly
Calculate an average instantly by typing numbers after the executable name:
```bash
# Windows
.\calc_avg.exe 15 25 35 45

# Linux/macOS
./calc_avg 15 25 35 45
```

### Passing a File
Calculate the average of numbers stored in a text file:
```bash
.\calc_avg.exe --file numbers.txt
```

### Passing Multiple Files (Batch)
Calculate the global average across multiple files at once:
```bash
.\calc_avg.exe --batch file1.txt file2.txt file3.txt
```

*Note: All CLI calculations still trigger the ASCII Bar Chart (for arrays) and silently log their results to the `history.log` file just like Interactive Mode!*
