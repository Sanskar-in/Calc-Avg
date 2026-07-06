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
// File Purpose: Main entry point for the Calc-Avg project, orchestrating the menu logic.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "average.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "--file") == 0) {
            if (argc == 3) {
                double avg = calc_avg_from_file(argv[2]);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of the numbers in '%s' is: %.4f\n" COLOR_RESET, argv[2], avg);
                    log_history("CLI File Average", avg);
                }
            } else {
                printf(COLOR_RED "Usage: %s --file <filename>\n" COLOR_RESET, argv[0]);
            }
            return 0;
        } else if (strcmp(argv[1], "--api") == 0) {
            if (argc == 4) {
                double avg = calc_avg_from_api(argv[2], argv[3]);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of the live numbers at '%s%s' is: %.4f\n" COLOR_RESET, argv[2], argv[3], avg);
                    log_history("CLI API Average", avg);
                }
            } else {
                printf(COLOR_RED "Usage: %s --api <hostname> <path>\n" COLOR_RESET, argv[0]);
            }
            return 0;
        } else if (strcmp(argv[1], "--sqlite") == 0) {
            if (argc == 5) {
                double avg = calc_avg_from_sqlite(argv[2], argv[3], argv[4]);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of %s.%s is: %.4f\n" COLOR_RESET, argv[3], argv[4], avg);
                    log_history("CLI SQLite Average", avg);
                }
            } else {
                printf(COLOR_RED "Usage: %s --sqlite <db_file> <table_name> <column_name>\n" COLOR_RESET, argv[0]);
            }
            return 0;
        } else if (strcmp(argv[1], "--report") == 0) {
            if (argc >= 3) {
                int count = argc - 2;
                double *numbers = (double *)malloc(count * sizeof(double));
                if (!numbers) return 1;
                for (int i = 0; i < count; i++) {
                    numbers[i] = atof(argv[i + 2]);
                }
                generate_html_report(numbers, count, "report.html");
                log_history("CLI HTML Report Generated", 0.0);
                free(numbers);
            } else {
                printf(COLOR_RED "Usage: %s --report <num1> <num2> ...\n" COLOR_RESET, argv[0]);
            }
            return 0;
        } else if (strcmp(argv[1], "--csv") == 0) {
            if (argc == 3) {
                double avg = calc_avg_from_csv(argv[2]);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of the numbers in CSV '%s' is: %.4f\n" COLOR_RESET, argv[2], avg);
                    log_history("CLI CSV Average", avg);
                }
            } else {
                printf(COLOR_RED "Usage: %s --csv <filename>\n" COLOR_RESET, argv[0]);
            }
            return 0;
        } else if (strcmp(argv[1], "--json") == 0) {
            if (argc == 3) {
                double avg = calc_avg_from_json(argv[2]);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of the numbers in JSON '%s' is: %.4f\n" COLOR_RESET, argv[2], avg);
                    log_history("CLI JSON Average", avg);
                }
            } else {
                printf(COLOR_RED "Usage: %s --json <filename>\n" COLOR_RESET, argv[0]);
            }
            return 0;
        } else if (strcmp(argv[1], "--batch") == 0) {
            if (argc >= 3) {
                int count = argc - 2;
                double avg = calc_avg_from_batch_threaded(&argv[2], count);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The multi-threaded average of %d files is: %.4f\n" COLOR_RESET, count, avg);
                    log_history("CLI Multi-Threaded Batch Average", avg);
                }
            } else {
                printf(COLOR_RED "Usage: %s --batch <file1> <file2> ...\n" COLOR_RESET, argv[0]);
            }
            return 0;
        } else {
            // Assume the arguments are numbers
            int count = argc - 1;
            double *numbers = (double *)malloc(count * sizeof(double));
            if (!numbers) return 1;
            for (int i = 0; i < count; i++) {
                numbers[i] = atof(argv[i + 1]);
            }
            double avg = calc_avg_array(numbers, count);
            print_ascii_bar_chart(numbers, count);
            printf(COLOR_GREEN "\n---> The average of the %d numbers is: %.4f\n" COLOR_RESET, count, avg);
            log_history("CLI Array Average", avg);
            free(numbers);
            return 0;
        }
    }

    int choice;
    double last_result = 0.0;
    int has_last_result = 0;
    
    printf(COLOR_CYAN COLOR_BOLD "// =============================================================================\n" COLOR_RESET);
    printf(COLOR_CYAN COLOR_BOLD "// (c) Copyright Sanskar. All rights reserved.\n" COLOR_RESET);
    printf(COLOR_CYAN COLOR_BOLD "// Made by the Sanskar.\n" COLOR_RESET);
    printf(COLOR_CYAN COLOR_BOLD "// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- \"https://www.github.com/Sanskar-in/Calc-Avg\"\n" COLOR_RESET);
    printf(COLOR_CYAN COLOR_BOLD "// =============================================================================\n" COLOR_RESET);
    printf(COLOR_GREEN COLOR_BOLD "\nWelcome to Calc-Avg by Sanskar!\n" COLOR_RESET);

    while (1) {
        display_menu();
        choice = read_integer();

        switch (choice) {
            case 1: {
                int count;
                printf("How many numbers do you want to enter? ");
                count = read_integer();
                if (count <= 0) {
                    printf(COLOR_RED "Please enter a positive number.\n" COLOR_RESET);
                    break;
                }
                double *numbers = (double *)malloc(count * sizeof(double));
                if (numbers == NULL) break;
                for (int i = 0; i < count; i++) {
                    printf("Enter number %d: ", i + 1);
                    numbers[i] = read_double();
                }
                double avg = calc_avg_array(numbers, count);
                print_ascii_bar_chart(numbers, count);
                printf(COLOR_GREEN "\n---> The average of the %d numbers is: %.4f\n" COLOR_RESET, count, avg);
                
                last_result = avg;
                has_last_result = 1;
                log_history("Dynamic Array Average", avg);
                free(numbers);
                break;
            }
            case 2: {
                int n;
                printf("Enter the value of N (Natural numbers): ");
                n = read_integer();
                if (n > 0) {
                    double avg = calc_avg_natural(n);
                    printf(COLOR_GREEN "\n---> The average of the first %d natural numbers is: %.4f\n" COLOR_RESET, n, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("First N Natural Average", avg);
                } else {
                    printf(COLOR_RED "N must be a positive integer.\n" COLOR_RESET);
                }
                break;
            }
            case 3: {
                int n;
                printf("Enter the value of N (Odd numbers): ");
                n = read_integer();
                if (n > 0) {
                    double avg = calc_avg_odd(n);
                    printf(COLOR_GREEN "\n---> The average of the first %d odd numbers is: %.4f\n" COLOR_RESET, n, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("First N Odd Average", avg);
                } else {
                    printf(COLOR_RED "N must be a positive integer.\n" COLOR_RESET);
                }
                break;
            }
            case 4: {
                int n;
                printf("Enter the value of N (Even numbers): ");
                n = read_integer();
                if (n > 0) {
                    double avg = calc_avg_even(n);
                    printf(COLOR_GREEN "\n---> The average of the first %d even numbers is: %.4f\n" COLOR_RESET, n, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("First N Even Average", avg);
                } else {
                    printf(COLOR_RED "N must be a positive integer.\n" COLOR_RESET);
                }
                break;
            }
            case 5: {
                int n;
                printf("Enter the value of N (Prime numbers): ");
                n = read_integer();
                if (n > 0) {
                    double avg = calc_avg_prime(n);
                    printf(COLOR_GREEN "\n---> The average of the first %d prime numbers is: %.4f\n" COLOR_RESET, n, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("First N Prime Average", avg);
                } else {
                    printf(COLOR_RED "N must be a positive integer.\n" COLOR_RESET);
                }
                break;
            }
            case 6: {
                int n;
                printf("Enter the value of N (Fibonacci numbers): ");
                n = read_integer();
                if (n > 0) {
                    double avg = calc_avg_fibonacci(n);
                    printf(COLOR_GREEN "\n---> The average of the first %d Fibonacci numbers is: %.4f\n" COLOR_RESET, n, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("First N Fibonacci Average", avg);
                } else {
                    printf(COLOR_RED "N must be a positive integer.\n" COLOR_RESET);
                }
                break;
            }
            case 7: {
                char filename[256];
                printf("Enter the filename (e.g., numbers.txt): ");
                read_string(filename, sizeof(filename));
                double avg = calc_avg_from_file(filename);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of the numbers in '%s' is: %.4f\n" COLOR_RESET, filename, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("File Reader Average", avg);
                }
                break;
            }
            case 8:
            case 9:
            case 10:
            case 11: {
                int count;
                printf("How many numbers do you want to enter for this statistical operation? ");
                count = read_integer();
                if (count <= 0) {
                    printf(COLOR_RED "Please enter a positive number.\n" COLOR_RESET);
                    break;
                }
                double *numbers = (double *)malloc(count * sizeof(double));
                if (numbers == NULL) break;
                for (int i = 0; i < count; i++) {
                    printf("Enter number %d: ", i + 1);
                    numbers[i] = read_double();
                }
                
                double avg = 0.0;
                if (choice == 8) {
                    avg = calc_median(numbers, count);
                    printf(COLOR_GREEN "\n---> The median is: %.4f\n" COLOR_RESET, avg);
                    log_history("Median", avg);
                } else if (choice == 9) {
                    avg = calc_mode(numbers, count);
                    printf(COLOR_GREEN "\n---> The mode is: %.4f\n" COLOR_RESET, avg);
                    log_history("Mode", avg);
                } else if (choice == 10) {
                    avg = calc_geometric_mean(numbers, count);
                    if (avg != 0.0) {
                        printf(COLOR_GREEN "\n---> The geometric mean is: %.4f\n" COLOR_RESET, avg);
                        log_history("Geometric Mean", avg);
                    }
                } else if (choice == 11) {
                    avg = calc_harmonic_mean(numbers, count);
                    if (avg != 0.0) {
                        printf(COLOR_GREEN "\n---> The harmonic mean is: %.4f\n" COLOR_RESET, avg);
                        log_history("Harmonic Mean", avg);
                    }
                }
                
                if (avg != 0.0 || choice == 8 || choice == 9) {
                    last_result = avg;
                    has_last_result = 1;
                }
                
                free(numbers);
                break;
            }
            case 12: {
                int count;
                printf("How many pairs of (value, weight) do you want to enter? ");
                count = read_integer();
                if (count <= 0) {
                    printf(COLOR_RED "Please enter a positive number.\n" COLOR_RESET);
                    break;
                }
                double *values = (double *)malloc(count * sizeof(double));
                double *weights = (double *)malloc(count * sizeof(double));
                if (values == NULL || weights == NULL) break;
                
                for (int i = 0; i < count; i++) {
                    printf("Enter value %d: ", i + 1);
                    values[i] = read_double();
                    printf("Enter weight %d: ", i + 1);
                    weights[i] = read_double();
                }
                
                double avg = calc_weighted_average(values, weights, count);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The weighted average is: %.4f\n" COLOR_RESET, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("Weighted Average", avg);
                }
                
                free(values);
                free(weights);
                break;
            }
            case 13:
                if (has_last_result) {
                    export_result(last_result);
                } else {
                    printf(COLOR_RED "\nNo calculations performed yet to export.\n" COLOR_RESET);
                }
                break;
            case 14: {
                int count;
                printf("How many numbers do you want to enter? ");
                count = read_integer();
                if (count <= 0) {
                    printf(COLOR_RED "Please enter a positive number.\n" COLOR_RESET);
                    break;
                }
                double *numbers = (double *)malloc(count * sizeof(double));
                if (numbers == NULL) break;
                for (int i = 0; i < count; i++) {
                    printf("Enter number %d: ", i + 1);
                    numbers[i] = read_double();
                }
                
                printf("Enter minimum value for the range: ");
                double min_val = read_double();
                printf("Enter maximum value for the range: ");
                double max_val = read_double();
                
                double avg = calc_filtered_average(numbers, count, min_val, max_val);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The filtered average is: %.4f\n" COLOR_RESET, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("Filtered Range Average", avg);
                }
                free(numbers);
                break;
            }
            case 15:
            case 16: {
                int count;
                printf("How many numbers do you want to enter? ");
                count = read_integer();
                if (count <= 0) {
                    printf(COLOR_RED "Please enter a positive number.\n" COLOR_RESET);
                    break;
                }
                double *numbers = (double *)malloc(count * sizeof(double));
                if (numbers == NULL) break;
                for (int i = 0; i < count; i++) {
                    printf("Enter number %d: ", i + 1);
                    numbers[i] = read_double();
                }
                
                printf("Calculate for Population (1) or Sample (0)? ");
                int is_pop = read_integer();
                
                double result = 0.0;
                if (choice == 15) {
                    result = calc_variance(numbers, count, is_pop != 0);
                    printf(COLOR_GREEN "\n---> The variance is: %.4f\n" COLOR_RESET, result);
                    log_history("Variance", result);
                } else if (choice == 16) {
                    result = calc_standard_deviation(numbers, count, is_pop != 0);
                    printf(COLOR_GREEN "\n---> The standard deviation is: %.4f\n" COLOR_RESET, result);
                    log_history("Standard Deviation", result);
                }
                
                last_result = result;
                has_last_result = 1;
                free(numbers);
                break;
            }
            case 17:
                view_history();
                break;
            case 18: {
                int file_count;
                printf("How many files do you want to process? ");
                file_count = read_integer();
                if (file_count <= 0) {
                    printf(COLOR_RED "Please enter a positive number.\n" COLOR_RESET);
                    break;
                }
                
                char **filenames = malloc(file_count * sizeof(char*));
                if (!filenames) break;
                
                for (int i = 0; i < file_count; i++) {
                    filenames[i] = malloc(256);
                    printf("Enter filename %d: ", i + 1);
                    read_string(filenames[i], 256);
                }
                
                double avg = calc_avg_from_batch_threaded(filenames, file_count);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The multi-threaded average across %d files is: %.4f\n" COLOR_RESET, file_count, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("Multi-Threaded Batch Average", avg);
                }
                
                for (int i = 0; i < file_count; i++) {
                    free(filenames[i]);
                }
                free(filenames);
                break;
            }
            case 19: {
                char filename[256];
                printf("Enter the CSV filename: ");
                read_string(filename, sizeof(filename));
                double avg = calc_avg_from_csv(filename);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of the numbers in CSV '%s' is: %.4f\n" COLOR_RESET, filename, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("CSV Average", avg);
                }
                break;
            }
            case 20: {
                char filename[256];
                printf("Enter the JSON filename: ");
                read_string(filename, sizeof(filename));
                double avg = calc_avg_from_json(filename);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of the numbers in JSON '%s' is: %.4f\n" COLOR_RESET, filename, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("JSON Average", avg);
                }
                break;
            }
            case 21: {
                int count;
                printf("How many numbers do you want to include in the report? ");
                count = read_integer();
                if (count <= 0) {
                    printf(COLOR_RED "Please enter a positive number.\n" COLOR_RESET);
                    break;
                }
                
                double *numbers = (double *)malloc(count * sizeof(double));
                if (numbers == NULL) {
                    printf(COLOR_RED "Memory allocation failed.\n" COLOR_RESET);
                    break;
                }
                
                for (int i = 0; i < count; i++) {
                    printf("Enter number %d: ", i + 1);
                    numbers[i] = read_double();
                }
                
                generate_html_report(numbers, count, "report.html");
                log_history("HTML Report Generated", 0.0);
                free(numbers);
                break;
            }
            case 22: {
                char hostname[256];
                char path[256];
                printf("Enter the API Hostname (e.g., api.example.com): ");
                read_string(hostname, sizeof(hostname));
                printf("Enter the API Path (e.g., /data.json): ");
                read_string(path, sizeof(path));
                
                double avg = calc_avg_from_api(hostname, path);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of the live numbers at '%s%s' is: %.4f\n" COLOR_RESET, hostname, path, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("Live API Average", avg);
                }
                break;
            }
            case 23:
                printf(COLOR_CYAN COLOR_BOLD "\n=========================================\n" COLOR_RESET);
                printf(COLOR_YELLOW "Follow Sanskar or Creator on:-\n" COLOR_RESET);
                printf("GitHub:- " COLOR_GREEN "https://www.github.com/Sanskar-in\n" COLOR_RESET);
                printf("YouTube:- " COLOR_GREEN "https://youtube.com/@Sanskar-in\n" COLOR_RESET);
                printf("LinkedIn:- " COLOR_GREEN "https://www.linkedin.com/in/sanskar-in/\n" COLOR_RESET);
                printf("X (Formally known as Twitter):- " COLOR_GREEN "https://www.x.com/Sanskar_in\n" COLOR_RESET);
                printf("Need support:- " COLOR_GREEN "supportramsandesh@gmail.com\n" COLOR_RESET);
                printf("Want to make your own projects, apps, and games via Sanskar:- " COLOR_GREEN "sanskaryadavfrom2012to2026@gmail.com\n" COLOR_RESET);
                printf(COLOR_CYAN COLOR_BOLD "=========================================\n" COLOR_RESET);
                break;
            case 24: {
                char db_filename[256];
                char table_name[256];
                char column_name[256];
                printf("Enter the SQLite Database filename (e.g., data.db): ");
                read_string(db_filename, sizeof(db_filename));
                printf("Enter the Table Name: ");
                read_string(table_name, sizeof(table_name));
                printf("Enter the Numerical Column Name: ");
                read_string(column_name, sizeof(column_name));
                
                double avg = calc_avg_from_sqlite(db_filename, table_name, column_name);
                if (avg != 0.0) {
                    printf(COLOR_GREEN "\n---> The average of %s.%s in '%s' is: %.4f\n" COLOR_RESET, table_name, column_name, db_filename, avg);
                    last_result = avg;
                    has_last_result = 1;
                    log_history("SQLite Database Average", avg);
                }
                break;
            }
            case 25: {
                int rows, cols;
                printf("Enter the number of Rows: ");
                rows = read_integer();
                printf("Enter the number of Columns: ");
                cols = read_integer();
                
                if (rows <= 0 || cols <= 0) {
                    printf(COLOR_RED "Invalid dimensions.\n" COLOR_RESET);
                    break;
                }
                
                double **matrix = (double **)malloc(rows * sizeof(double *));
                for (int i = 0; i < rows; i++) {
                    matrix[i] = (double *)malloc(cols * sizeof(double));
                }
                
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        printf("Enter value for Row %d, Col %d: ", i + 1, j + 1);
                        matrix[i][j] = read_double();
                    }
                }
                
                double avg = calc_avg_matrix(rows, cols, matrix);
                printf(COLOR_GREEN "---> The Total Average of the entire %dx%d matrix is: %.4f\n" COLOR_RESET, rows, cols, avg);
                last_result = avg;
                has_last_result = 1;
                log_history("2D Matrix Average", avg);
                
                for (int i = 0; i < rows; i++) {
                    free(matrix[i]);
                }
                free(matrix);
                break;
            }
            case 26: {
                int count, window_size;
                printf("Enter the total number of periods (e.g. days) to analyze: ");
                count = read_integer();
                if (count <= 0) {
                    printf(COLOR_RED "Please enter a positive number.\n" COLOR_RESET);
                    break;
                }
                
                double *prices = (double *)malloc(count * sizeof(double));
                if (!prices) break;
                
                for (int i = 0; i < count; i++) {
                    printf("Enter Price for Period %d: ", i + 1);
                    prices[i] = read_double();
                }
                
                printf("Enter the Moving Average Window Size (e.g. 5, 14, 50): ");
                window_size = read_integer();
                
                calc_financial_moving_averages(prices, count, window_size);
                
                free(prices);
                break;
            }
            case 27:
                printf(COLOR_CYAN "\nRedirecting to GitHub: https://www.github.com/Sanskar-in/Calc-Avg\n" COLOR_RESET);
#if defined(_WIN32)
                system("start https://www.github.com/Sanskar-in/Calc-Avg");
#elif defined(__APPLE__)
                system("open https://www.github.com/Sanskar-in/Calc-Avg");
#else
                system("xdg-open https://www.github.com/Sanskar-in/Calc-Avg");
#endif
                break;
            case 28:
                printf(COLOR_CYAN COLOR_BOLD "\nExiting Calc-Avg. Thank you for using this open-source project by Sanskar!\n" COLOR_RESET);
                return 0;
            default:
                printf(COLOR_RED "Invalid choice. Please select an option between 1 and 28.\n" COLOR_RESET);
                break;
        }
    }

    return 0;
}
