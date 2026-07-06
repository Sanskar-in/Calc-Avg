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
    bool in_string = false;
    char num_buffer[64];
    int num_idx = 0;

    // Custom state machine to extract raw numbers and ignore JSON formatting
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '"') {
            in_string = !in_string;
            continue;
        }
        
        if (!in_string) {
            if (isdigit(ch) || ch == '-' || ch == '.' || ch == 'e' || ch == 'E' || ch == '+') {
                if (num_idx < 63) {
                    num_buffer[num_idx++] = (char)ch;
                }
            } else {
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
            }
        }
    }
    
    // Check if file ended with a number
    if (num_idx > 0) {
        num_buffer[num_idx] = '\0';
        char *endptr;
        double val = strtod(num_buffer, &endptr);
        if (endptr != num_buffer) {
            sum += val;
            count++;
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
#else
    printf("Error: API Networking is currently only supported on Windows.\n");
    return 0.0;
#endif
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
