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
// File Purpose: Implementation of utility functions for input/output and menus.

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "average.h"
#include "utils.h"

void display_menu() {
    printf("\n");
    printf(COLOR_CYAN COLOR_BOLD "=========================================\n" COLOR_RESET);
    printf(COLOR_CYAN COLOR_BOLD "              Calc-Avg Menu              \n" COLOR_RESET);
    printf(COLOR_CYAN COLOR_BOLD "=========================================\n" COLOR_RESET);
    printf(COLOR_YELLOW "1. Average of dynamically entered numbers\n" COLOR_RESET);
    printf(COLOR_YELLOW "2. Average of first N natural numbers\n" COLOR_RESET);
    printf(COLOR_YELLOW "3. Average of first N odd numbers\n" COLOR_RESET);
    printf(COLOR_YELLOW "4. Average of first N even numbers\n" COLOR_RESET);
    printf(COLOR_YELLOW "5. Average of first N prime numbers\n" COLOR_RESET);
    printf(COLOR_YELLOW "6. Average of first N Fibonacci numbers\n" COLOR_RESET);
    printf(COLOR_YELLOW "7. Average of numbers from a text file\n" COLOR_RESET);
    printf(COLOR_YELLOW "8. Median\n" COLOR_RESET);
    printf(COLOR_YELLOW "9. Mode\n" COLOR_RESET);
    printf(COLOR_YELLOW "10. Geometric Mean\n" COLOR_RESET);
    printf(COLOR_YELLOW "11. Harmonic Mean\n" COLOR_RESET);
    printf(COLOR_YELLOW "12. Weighted Average\n" COLOR_RESET);
    printf(COLOR_YELLOW "13. Export last result\n" COLOR_RESET);
    printf(COLOR_YELLOW "14. Filtered/Range-based Average\n" COLOR_RESET);
    printf(COLOR_YELLOW "15. Variance\n" COLOR_RESET);
    printf(COLOR_YELLOW "16. Standard Deviation\n" COLOR_RESET);
    printf(COLOR_YELLOW "17. View Calculation History\n" COLOR_RESET);
    printf(COLOR_YELLOW "18. Batch File Average\n" COLOR_RESET);
    printf(COLOR_YELLOW "19. Average from CSV Spreadsheet\n" COLOR_RESET);
    printf(COLOR_YELLOW "20. Average from JSON Data\n" COLOR_RESET);
    printf(COLOR_YELLOW "21. Generate HTML Report from Array\n" COLOR_RESET);
    printf(COLOR_YELLOW "22. Average from Live Internet API\n" COLOR_RESET);
    printf(COLOR_YELLOW "23. Follow Sanskar / Creator Support\n" COLOR_RESET);
    printf(COLOR_YELLOW "24. Average from SQLite Database\n" COLOR_RESET);
    printf(COLOR_YELLOW "25. Average of a 2D Matrix\n" COLOR_RESET);
    printf(COLOR_YELLOW "26. Financial Market Analysis (SMA & EMA)\n" COLOR_RESET);
    printf(COLOR_YELLOW "27. Engineering Calculus (Derivatives & Integrals)\n" COLOR_RESET);
    printf(COLOR_YELLOW "28. Launch Graphical User Interface (GUI)\n" COLOR_RESET);
    printf(COLOR_YELLOW "29. Machine Learning (Linear Regression Predictions)\n" COLOR_RESET);
    printf(COLOR_YELLOW "30. Cybersecurity Cryptography (SHA-256 Hashing)\n" COLOR_RESET);
    printf(COLOR_YELLOW "31. Big Data Compression (Run-Length Encoding)\n" COLOR_RESET);
    printf(COLOR_YELLOW "32. See Source Code on GitHub\n" COLOR_RESET);
    printf(COLOR_RED "33. Exit\n" COLOR_RESET);
    printf(COLOR_CYAN COLOR_BOLD "=========================================\n" COLOR_RESET);
    printf(COLOR_GREEN "Enter your choice: " COLOR_RESET);
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int read_integer() {
    int value;
    while (scanf("%d", &value) != 1) {
        printf("Invalid input. Please enter an integer: ");
        clear_input_buffer();
    }
    clear_input_buffer(); // clear the trailing newline
    return value;
}

double read_double() {
    double value;
    while (scanf("%lf", &value) != 1) {
        printf("Invalid input. Please enter a number: ");
        clear_input_buffer();
    }
    clear_input_buffer(); // clear the trailing newline
    return value;
}

void read_string(char *buffer, int max_len) {
    if (fgets(buffer, max_len, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            clear_input_buffer();
        }
    }
}

void log_history(const char* operation, double result) {
    FILE *f = fopen("history.log", "a");
    if (f) {
        fprintf(f, "%s: %.4f\n", operation, result);
        fclose(f);
    }
}

void export_result(double result) {
    FILE *f = fopen("results.txt", "w");
    if (f) {
        fprintf(f, "Last Calculated Result: %.4f\n", result);
        fclose(f);
        printf("\nResult successfully exported to results.txt\n");
    } else {
        printf("\nError exporting result.\n");
    }
}

void view_history() {
    FILE *file = fopen("history.log", "r");
    if (file == NULL) {
        printf(COLOR_RED "\nNo history log found. Please perform a calculation first.\n" COLOR_RESET);
        return;
    }

    printf(COLOR_CYAN COLOR_BOLD "\n=== Calculation History ===\n" COLOR_RESET);
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    printf(COLOR_CYAN COLOR_BOLD "===========================\n" COLOR_RESET);
    fclose(file);
}

void print_ascii_bar_chart(double numbers[], int count) {
    if (count <= 0) return;
    double max_val = numbers[0];
    for(int i = 1; i < count; i++) {
        if(numbers[i] > max_val) max_val = numbers[i];
    }
    if (max_val <= 0) max_val = 1.0; 

    printf(COLOR_CYAN COLOR_BOLD "\n=== Data Visualizer (Bar Chart) ===\n" COLOR_RESET);
    for(int i = 0; i < count; i++) {
        printf("Val %3d | ", i + 1);
        int bar_len = (int)((numbers[i] / max_val) * 40.0);
        if (bar_len < 0) bar_len = 0;
        
        printf(COLOR_GREEN);
        for(int j = 0; j < bar_len; j++) {
            printf("#");
        }
        printf(COLOR_RESET);
        printf(" %.2f\n", numbers[i]);
    }
    printf(COLOR_CYAN COLOR_BOLD "===================================\n" COLOR_RESET);
}

void generate_html_report(double numbers[], int count, const char *report_filename) {
    if (count <= 0) {
        printf(COLOR_RED "Error: No numbers to generate report for.\n" COLOR_RESET);
        return;
    }
    
    FILE *file = fopen(report_filename, "w");
    if (!file) {
        printf(COLOR_RED "Error: Could not create HTML report file.\n" COLOR_RESET);
        return;
    }
    
    // Calculate Statistics
    double mean = calc_avg_array(numbers, count);
    double median = calc_median(numbers, count);
    double mode = calc_mode(numbers, count);
    double variance = calc_variance(numbers, count, false); // Assuming sample
    double stddev = calc_standard_deviation(numbers, count, false); // Assuming sample
    
    // Get current time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    fprintf(file, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n");
    fprintf(file, "<meta charset=\"UTF-8\">\n");
    fprintf(file, "<title>Calc-Avg Statistical Report</title>\n");
    fprintf(file, "<style>\n");
    fprintf(file, "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #f4f7f6; color: #333; margin: 40px; }\n");
    fprintf(file, "h1 { color: #2c3e50; text-align: center; border-bottom: 2px solid #3498db; padding-bottom: 10px; }\n");
    fprintf(file, ".container { background: #fff; padding: 30px; border-radius: 8px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); max-width: 800px; margin: auto; }\n");
    fprintf(file, "table { width: 100%%; border-collapse: collapse; margin-top: 20px; }\n");
    fprintf(file, "th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    fprintf(file, "th { background-color: #3498db; color: white; }\n");
    fprintf(file, "tr:hover { background-color: #f1f1f1; }\n");
    fprintf(file, ".footer { text-align: center; margin-top: 30px; font-size: 0.9em; color: #7f8c8d; }\n");
    fprintf(file, "</style>\n</head>\n<body>\n");
    
    fprintf(file, "<div class=\"container\">\n");
    fprintf(file, "<h1>Data Science Statistical Report</h1>\n");
    fprintf(file, "<p>Generated by <strong>Calc-Avg v2.1</strong> on %d-%02d-%02d %02d:%02d:%02d</p>\n", 
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    fprintf(file, "<h2>Analysis Summary</h2>\n");
    fprintf(file, "<p>Data point count: %d</p>\n", count);
    
    fprintf(file, "<table>\n");
    fprintf(file, "<tr><th>Statistic</th><th>Value</th></tr>\n");
    fprintf(file, "<tr><td><strong>Mean (Average)</strong></td><td>%.4f</td></tr>\n", mean);
    fprintf(file, "<tr><td><strong>Median</strong></td><td>%.4f</td></tr>\n", median);
    fprintf(file, "<tr><td><strong>Mode</strong></td><td>%.4f</td></tr>\n", mode);
    fprintf(file, "<tr><td><strong>Sample Variance</strong></td><td>%.4f</td></tr>\n", variance);
    fprintf(file, "<tr><td><strong>Sample Standard Deviation</strong></td><td>%.4f</td></tr>\n", stddev);
    fprintf(file, "</table>\n");
    
    fprintf(file, "<div class=\"footer\">\n");
    fprintf(file, "<p>&copy; Copyright Sanskar. Open-Source Project.</p>\n");
    fprintf(file, "<p><a href=\"https://www.github.com/Sanskar-in/Calc-Avg\">View on GitHub</a></p>\n");
    fprintf(file, "</div>\n</div>\n</body>\n</html>\n");
    
    fclose(file);
    printf(COLOR_GREEN "\n---> HTML Report successfully generated at: %s\n" COLOR_RESET, report_filename);
}
