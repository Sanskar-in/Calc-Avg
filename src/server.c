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

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

// HTML template with styling and form
const char* html_page = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n"
"Connection: close\r\n\r\n"
"<!DOCTYPE html>"
"<html><head><title>Calc-Avg Web Portal</title>"
"<style>"
"body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: #0f172a; color: #f8fafc; text-align: center; padding: 50px; }"
"h1 { color: #38bdf8; }"
"input { padding: 10px; font-size: 16px; border-radius: 5px; border: none; width: 300px; }"
"button { padding: 10px 20px; font-size: 16px; border-radius: 5px; border: none; background: #38bdf8; color: #0f172a; cursor: pointer; font-weight: bold; margin-left: 10px; }"
"button:hover { background: #0284c7; }"
".result { margin-top: 30px; font-size: 24px; color: #4ade80; }"
"</style>"
"</head><body>"
"<h1>Calc-Avg Web Server</h1>"
"<p>Enter numbers separated by commas to calculate the mathematical average directly through the C Backend.</p>"
"<form action='/calc' method='GET'>"
"<input type='text' name='data' placeholder='e.g., 10, 20, 30' required />"
"<button type='submit'>Calculate Average</button>"
"</form>"
"%s" // Placeholder for result
"</body></html>";

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

    printf("\n" COLOR_CYAN COLOR_BOLD "--- Web Server: Calc-Avg Browser Portal ---" COLOR_RESET "\n");
    printf(COLOR_GREEN "Server is LIVE and listening on port %d" COLOR_RESET "\n", PORT);
    printf(COLOR_YELLOW "Open your Web Browser and navigate to: http://localhost:%d\n" COLOR_RESET, PORT);
    printf("Press Ctrl+C to stop the server and return to the terminal.\n\n");

    SOCKET ClientSocket;
    char buffer[BUFFER_SIZE];

    while (1) {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("Accept failed: %d\n", WSAGetLastError());
            continue;
        }

        int bytesReceived = recv(ClientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            // Check if it's a GET request for /calc
            if (strncmp(buffer, "GET /calc?data=", 15) == 0) {
                // Extract data
                char *data_start = buffer + 15;
                char *data_end = strchr(data_start, ' ');
                if (data_end) *data_end = '\0';

                // Decode URL entities (e.g. %2C for comma)
                char decoded[1024] = {0};
                int j = 0;
                for (int i = 0; data_start[i] != '\0' && j < 1023; i++, j++) {
                    if (data_start[i] == '%' && data_start[i+1] == '2' && (data_start[i+2] == 'C' || data_start[i+2] == 'c')) {
                        decoded[j] = ',';
                        i += 2;
                    } else if (data_start[i] == '+') {
                        decoded[j] = ' ';
                    } else {
                        decoded[j] = data_start[i];
                    }
                }

                // Parse commas and calculate average
                double sum = 0;
                int count = 0;
                char *token = strtok(decoded, ", ");
                while (token != NULL) {
                    sum += atof(token);
                    count++;
                    token = strtok(NULL, ", ");
                }

                char result_html[512];
                if (count > 0) {
                    double average = sum / count;
                    snprintf(result_html, sizeof(result_html), "<div class='result'>Mathematical Average: <strong>%.4f</strong> (from %d numbers)</div>", average, count);
                    printf("Web Request Processed: Calculated Average %.4f for %d numbers.\n", average, count);
                } else {
                    snprintf(result_html, sizeof(result_html), "<div class='result' style='color:#ef4444;'>Error: No valid numbers found.</div>");
                }

                char response[BUFFER_SIZE];
                snprintf(response, sizeof(response), html_page, result_html);
                send(ClientSocket, response, (int)strlen(response), 0);

            } else {
                // Serve root page
                char response[BUFFER_SIZE];
                snprintf(response, sizeof(response), html_page, "");
                send(ClientSocket, response, (int)strlen(response), 0);
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
