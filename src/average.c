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
// File Purpose: Implementation of average calculation functions.

#include "average.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket close
#define WSACleanup()
#endif

double calc_avg_array(double numbers[], int count) {
    if (count <= 0) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += numbers[i];
    }
    return sum / count;
}

double calc_avg_natural(int n) {
    if (n <= 0) return 0.0;
    return (n + 1) / 2.0;
}

double calc_avg_odd(int n) {
    if (n <= 0) return 0.0;
    return (double)n;
}

double calc_avg_even(int n) {
    if (n <= 0) return 0.0;
    return (double)(n + 1);
}

bool is_prime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

double calc_avg_prime(int n) {
    if (n <= 0) return 0.0;
    double sum = 0.0;
    int count = 0;
    int num = 2;
    while (count < n) {
        if (is_prime(num)) {
            sum += num;
            count++;
        }
        num++;
    }
    return sum / n;
}

double calc_avg_fibonacci(int n) {
    if (n <= 0) return 0.0;
    if (n == 1) return 0.0;
    double sum = 0.0;
    long long a = 0;
    long long b = 1;
    long long c;
    
    sum += a;
    if (n > 1) {
        sum += b;
    }
    
    for (int i = 2; i < n; i++) {
        c = a + b;
        sum += c;
        a = b;
        b = c;
    }
    return sum / n;
}

double calc_avg_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'.\n", filename);
        return 0.0;
    }
    
    double sum = 0.0;
    double num;
    int count = 0;
    
    while (fscanf(file, "%lf", &num) == 1) {
        sum += num;
        count++;
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("Error: No numbers found in file '%s'.\n", filename);
        return 0.0;
    }
    
    return sum / count;
}

int compare_doubles(const void* a, const void* b) {
    double diff = *(const double*)a - *(const double*)b;
    if (diff > 0) return 1;
    if (diff < 0) return -1;
    return 0;
}

double calc_median(double numbers[], int count) {
    if (count <= 0) return 0.0;
    
    double *copy = (double *)malloc(count * sizeof(double));
    if (!copy) return 0.0;
    
    for (int i = 0; i < count; i++) {
        copy[i] = numbers[i];
    }
    
    qsort(copy, count, sizeof(double), compare_doubles);
    
    double median;
    if (count % 2 == 0) {
        median = (copy[count / 2 - 1] + copy[count / 2]) / 2.0;
    } else {
        median = copy[count / 2];
    }
    
    free(copy);
    return median;
}

double calc_mode(double numbers[], int count) {
    if (count <= 0) return 0.0;
    
    double *copy = (double *)malloc(count * sizeof(double));
    if (!copy) return 0.0;
    
    for (int i = 0; i < count; i++) {
        copy[i] = numbers[i];
    }
    
    qsort(copy, count, sizeof(double), compare_doubles);
    
    double mode = copy[0];
    int max_count = 1;
    int current_count = 1;
    
    for (int i = 1; i < count; i++) {
        if (copy[i] == copy[i - 1]) {
            current_count++;
        } else {
            if (current_count > max_count) {
                max_count = current_count;
                mode = copy[i - 1];
            }
            current_count = 1;
        }
    }
    
    if (current_count > max_count) {
        mode = copy[count - 1];
    }
    
    free(copy);
    return mode;
}

double calc_geometric_mean(double numbers[], int count) {
    if (count <= 0) return 0.0;
    double product = 1.0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] <= 0) {
            printf("Error: Geometric mean requires strictly positive numbers.\n");
            return 0.0;
        }
        product *= numbers[i];
    }
    return pow(product, 1.0 / count);
}

double calc_harmonic_mean(double numbers[], int count) {
    if (count <= 0) return 0.0;
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] == 0) {
            printf("Error: Harmonic mean cannot handle zero values.\n");
            return 0.0;
        }
        sum += 1.0 / numbers[i];
    }
    return count / sum;
}

double calc_avg_multi_file(char filenames[][256], int file_count) {
    if (file_count <= 0) return 0.0;
    
    double total_sum = 0.0;
    int total_count = 0;
    
    for (int i = 0; i < file_count; i++) {
        FILE *file = fopen(filenames[i], "r");
        if (file == NULL) {
            printf("Warning: Could not open file '%s'. Skipping...\n", filenames[i]);
            continue;
        }
        
        double num;
        while (fscanf(file, "%lf", &num) == 1) {
            total_sum += num;
            total_count++;
        }
        fclose(file);
    }
    
    if (total_count == 0) {
        printf("Error: No valid numbers found across the specified files.\n");
        return 0.0;
    }
    
    return total_sum / total_count;
}

double calc_avg_from_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open CSV file '%s'.\n", filename);
        return 0.0;
    }

    double sum = 0.0;
    int count = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",\n\r");
        while (token != NULL) {
            // Trim leading spaces
            while (isspace((unsigned char)*token)) token++;
            
            // Only parse if it's a number (basic check to skip headers)
            if (isdigit((unsigned char)token[0]) || token[0] == '-' || token[0] == '.') {
                char *endptr;
                double val = strtod(token, &endptr);
                if (endptr != token) {
                    sum += val;
                    count++;
                }
            }
            token = strtok(NULL, ",\n\r");
        }
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("Error: No valid numerical data found in CSV.\n");
        return 0.0;
    }
    return sum / count;
}

double calc_avg_from_json(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open JSON file '%s'.\n", filename);
        return 0.0;
    }

    double sum = 0.0;
    int count = 0;
    int ch;
    bool in_array = false;
    bool in_string = false;
    char num_buffer[64];
    int num_idx = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '"') {
            in_string = !in_string;
            continue;
        }
        if (in_string) continue;

        if (!in_array) {
            if (ch == '[') {
                in_array = true;
                num_idx = 0;
            }
        } else {
            if (ch == ']') {
                if (num_idx > 0) {
                    num_buffer[num_idx] = '\0';
                    char *endptr;
                    double val = strtod(num_buffer, &endptr);
                    if (endptr != num_buffer) {
                        sum += val;
                        count++;
                    }
                }
                break; // end of array
            } else if (ch == ',') {
                if (num_idx > 0) {
                    num_buffer[num_idx] = '\0';
                    char *endptr;
                    double val = strtod(num_buffer, &endptr);
                    if (endptr != num_buffer) {
                        sum += val;
                        count++;
                    }
                    num_idx = 0;
                }
            } else if (isdigit(ch) || ch == '-' || ch == '.' || ch == 'e' || ch == 'E' || ch == '+') {
                if (num_idx < 63) {
                    num_buffer[num_idx++] = (char)ch;
                }
            }
        }
    }

    fclose(file);
    
    if (count == 0) {
        printf("Error: No valid numerical data found in JSON.\n");
        return 0.0;
    }
    return sum / count;
}

double calc_avg_from_api(const char *hostname, const char *path) {
#if defined(_WIN32)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Error: WSAStartup failed.\n");
        return 0.0;
    }
#endif

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("Error: Could not create socket.\n");
        WSACleanup();
        return 0.0;
    }

    struct hostent *server = gethostbyname(hostname);
    if (server == NULL) {
        printf("Error: No such host '%s'.\n", hostname);
        closesocket(sock);
        WSACleanup();
        return 0.0;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error: Could not connect to host.\n");
        closesocket(sock);
        WSACleanup();
        return 0.0;
    }

    char request[1024];
    snprintf(request, sizeof(request), 
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n", path, hostname);

    send(sock, request, strlen(request), 0);

    FILE *temp_file = fopen("temp_api_data.json", "w");
    if (!temp_file) {
        printf("Error: Could not create temp file.\n");
        closesocket(sock);
        WSACleanup();
        return 0.0;
    }

    char buffer[4096];
    int bytes_received;
    bool headers_passed = false;
    char header_end[] = "\r\n\r\n";
    int header_idx = 0;

    while ((bytes_received = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        if (!headers_passed) {
            for (int i = 0; i < bytes_received; i++) {
                if (!headers_passed) {
                    if (buffer[i] == header_end[header_idx]) {
                        header_idx++;
                        if (header_idx == 4) {
                            headers_passed = true;
                            if (i + 1 < bytes_received) {
                                fwrite(buffer + i + 1, 1, bytes_received - (i + 1), temp_file);
                            }
                        }
                    } else {
                        header_idx = (buffer[i] == '\r') ? 1 : 0;
                    }
                } else {
                    fputc(buffer[i], temp_file);
                }
            }
        } else {
            fwrite(buffer, 1, bytes_received, temp_file);
        }
    }

    fclose(temp_file);
    closesocket(sock);
    WSACleanup();

    printf("\n[API] Successfully downloaded data. Analyzing JSON payload...\n");
    double avg = calc_avg_from_json("temp_api_data.json");
    remove("temp_api_data.json");
    return avg;
}

double calc_weighted_average(double values[], double weights[], int count) {
    if (count <= 0) return 0.0;
    double sum_product = 0.0;
    double sum_weights = 0.0;
    
    for (int i = 0; i < count; i++) {
        sum_product += values[i] * weights[i];
        sum_weights += weights[i];
    }
    
    if (sum_weights == 0) {
        printf("Error: Sum of weights is zero.\n");
        return 0.0;
    }
    
    return sum_product / sum_weights;
}

double calc_filtered_average(double numbers[], int count, double min_val, double max_val) {
    if (count <= 0) return 0.0;
    double sum = 0.0;
    int valid_count = 0;
    for (int i = 0; i < count; i++) {
        if (numbers[i] >= min_val && numbers[i] <= max_val) {
            sum += numbers[i];
            valid_count++;
        }
    }
    if (valid_count == 0) {
        printf("Error: No numbers found in the specified range.\n");
        return 0.0;
    }
    return sum / valid_count;
}

double calc_variance(double numbers[], int count, bool is_population) {
    if (count <= 1 && !is_population) return 0.0;
    if (count <= 0) return 0.0;

    double mean = calc_avg_array(numbers, count);
    double sum_squared_diff = 0.0;

    for (int i = 0; i < count; i++) {
        double diff = numbers[i] - mean;
        sum_squared_diff += diff * diff;
    }

    if (is_population) {
        return sum_squared_diff / count;
    } else {
        return sum_squared_diff / (count - 1);
    }
}

double calc_standard_deviation(double numbers[], int count, bool is_population) {
    return sqrt(calc_variance(numbers, count, is_population));
}

#include <pthread.h>

typedef struct {
    const char *filename;
    double sum;
    int count;
} ThreadData;

#if defined(_WIN32)
DWORD WINAPI ProcessFileThread(LPVOID lpParam) {
#else
void* ProcessFileThread(void* lpParam) {
#endif
    ThreadData *data = (ThreadData*)lpParam;
    data->sum = 0.0;
    data->count = 0;

    FILE *file = fopen(data->filename, "r");
    if (file == NULL) {
        // Fallback to CSV/JSON if not a pure txt? To keep it robust we can just check extensions,
        // but for simplicity, let's just do standard fscanf. If the user wants full JSON/CSV threading
        // we'd route through the specific parsers. We will use standard text parsing.
        printf("Error: Could not open file %s for thread processing.\n", data->filename);
        return 1;
    }

    // A slightly robust parser that mimics calc_avg_from_file's simple parser
    char buffer[256];
    while (fscanf(file, "%255s", buffer) == 1) {
        char *endptr;
        double number = strtod(buffer, &endptr);
        if (endptr != buffer) {
            data->sum += number;
            data->count++;
        }
    }
    fclose(file);
    return 0;
}

double calc_avg_from_batch_threaded(char *filenames[], int file_count) {
    if (file_count <= 0) return 0.0;
    
#if defined(_WIN32)
    printf("Starting parallel multi-threaded batch processing for %d files on Windows...\n", file_count);
    HANDLE *threads = (HANDLE*)malloc(file_count * sizeof(HANDLE));
    ThreadData *thread_data = (ThreadData*)malloc(file_count * sizeof(ThreadData));
    
    double total_sum = 0.0;
    int total_count = 0;

    for (int i = 0; i < file_count; i++) {
        thread_data[i].filename = filenames[i];
        thread_data[i].sum = 0.0;
        thread_data[i].count = 0;
        threads[i] = CreateThread(NULL, 0, ProcessFileThread, &thread_data[i], 0, NULL);
        if (threads[i] == NULL) {
            printf("Error: Failed to create thread for %s.\n", filenames[i]);
        }
    }

    // Wait for all threads to complete
    WaitForMultipleObjects(file_count, threads, TRUE, INFINITE);

    for (int i = 0; i < file_count; i++) {
        total_sum += thread_data[i].sum;
        total_count += thread_data[i].count;
        if (threads[i] != NULL) {
            CloseHandle(threads[i]);
        }
    }

    free(threads);
    free(thread_data);

    if (total_count == 0) {
        printf("No valid numbers found in any file.\n");
        return 0.0;
    }
    return total_sum / total_count;
#else
    printf("Starting parallel multi-threaded batch processing for %d files on POSIX...\n", file_count);
    pthread_t *threads = (pthread_t*)malloc(file_count * sizeof(pthread_t));
    ThreadData *thread_data = (ThreadData*)malloc(file_count * sizeof(ThreadData));
    
    double total_sum = 0.0;
    int total_count = 0;

    for (int i = 0; i < file_count; i++) {
        thread_data[i].filename = filenames[i];
        thread_data[i].sum = 0.0;
        thread_data[i].count = 0;
        if (pthread_create(&threads[i], NULL, (void* (*)(void*))ProcessFileThread, &thread_data[i]) != 0) {
            printf("Error: Failed to create thread for %s.\n", filenames[i]);
        }
    }

    for (int i = 0; i < file_count; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].sum;
        total_count += thread_data[i].count;
    }

    free(threads);
    free(thread_data);

    if (total_count == 0) {
        printf("No valid numbers found in any file.\n");
        return 0.0;
    }
    return total_sum / total_count;
#endif
}

#include "../third_party/sqlite/sqlite3.h"

double calc_avg_from_sqlite(const char *db_filename, const char *table_name, const char *column_name) {
    sqlite3 *db;
    sqlite3_stmt *res;
    
    int rc = sqlite3_open(db_filename, &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0.0;
    }
    
    // Construct the SQL Query dynamically
    char sql[512];
    snprintf(sql, sizeof(sql), "SELECT %s FROM %s;", column_name, table_name);
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0.0;
    }
    
    double sum = 0.0;
    int count = 0;
    
    while (sqlite3_step(res) == SQLITE_ROW) {
        // Read the column as a double natively via SQLite engine
        double val = sqlite3_column_double(res, 0);
        sum += val;
        count++;
    }
    
    sqlite3_finalize(res);
    sqlite3_close(db);
    
    if (count == 0) {
        printf("No valid data found in column %s of table %s.\n", column_name, table_name);
        return 0.0;
    }
    
    return sum / count;
}

#include "../include/utils.h"

double calc_avg_matrix(int rows, int cols, double **matrix) {
    if (rows <= 0 || cols <= 0 || matrix == NULL) return 0.0;
    
    double total_sum = 0.0;
    
    printf("\n" COLOR_CYAN COLOR_BOLD "--- 2D Matrix Analysis ---" COLOR_RESET "\n\n");
    
    for (int i = 0; i < rows; i++) {
        double row_sum = 0.0;
        printf("Row %d: | ", i + 1);
        for (int j = 0; j < cols; j++) {
            printf("%7.2f ", matrix[i][j]);
            row_sum += matrix[i][j];
            total_sum += matrix[i][j];
        }
        printf("| -> " COLOR_YELLOW "Avg: %7.2f" COLOR_RESET "\n", row_sum / cols);
    }
    
    printf("--------");
    for (int j = 0; j < cols; j++) {
        printf("---------");
    }
    printf("\n");
    
    printf("Col Avg:  ");
    for (int j = 0; j < cols; j++) {
        double col_sum = 0.0;
        for (int i = 0; i < rows; i++) {
            col_sum += matrix[i][j];
        }
        printf(COLOR_GREEN "%7.2f " COLOR_RESET, col_sum / rows);
    }
    printf("\n\n");
    
    return total_sum / (rows * cols);
}

void calc_financial_moving_averages(double prices[], int count, int window_size) {
    if (count <= 0 || window_size <= 0) return;
    if (window_size > count) {
        printf(COLOR_RED "Window size cannot be greater than the number of prices.\n" COLOR_RESET);
        return;
    }
    
    printf("\n" COLOR_CYAN COLOR_BOLD "--- Financial Market Analysis (%d-Period Moving Averages) ---" COLOR_RESET "\n\n", window_size);
    printf("Period | Price      | SMA (Simple) | EMA (Exponential)\n");
    printf("------------------------------------------------------\n");
    
    double ema_multiplier = 2.0 / (window_size + 1);
    double prev_ema = 0.0;
    
    for (int i = 0; i < count; i++) {
        double current_price = prices[i];
        
        // Calculate SMA
        double sma = 0.0;
        if (i >= window_size - 1) {
            double sum = 0.0;
            for (int j = i - window_size + 1; j <= i; j++) {
                sum += prices[j];
            }
            sma = sum / window_size;
        }
        
        // Calculate EMA
        double ema = 0.0;
        if (i == window_size - 1) {
            // First EMA is usually seeded with the first SMA
            ema = sma;
            prev_ema = ema;
        } else if (i > window_size - 1) {
            ema = (current_price * ema_multiplier) + (prev_ema * (1.0 - ema_multiplier));
            prev_ema = ema;
        }
        
        // Print Row
        printf("%6d | %10.2f | ", i + 1, current_price);
        
        if (i >= window_size - 1) {
            printf(COLOR_YELLOW "%12.2f" COLOR_RESET " | ", sma);
            printf(COLOR_GREEN "%17.2f" COLOR_RESET "\n", ema);
        } else {
            printf("           - |                 -\n");
        }
    }
    printf("\n");
}

void calc_engineering_calculus(double data_points[], int count, double step_size) {
    if (count < 2 || step_size <= 0) {
        printf(COLOR_RED "Calculus operations require at least 2 data points and a positive step size.\n" COLOR_RESET);
        return;
    }
    
    // Calculate Numerical Integral (Trapezoidal Rule)
    double integral = 0.0;
    for (int i = 0; i < count - 1; i++) {
        integral += (data_points[i] + data_points[i+1]) / 2.0;
    }
    integral *= step_size;
    
    printf("\n" COLOR_CYAN COLOR_BOLD "--- Engineering Calculus (Numerical Integrals & Derivatives) ---" COLOR_RESET "\n\n");
    printf(COLOR_GREEN "Total Integral (Accumulated Area): " COLOR_RESET "%10.4f\n\n", integral);
    
    printf(" Point |   Value    | Derivative (Rate of Change)\n");
    printf("---------------------------------------------------\n");
    
    // Calculate Derivatives
    for (int i = 0; i < count; i++) {
        double derivative = 0.0;
        
        if (i == 0) {
            // Forward Difference for the first point
            derivative = (data_points[1] - data_points[0]) / step_size;
        } else if (i == count - 1) {
            // Backward Difference for the last point
            derivative = (data_points[count-1] - data_points[count-2]) / step_size;
        } else {
            // Central Difference for internal points
            derivative = (data_points[i+1] - data_points[i-1]) / (2.0 * step_size);
        }
        
        printf("%6d | %10.4f | ", i + 1, data_points[i]);
        printf(COLOR_YELLOW "%12.4f" COLOR_RESET "\n", derivative);
    }
    printf("\n");
}

void calc_linear_regression(double data_points[], int count, int future_predictions) {
    if (count < 2) {
        printf(COLOR_RED "Linear Regression requires at least 2 historical data points to train the model.\n" COLOR_RESET);
        return;
    }
    
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_x2 = 0.0;
    
    for (int i = 0; i < count; i++) {
        double x = (double)(i + 1); // Time period (1, 2, 3...)
        double y = data_points[i];
        
        sum_x += x;
        sum_y += y;
        sum_xy += (x * y);
        sum_x2 += (x * x);
    }
    
    // Calculate Slope (m) and Y-Intercept (b)
    double denominator = (count * sum_x2) - (sum_x * sum_x);
    if (denominator == 0) {
        printf(COLOR_RED "Mathematical Error: Cannot calculate regression (divide by zero).\n" COLOR_RESET);
        return;
    }
    
    double m = ((count * sum_xy) - (sum_x * sum_y)) / denominator;
    double b = (sum_y - (m * sum_x)) / count;
    
    printf("\n" COLOR_CYAN COLOR_BOLD "--- Machine Learning: Predictive Linear Regression ---" COLOR_RESET "\n\n");
    printf(COLOR_YELLOW "Trained Mathematical Model (Line of Best Fit): " COLOR_RESET "y = %.4fx + %.4f\n\n", m, b);
    
    printf(" Period |   Actual Value   | Predicted Value\n");
    printf("---------------------------------------------\n");
    
    // Print Historical Data
    for (int i = 0; i < count; i++) {
        double x = (double)(i + 1);
        double predicted_y = (m * x) + b;
        
        printf("%7d | %16.4f | %15.4f\n", i + 1, data_points[i], predicted_y);
    }
    
    // Predict Future Data
    if (future_predictions > 0) {
        printf("---------------------------------------------\n");
        printf(COLOR_GREEN "        FUTURE PREDICTIONS (AI GENERATED)\n" COLOR_RESET);
        printf("---------------------------------------------\n");
        
        for (int i = count; i < count + future_predictions; i++) {
            double x = (double)(i + 1);
            double predicted_y = (m * x) + b;
            
            printf(COLOR_GREEN "%7d" COLOR_RESET " |                - | " COLOR_GREEN "%15.4f" COLOR_RESET "\n", i + 1, predicted_y);
        }
    }
    printf("\n");
}

void calc_rle_compression(double data_points[], int count) {
    if (count == 0) {
        printf(COLOR_RED "Error: No data to compress.\n" COLOR_RESET);
        return;
    }

    printf("\n" COLOR_CYAN COLOR_BOLD "--- Big Data: Run-Length Encoding (RLE) Compression ---" COLOR_RESET "\n\n");
    printf(COLOR_YELLOW "Original Data Size: " COLOR_RESET "%d data points (%.0f bytes in memory)\n\n", count, (double)(count * sizeof(double)));
    
    printf("Compressed Byte Stream:\n");
    printf("---------------------------------------------\n");
    
    double sum = 0.0;
    int total_compressed_blocks = 0;
    
    for (int i = 0; i < count; i++) {
        double current_val = data_points[i];
        int run_length = 1;
        
        // Count consecutive identical numbers
        while (i + 1 < count && data_points[i + 1] == current_val) {
            run_length++;
            i++;
        }
        
        // Decompress dynamically to calculate sum
        sum += (current_val * run_length);
        total_compressed_blocks++;
        
        // Print compressed block
        printf("[ %dx %.4f ] ", run_length, current_val);
        if (total_compressed_blocks % 5 == 0) printf("\n");
    }
    
    printf("\n\n---------------------------------------------\n");
    
    double compressed_size = (double)(total_compressed_blocks * (sizeof(int) + sizeof(double)));
    double original_size = (double)(count * sizeof(double));
    double compression_ratio = (1.0 - (compressed_size / original_size)) * 100.0;
    
    if (compression_ratio < 0) compression_ratio = 0.0; // In case of negative compression
    
    printf(COLOR_GREEN "Compressed Data Size: " COLOR_RESET "%d blocks (%.0f bytes)\n", total_compressed_blocks, compressed_size);
    printf(COLOR_YELLOW "Compression Ratio: " COLOR_RESET "Reduced memory footprint by %.2f%%\n\n", compression_ratio);
    
    double average = sum / count;
    printf(COLOR_GREEN "Decompressed Mathematical Average: " COLOR_RESET "%.4f\n", average);
}
