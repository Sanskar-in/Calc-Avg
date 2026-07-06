<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
// =============================================================================
-->

# Dynamic Arrays & Sequences

The **Calc-Avg** engine is built to dynamically handle arrays of any size, meaning you are never limited to a fixed set of numbers.

## Features

### 1. Arbitrary User Inputs
When using the interactive menu (Option 1), the system dynamically allocates memory (`malloc`) to store exactly as many numbers as you wish to process. This ensures zero memory waste and prevents buffer overflows.

### 2. Built-in Mathematical Sequences
If you don't want to type out arrays manually, Calc-Avg features mathematical sequence generators that compute the average of the first *N* numbers in a series instantly:
- **Natural Numbers:** Uses the `n * (n + 1) / 2` mathematical proof for instant O(1) time complexity calculation.
- **Odd/Even Numbers:** Automatically deduces the mathematical sum of odds and evens to instantly return the average without looping.
- **Prime Numbers:** Implements a highly optimized `O(sqrt(n))` primality testing algorithm to calculate the average of prime numbers without lagging your system.
- **Fibonacci Sequence:** Dynamically generates the Fibonacci sequence up to your specified *N* and aggregates the sum securely using `long long` integer limits to prevent early overflow.
