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

// Helper function to serve a file from the web directory
void send_file(SOCKET client, const char* filepath, const char* content_type) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        // Fallback for missing file
        char header[] = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n404 File Not Found.";
        send(client, header, strlen(header), 0);
        return;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Read file into buffer
    char *file_buf = (char *)malloc(fsize + 1);
    if (!file_buf) {
        fclose(file);
        return;
    }
    fread(file_buf, 1, fsize, file);
    fclose(file);
    
    // Send HTTP Header
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

// Helper to sort for median
static int compare_doubles(const void *a, const void *b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da > db) - (da < db);
}

// The main web server loop
void launch_web_server(void) {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }

    SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(ListenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    printf("\n" COLOR_CYAN COLOR_BOLD "--- Web Server: Calc-Avg Version 3.1 REST API & Dashboard ---" COLOR_RESET "\n");
    printf(COLOR_GREEN "Server is LIVE and listening on port %d" COLOR_RESET "\n", PORT);
    printf(COLOR_YELLOW "Open your Web Browser and navigate to: http://localhost:%d\n" COLOR_RESET, PORT);
    printf("Serving files from the 'web/' directory.\n");
    printf("Press Ctrl+C to stop the server and return to the terminal.\n\n");

    SOCKET ClientSocket;
    char buffer[BUFFER_SIZE];

    while (1) {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            continue;
        }

        int bytesReceived = recv(ClientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            // Extract the requested path
            char method[16], path[1024];
            sscanf(buffer, "%15s %1023s", method, path);
            
            // API Endpoint: /api/calculate
            if (strncmp(path, "/api/calculate", 14) == 0) {
                // Parse Query String
                char *data_ptr = strstr(path, "data=");
                char *op_ptr = strstr(path, "op=");
                
                if (data_ptr) {
                    data_ptr += 5; // skip 'data='
                    char *amp = strchr(data_ptr, '&');
                    if (amp) *amp = '\0'; // terminate at next param
                    
                    int is_all_stats = (op_ptr && strncmp(op_ptr + 3, "all", 3) == 0);
                    
                    // Decode URL entities (%2C for comma, + for space)
                    char decoded[2048] = {0};
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

                    // Parse numbers
                    double numbers[1000];
                    int count = 0;
                    double sum = 0;
                    
                    char *token = strtok(decoded, ", ");
                    while (token != NULL && count < 1000) {
                        numbers[count] = atof(token);
                        sum += numbers[count];
                        count++;
                        token = strtok(NULL, ", ");
                    }
                    
                    if (count > 0) {
                        double mean = sum / count;
                        
                        // Construct JSON Response
                        char json_resp[4096];
                        char data_array_str[2048] = "[";
                        
                        // Build original_data JSON array
                        for(int i = 0; i < count; i++) {
                            char temp[32];
                            snprintf(temp, sizeof(temp), "%.4f%s", numbers[i], (i == count-1) ? "" : ",");
                            strcat(data_array_str, temp);
                        }
                        strcat(data_array_str, "]");
                        
                        if (is_all_stats) {
                            // Calculate Median
                            double sorted[1000];
                            for(int i=0; i<count; i++) sorted[i] = numbers[i];
                            qsort(sorted, count, sizeof(double), compare_doubles);
                            double median = (count % 2 == 0) ? (sorted[count/2 - 1] + sorted[count/2]) / 2.0 : sorted[count/2];
                            
                            // Calculate Mode (simplified)
                            double mode = sorted[0];
                            int max_count = 1, current_count = 1;
                            for (int i = 1; i < count; i++) {
                                if (sorted[i] == sorted[i-1]) {
                                    current_count++;
                                } else {
                                    if (current_count > max_count) {
                                        max_count = current_count;
                                        mode = sorted[i-1];
                                    }
                                    current_count = 1;
                                }
                            }
                            if (current_count > max_count) mode = sorted[count-1];
                            
                            // Calculate Variance and Std Dev (Population)
                            double var_sum = 0;
                            for(int i=0; i<count; i++) {
                                var_sum += pow(numbers[i] - mean, 2);
                            }
                            double variance = var_sum / count;
                            double std_dev = sqrt(variance);
                            
                            snprintf(json_resp, sizeof(json_resp),
                                "{\"count\":%d, \"sum\":%.4f, \"mean\":%.4f, \"median\":%.4f, \"mode\":%.4f, \"variance\":%.4f, \"std_dev\":%.4f, \"original_data\":%s}",
                                count, sum, mean, median, mode, variance, std_dev, data_array_str);
                                
                            printf("API Request: Computed Comprehensive Stats for %d items.\n", count);
                        } else {
                            snprintf(json_resp, sizeof(json_resp),
                                "{\"count\":%d, \"sum\":%.4f, \"mean\":%.4f, \"original_data\":%s}",
                                count, sum, mean, data_array_str);
                                
                            printf("API Request: Computed Mean for %d items.\n", count);
                        }
                        
                        char header[256];
                        snprintf(header, sizeof(header),
                                 "HTTP/1.1 200 OK\r\n"
                                 "Content-Type: application/json\r\n"
                                 "Connection: close\r\n\r\n");
                                 
                        send(ClientSocket, header, strlen(header), 0);
                        send(ClientSocket, json_resp, strlen(json_resp), 0);
                    } else {
                        char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"No valid numbers provided\"}";
                        send(ClientSocket, err, strlen(err), 0);
                    }
                }
            } 
            // Static File Serving Route
            else {
                if (strcmp(path, "/") == 0) {
                    // Serve index.html from web folder
                    send_file(ClientSocket, "web/index.html", "text/html");
                    printf("Served: web/index.html\n");
                } else if (strcmp(path, "/style.css") == 0) {
                    send_file(ClientSocket, "web/style.css", "text/css");
                    printf("Served: web/style.css\n");
                } else if (strcmp(path, "/app.js") == 0) {
                    send_file(ClientSocket, "web/app.js", "application/javascript");
                    printf("Served: web/app.js\n");
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
