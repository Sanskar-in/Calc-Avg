<!-- 
// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/sanskarIN/Calc-Avg"
// =============================================================================
-->

# Advanced Statistical Engine

Calc-Avg isn't just about finding the basic "mean". It is a full-fledged statistical modeling tool.

## Supported Calculations

### 1. Median
To find the exact middle value of a dataset, Calc-Avg safely clones your array and implements a **QuickSort** (`qsort`) algorithm to perfectly align the numbers from lowest to highest. It accurately handles both odd-count and even-count arrays.

### 2. Mode
After sorting the dataset, the engine accurately scans for the highest frequency of consecutive duplicates, instantly returning the most commonly occurring number in your dataset.

### 3. Geometric Mean
The Geometric Mean is crucial for determining growth rates. The engine uses strict validation to ensure all numbers are purely positive, computes their mathematical product, and leverages `#include <math.h>` to compute the precise root.

### 4. Harmonic Mean
Crucial for computing average rates (like speed). Calc-Avg safely validates against divide-by-zero errors to compute the sum of reciprocals accurately.

### 5. Variance & Standard Deviation
True statistical variance! When you select these options, the engine specifically asks you if your data is a **Population** or a **Sample**. It adjusts the division (by `N` or `N-1`) accordingly to ensure mathematically perfect statistical analysis of data spread.
