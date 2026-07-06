<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
// =============================================================================
-->

# File & Batch Processing

For massive datasets, manual typing is impossible. Calc-Avg features robust file I/O capabilities.

## Single File Processing
Using Option 7 in the interactive menu or `--file` in the CLI, you can point Calc-Avg to any `.txt` file on your system. It will safely open the file in Read Mode (`r`), scan through every single floating-point number ignoring whitespace, aggregate the sum, and return the average.

## Multi-File Batch Processing
Introduced in Version 1.10, the engine can now merge infinite datasets!
Using Option 18 in the menu or `--batch` in the CLI, you can pass multiple file names at once.

**How it works:**
The engine dynamically creates a 2D matrix to store the file names. It systematically iterates through each file, opening it, harvesting the data, and combining the total sum and total count globally. Once all files are exhausted, it computes the singular, massive average of your entire multi-file project.
