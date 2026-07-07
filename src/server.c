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

#include "../include/server.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 8192

// Helper function to serve a file from the web-app directory
void send_file(SOCKET client, const char* filepath, const char* content_type) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        char header[] = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n404 File Not Found.";
        send(client, header, strlen(header), 0);
        return;
    }
    
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *file_buf = (char *)malloc(fsize + 1);
    if (!file_buf) {
        fclose(file);
        return;
    }
    fread(file_buf, 1, fsize, file);
    fclose(file);
    
    char header[256];
    snprintf(header, sizeof(header), 
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n\r\n", content_type, fsize);
    
    send(client, header, strlen(header), 0);
    send(client, file_buf, fsize, 0);
    
    free(file_buf);
}

static int compare_doubles(const void *a, const void *b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da > db) - (da < db);
}

// Function to parse query parameter values dynamically
void parse_query_param(const char* query, const char* key, char* output, int out_len) {
    output[0] = '\0';
    char search_key[32];
    snprintf(search_key, sizeof(search_key), "%s=", key);
    const char *start = strstr(query, search_key);
    if (!start) return;
    
    start += strlen(search_key);
    const char *end = strchr(start, '&');
    if (!end) end = start + strlen(start);
    
    int len = end - start;
    if (len >= out_len) len = out_len - 1;
    strncpy(output, start, len);
    output[len] = '\0';
}

void decode_url(char* decoded, const char* data_ptr) {
    int j = 0;
    for (int i = 0; data_ptr[i] != '\0' && j < 2047; i++, j++) {
        if (data_ptr[i] == '%' && data_ptr[i+1] == '2' && (data_ptr[i+2] == 'C' || data_ptr[i+2] == 'c')) {
            decoded[j] = ','; i += 2;
        } else if (data_ptr[i] == '+') {
            decoded[j] = ' ';
        } else {
            decoded[j] = data_ptr[i];
        }
    }
    decoded[j] = '\0';
}

void parse_numbers(const char* decoded, double* numbers, int* count, double* sum) {
    *count = 0; *sum = 0;
    char temp[2048];
    strncpy(temp, decoded, sizeof(temp));
    char *token = strtok(temp, ", ");
    while (token != NULL && *count < 1000) {
        numbers[*count] = atof(token);
        *sum += numbers[*count];
        (*count)++;
        token = strtok(NULL, ", ");
    }
}

// The main web server loop
void launch_web_server(void) {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) return;

    SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) { WSACleanup(); return; }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(ListenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(ListenSocket); WSACleanup(); return;
    }

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(ListenSocket); WSACleanup(); return;
    }

    printf("\n" COLOR_CYAN COLOR_BOLD "--- Web Server: Calc-Avg Version 4.1 Unified API ---" COLOR_RESET "\n");
    printf(COLOR_GREEN "Server is LIVE and listening on port %d" COLOR_RESET "\n", PORT);
    printf(COLOR_YELLOW "Open your Web Browser and navigate to: http://localhost:%d\n" COLOR_RESET, PORT);
    printf("Serving Unified Web App from the 'web-app/' directory.\n");
    printf("Press Ctrl+C to stop the server and return to the terminal.\n\n");

    SOCKET ClientSocket;
    char buffer[BUFFER_SIZE];

    while (1) {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) continue;

        int bytesReceived = recv(ClientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            char method[16], path[1024];
            sscanf(buffer, "%15s %1023s", method, path);
            
            // API Endpoint: /api/calculate (Stats)
            if (strncmp(path, "/api/calculate", 14) == 0) {
                char data_param[2048];
                parse_query_param(path, "data", data_param, sizeof(data_param));
                
                char decoded[2048] = {0};
                decode_url(decoded, data_param);
                
                double numbers[1000];
                int count = 0; double sum = 0;
                parse_numbers(decoded, numbers, &count, &sum);
                
                if (count > 0) {
                    double mean = sum / count;
                    double sorted[1000];
                    for(int i=0; i<count; i++) sorted[i] = numbers[i];
                    qsort(sorted, count, sizeof(double), compare_doubles);
                    double median = (count % 2 == 0) ? (sorted[count/2 - 1] + sorted[count/2]) / 2.0 : sorted[count/2];
                    
                    double var_sum = 0;
                    for(int i=0; i<count; i++) var_sum += pow(numbers[i] - mean, 2);
                    double variance = var_sum / count;
                    double std_dev = sqrt(variance);
                    
                    char data_array_str[2048] = "[";
                    for(int i = 0; i < count; i++) {
                        char temp[32]; snprintf(temp, sizeof(temp), "%.4f%s", numbers[i], (i == count-1) ? "" : ",");
                        strcat(data_array_str, temp);
                    }
                    strcat(data_array_str, "]");
                    
                    char json_resp[4096];
                    snprintf(json_resp, sizeof(json_resp),
                        "{\"count\":%d, \"sum\":%.4f, \"mean\":%.4f, \"median\":%.4f, \"variance\":%.4f, \"std_dev\":%.4f, \"original_data\":%s}",
                        count, sum, mean, median, variance, std_dev, data_array_str);
                        
                    printf("API Request: Computed Stats for %d items.\n", count);
                    
                    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, header, strlen(header), 0);
                    send(ClientSocket, json_resp, strlen(json_resp), 0);
                } else {
                    char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"No valid data\"}";
                    send(ClientSocket, err, strlen(err), 0);
                }
            } 
            // API Endpoint: /api/predict (Linear Regression)
            else if (strncmp(path, "/api/predict", 12) == 0) {
                char data_param[2048], future_param[16];
                parse_query_param(path, "data", data_param, sizeof(data_param));
                parse_query_param(path, "future", future_param, sizeof(future_param));
                
                int future = atoi(future_param);
                if (future <= 0) future = 3;
                
                char decoded[2048] = {0};
                decode_url(decoded, data_param);
                
                double y_vals[1000];
                int n = 0; double sum_ignore = 0;
                parse_numbers(decoded, y_vals, &n, &sum_ignore);
                
                if (n >= 2) {
                    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x_sq = 0;
                    for (int i = 0; i < n; i++) {
                        double x = i + 1;
                        sum_x += x;
                        sum_y += y_vals[i];
                        sum_xy += x * y_vals[i];
                        sum_x_sq += x * x;
                    }
                    
                    double denominator = (n * sum_x_sq) - (sum_x * sum_x);
                    double m = 0, b = 0;
                    if (denominator != 0) {
                        m = ((n * sum_xy) - (sum_x * sum_y)) / denominator;
                        b = (sum_y - (m * sum_x)) / n;
                    }
                    
                    char data_array_str[2048] = "[";
                    for(int i = 0; i < n; i++) {
                        char temp[32]; snprintf(temp, sizeof(temp), "%.4f%s", y_vals[i], (i == n-1) ? "" : ",");
                        strcat(data_array_str, temp);
                    }
                    strcat(data_array_str, "]");
                    
                    char json_resp[4096];
                    snprintf(json_resp, sizeof(json_resp),
                        "{\"n\":%d, \"m\":%.4f, \"b\":%.4f, \"original_data\":%s}",
                        n, m, b, data_array_str);
                        
                    printf("API Request: Computed AI Predictions for %d items.\n", n);
                    
                    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, header, strlen(header), 0);
                    send(ClientSocket, json_resp, strlen(json_resp), 0);
                } else {
                    char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"Need at least 2 points\"}";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
            // API Endpoint: /api/sma (Simple Moving Average)
            else if (strncmp(path, "/api/sma", 8) == 0) {
                char data_param[2048], window_param[16];
                parse_query_param(path, "data", data_param, sizeof(data_param));
                parse_query_param(path, "window", window_param, sizeof(window_param));
                
                int window = atoi(window_param);
                if (window <= 1) window = 2;
                
                char decoded[2048] = {0};
                decode_url(decoded, data_param);
                
                double prices[1000];
                int count = 0; double sum_ignore = 0;
                parse_numbers(decoded, prices, &count, &sum_ignore);
                
                if (count >= window) {
                    char sma_array_str[4096] = "[";
                    
                    for (int i = 0; i < window - 1; i++) {
                        strcat(sma_array_str, "null,");
                    }
                    
                    for (int i = 0; i <= count - window; i++) {
                        double sum = 0;
                        for (int j = 0; j < window; j++) {
                            sum += prices[i + j];
                        }
                        char temp[32];
                        snprintf(temp, sizeof(temp), "%.4f%s", sum / window, (i == count - window) ? "" : ",");
                        strcat(sma_array_str, temp);
                    }
                    strcat(sma_array_str, "]");
                    
                    char prices_array_str[2048] = "[";
                    for(int i = 0; i < count; i++) {
                        char temp[32]; snprintf(temp, sizeof(temp), "%.4f%s", prices[i], (i == count-1) ? "" : ",");
                        strcat(prices_array_str, temp);
                    }
                    strcat(prices_array_str, "]");
                    
                    char json_resp[8192];
                    snprintf(json_resp, sizeof(json_resp),
                        "{\"count\":%d, \"window\":%d, \"original_data\":%s, \"sma\":%s}",
                        count, window, prices_array_str, sma_array_str);
                        
                    printf("API Request: Computed Financial SMA (Window %d) for %d items.\n", window, count);
                    
                    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, header, strlen(header), 0);
                    send(ClientSocket, json_resp, strlen(json_resp), 0);
                } else {
                    char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"Insufficient data\"}";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
            // Static File Serving Route (Serving from web-app/ directory)
            else {
                if (strcmp(path, "/") == 0) {
                    send_file(ClientSocket, "web-app/index.html", "text/html");
                    printf("Served: web-app/index.html\n");
                } else if (strcmp(path, "/style.css") == 0) {
                    send_file(ClientSocket, "web-app/style.css", "text/css");
                    printf("Served: web-app/style.css\n");
                } else if (strcmp(path, "/script.js") == 0) {
                    send_file(ClientSocket, "web-app/script.js", "application/javascript");
                    printf("Served: web-app/script.js\n");
                } else {
                    char err[] = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\nFile Not Found";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
        }
        closesocket(ClientSocket);
    }

    closesocket(ListenSocket);
    WSACleanup();
}
#else
void launch_web_server(void) {
    printf(COLOR_RED "Error: The Web Server engine is currently configured for Windows (winsock2) only.\n" COLOR_RESET);
}
#endif
