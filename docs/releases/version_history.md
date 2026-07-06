<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
// =============================================================================
-->

# Calc-Avg Official Release History

Welcome to the version history tracker for **Calc-Avg**. This document outlines the progression of the project from its foundational stages up to the ultimate v1.10 master release.

All compiled binaries for these releases can be found safely archived in the `versions/` directory at the root of the project.

---

## 🏆 Current Stable Release

### Version 1.10: The Multi-File Update (LATEST)
- **Feature Addition:** Implemented the Multi-File Batch Processing engine (`calc_avg_multi_file`).
- **Menu Update:** Added Option 18 allowing users to dynamically enter multiple files to aggregate.
- **CLI Update:** Implemented the `--batch` flag, allowing power-users to instantly process massive arrays of files (e.g. `calc_avg --batch file1.txt file2.txt`).
- **Documentation:** Major expansion of the `README.md` and the creation of a massive `docs/` wiki.

---

## 📜 Previous Releases

### Version 1.9: The Visual Update
- **Feature Addition:** Implemented the ASCII Data Visualizer (`print_ascii_bar_chart`).
- **UI Update:** Whenever a user calculates the average of an array (either via Menu Option 1 or via CLI arguments), the terminal now draws a mathematically scaled Green ASCII bar chart inside a Cyan border to visually map the data distribution before calculating the final mean.

### Version 1.8: The Quality of Life Update
- **Feature Addition:** Implemented the Native In-App History Viewer (`view_history`).
- **UI Update:** Added Option 17. Users can now read the entire `history.log` directly inside the terminal seamlessly without having to open an external text editor. Error handling gracefully warns the user if the log file hasn't been created yet.

### Version 1.7: The Power-User Update
- **Feature Addition:** Fully implemented Command-Line Argument (CLI) parsing!
- **Functionality:** Bypassed the interactive menu for instant calculations. Added support for instant array processing (`calc_avg 10 20 30`) and instant file processing (`calc_avg --file data.txt`).

### Version 1.6: The True Statistics Update
- **Feature Addition:** Added Variance (`calc_variance`) and Standard Deviation (`calc_standard_deviation`) calculations.
- **UI Update:** Added conditional logic inside the menu (Options 15 & 16) to ask the user if they are calculating based on a *Population* or a *Sample*, adjusting the mathematical division (N vs N-1) dynamically for perfect statistical accuracy. 

### Version 1.5: The Open-Source Branding Update
- **UI Update:** Implemented the official Copyright Terminal Banner. Upon launching the tool, users are instantly greeted with a golden banner establishing the project as open-source by **Sanskar**, linking directly to the GitHub repository.

### Version 1.4: The Premium Interface Update
- **Feature Addition:** Added the Range-Based / Filtered Average function (Option 14).
- **UI Update:** Completely overhauled the entire terminal interface using standard ANSI escape sequences. The menu is now beautifully rendered with Cyan headers, Yellow options, Red error messages, and bold Green success outputs. 

### Version 1.3: The Data Persistence Update
- **Feature Addition:** Integrated Advanced Statistical algorithms: Median, Mode, Geometric Mean, and Harmonic Mean. Added Weighted Averages.
- **Data Update:** Built the `log_history` function. The program now silently saves every single calculation to a background `history.log` file. Also added the ability to export a specific result to a clean `results.txt` file.

### Version 1.2: The Community Update
- **Feature Addition:** Implemented cross-platform system redirection (Windows, macOS, Linux).
- **UI Update:** Added an option in the menu to instantly launch the user's default web browser and redirect them directly to the official GitHub repository for Calc-Avg.

### Version 1.1: The Mathematical Sequence Update
- **Feature Addition:** Implemented O(1) mathematical algorithms to calculate the average of Natural, Odd, and Even numbers. 
- **Feature Addition:** Built an optimized `O(sqrt(N))` algorithm for Prime number averages, and a dynamic loop for Fibonacci Sequence averages. Implemented the basic `calc_avg_from_file` single-file reading algorithm.

### Version 1.0: The Foundation
- **Base Release:** The core skeleton of the project. Implemented the infinite `while` loop for the interactive menu and the dynamic memory allocation (`malloc`) system for calculating simple means on the fly.
