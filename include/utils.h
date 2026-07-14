// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
// 
// Follow Sanskar or Creator on:-
// GitHub:- "https://www.github.com/Sanskar-in"
// YouTube:- "https://youtube.com/@Sanskar-in"
// LinkedIn:- "https://www.linkedin.com/in/sanskar-in/"
// X (Formally known as Twitter):- "https://www.x.com/Sanskar_in"
// Need support:- supportramsandesh@gmail.com
// Want to make your own projects, apps, and games via Sanskar:- sanskaryadavfrom2012to2026@gmail.com
// =============================================================================
// File Purpose: Declarations for utility functions like input handling and menus.

#ifndef UTILS_H
#define UTILS_H

#define COLOR_RESET   "\x1b[0m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BOLD    "\x1b[1m"

/**
 * Displays the main menu to the user.
 */
void display_menu();

/**
 * Safely reads an integer from the user.
 * @return The read integer.
 */
int read_integer();

/**
 * Safely reads a double from the user.
 * @return The read double.
 */
double read_double();

/**
 * Clears the input buffer to prevent infinite loops on invalid input.
 */
void clear_input_buffer();

/**
 * Safely reads a string from the user.
 * @param buffer The buffer to store the string.
 * @param max_len The maximum length of the string.
 */
void read_string(char *buffer, int max_len);

/**
 * Logs a calculation to the history log file.
 * @param operation Description of the operation.
 * @param result The calculated result.
 */
void log_history(const char* operation, double result);

/**
 * Exports the final result to a text file.
 * @param result The result to export.
 */
void export_result(double result);

/**
 * Reads and displays the history log inside the terminal.
 */
void view_history();

/**
 * Prints an ASCII bar chart for the given array of numbers.
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 */
void print_ascii_bar_chart(double numbers[], int count);

/**
 * Generates a styled HTML report containing comprehensive statistics.
 * @param numbers Array of numbers to analyze.
 * @param count Number of elements in the array.
 * @param report_filename Name of the output HTML file.
 */
void generate_html_report(double numbers[], int count, const char *report_filename);

/**
 * Runs a multithreaded Matrix Digital Rain visualizer in the terminal.
 */
void start_matrix_rain();

#endif // UTILS_H
