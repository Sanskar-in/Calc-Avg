<!--
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
// =============================================================================
-->

# 🧮 Calc-Avg: The Ultimate Command-Line Calculator

Welcome to **Calc-Avg**, a highly advanced, open-source C project created by **Sanskar**.
This tool goes far beyond simple addition and division—it is a comprehensive, feature-rich statistical analysis suite built entirely for the terminal. With lightning-fast command-line argument processing, beautiful ANSI-colored interfaces, and advanced mathematical algorithms, Calc-Avg is the only average calculator you will ever need.

🔗 **GitHub Repository:** [https://www.github.com/Sanskar-in/Calc-Avg](https://www.github.com/Sanskar-in/Calc-Avg)

---

## 🌟 Comprehensive Feature List

Calc-Avg has evolved through to **Version 2.3** to include a massive suite of capabilities:

### 1. Advanced Statistical & Mathematical Engine

- **Dynamic Average**: Enter any arbitrary amount of numbers on the fly to calculate their mean.
- **Sequence Generators**: Instantly compute the average of the first _N_ Natural numbers, Odd numbers, Even numbers, Prime numbers (using an optimized primality testing algorithm), or Fibonacci sequence numbers.
- **Advanced Statistical Averages**: Go beyond the mean. Accurately calculate the **Median**, **Mode**, **Geometric Mean**, and **Harmonic Mean**.
- **Variance & Standard Deviation**: True statistical modeling! Choose whether to calculate based on a _Population_ or a _Sample_ for pinpoint accuracy.
- **Weighted Averages**: Assign specific weights to your data points for complex grading or financial calculations.
- **Filtered / Range-Based Averages**: Enter a dataset and provide a Minimum and Maximum bound. The engine will automatically filter out anomalies and calculate the average of only the numbers that fall within your specified range.

### 2. High-Performance Data & Network Processing

- **File Reader**: Calculate the average of hundreds of thousands of numbers stored in a single text file (e.g., `numbers.txt`).
- **Parallel Multi-Threaded Batch Processing**: Pass multiple files at once, and the C engine will utilize `<windows.h>` to spin up independent CPU threads to parse and analyze every single file simultaneously across your processor cores, slashing analysis time!
- **Enterprise Data Parsing**: Full integration of custom C-based parsers to extract raw numerical data directly from Excel Spreadsheets (`.csv`) and web data objects (`.json`)!
- **Live Internet API Networking**: Calculates averages using real-time data from the internet! It utilizes raw Windows Sockets (`winsock2.h`) to connect to APIs via HTTP GET requests, extracting and processing JSON payloads dynamically.

### 3. Power-User Command-Line Interface (CLI)

Bypass the interactive menu entirely for instant calculations:

- **CLI Array Processing**: Calculate data instantly directly from your shell.
- **CLI Batch Processing**: Pass the `--file` or `--batch` flags to process text files instantly.
- **HTML Report Generation (`--report`)**: Pass an array to instantly generate a beautifully styled HTML/CSS web report containing the Mean, Median, Mode, Variance, and Standard Deviation of your data!

### 4. Premium User Experience (UX)

- **Autonomous HTML Web Reports**: Automatically generates styled `.html` webpages containing full analytics tables of your dataset, bypassing the terminal entirely.
- **ASCII Data Visualizer**: When entering arrays, the program draws a mathematically scaled Green ASCII Bar Chart `(████████)` mapping out the relative magnitude of your data points right in your terminal!
- **Colorful ANSI Interface**: The interactive menu features a premium, responsive color scheme. Cyan borders, Yellow options, Red error handling, and bold Green successes make this tool a joy to use.
- **In-App History Viewer**: Every calculation is silently logged to a background `history.log` file. You can open and read this entire history directly inside the terminal without ever leaving the app.
- **Direct Result Exporting**: Save your final answers directly to a clean `results.txt` file at the push of a button.
- **GitHub Redirection**: Option 22 in the menu will instantly launch your default web browser and redirect you to the official GitHub repository!

---

## 🚀 Installation & Compilation

Calc-Avg is written in pure, standard C and has zero external dependencies, making it universally cross-platform (Windows, macOS, Linux).

### Prerequisites

- GCC (GNU Compiler Collection) or any standard C compiler.
- `make` (Optional, for simplified building).

### Build Instructions

**Using the included Makefile (Recommended for Linux/macOS):**

```bash
# Compile the project
make

# Clean up compiled binaries
make clean
```

**Manual Compilation (Windows/PowerShell):**
If you do not have `make` installed, you can compile the source code directly using GCC:

```bash
gcc -Iinclude src/main.c src/average.c src/utils.c -o calc_avg.exe -lm
```

_(Note: The `-lm` flag links the math library required for Geometric Mean and Standard Deviation calculations)._

---

## 💻 Usage Guide

### 1. Interactive Menu Mode

Simply run the executable to launch the beautifully colored, 20-option interactive menu:

```bash
./calc_avg.exe
```

### 2. Command-Line (CLI) Mode

For power users who need instant answers without a UI:

**Average a direct list of numbers:**

```bash
./calc_avg.exe 15 25 35 45 55
```

> _Output: ---> The average of the 5 numbers is: 35.0000_

**Average a single file:**

```bash
./calc_avg.exe --file data_2024.txt
```

**Average multiple files (Batch Processing):**

```bash
./calc_avg.exe --batch data_2024.txt data_2025.txt data_2026.txt
```

---

## 📂 Project Architecture

The codebase is strictly organized for modularity and open-source contribution:

```text
Calc-Avg/
│
├── include/
│   ├── average.h       # Signatures for all mathematical algorithms
│   └── utils.h         # ANSI definitions, I/O handling, and visualizers
│
├── src/
│   ├── main.c          # Core execution, CLI argument parser, Menu switch-case
│   ├── average.c       # Implementation of all math/statistical logic
│   └── utils.c         # Implementation of UI, History Logging, ASCII charts
│
├── versions/           # Archived, pre-compiled incremental releases (1.0 - 1.10)
│
├── Makefile            # Build instructions
└── README.md           # You are here!
```

## 🌍 Follow Sanskar (Creator)

This open-source project is proudly created and maintained by **Sanskar**. You can read the full creator documentation in [docs/Follow-Sanskar/follow_sanskar.md](file:///e:/Easy Projects/Avg-c_lang/docs/Follow-Sanskar/follow_sanskar.md) or use the links below:

- **GitHub**: [https://www.github.com/Sanskar-in](https://www.github.com/Sanskar-in)
- **YouTube**: [https://youtube.com/@Sanskar-in](https://youtube.com/@Sanskar-in)
- **LinkedIn**: [https://www.linkedin.com/in/sanskar-in/](https://www.linkedin.com/in/sanskar-in/)
- **X (Formally known as Twitter)**: [https://www.x.com/Sanskar_in](https://www.x.com/Sanskar_in)

**Need support?:** `supportramsandesh@gmail.com`
**Want to make your own projects, apps, and games via Sanskar?:** `sanskaryadavfrom2012to2026@gmail.com`

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](file:///e:/Easy Projects/Avg-c_lang/LICENSE) file for details. This means you are free to use, modify, and distribute this software however you like, as long as you provide attribution back to Sanskar.

---

## 🤝 Contributing & Suggestions

This project is fully open-source and created by **Sanskar**. Contributions, issues, and feature requests are welcome!

### Have an Opinion or Suggestion?

We want to hear from you! If you have ideas for new features (e.g., GUI support, JSON parsing) or general opinions about the software:

1. Navigate to the `docs/users_suggest/` directory in this repository.
2. Read the `contributing_and_ideas.md` file.
3. You can fill out the `suggestion_template.md` and submit it to us, or directly append your thoughts to the `user_opinions_and_suggestions.md` ledger via a Pull Request!

### How to Contribute Code

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📜 License & Copyright

// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// =============================================================================
