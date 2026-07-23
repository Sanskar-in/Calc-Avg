// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/sanskarIN/Calc-Avg"
//
// Follow Sanskar or Creator on:-
// GitHub:- "https://www.github.com/sanskarIN"
// YouTube:- "https://youtube.com/@sanskarIN"
// LinkedIn:- "https://www.linkedin.com/in/sanskar-in/"
// X (Formally known as Twitter):- "https://www.x.com/Sanskar_in"
// Need support:- supportramsandesh@gmail.com
// Want to make your own projects, apps, and games via Sanskar:- sanskaryadavfrom2012to2026@gmail.com
// =============================================================================

#include "../include/gui.h"
#include "../include/average.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

#define ID_INPUT_TEXT 101
#define ID_CALC_BUTTON 102
#define ID_RESULT_LABEL 103

// Global handles
HWND hInputBox, hCalcButton, hResultLabel;

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
        {
            HFONT hFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");
            
            HWND hLabel1 = CreateWindowW(L"Static", L"Enter numbers separated by commas (e.g., 10, 20, 30):",
                          WS_VISIBLE | WS_CHILD, 20, 20, 400, 20, hwnd, NULL, NULL, NULL);
            SendMessage(hLabel1, WM_SETFONT, (WPARAM)hFont, TRUE);

            hInputBox = CreateWindowW(L"Edit", L"",
                                      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                      20, 50, 400, 25, hwnd, (HMENU)ID_INPUT_TEXT, NULL, NULL);
            SendMessage(hInputBox, WM_SETFONT, (WPARAM)hFont, TRUE);

            hCalcButton = CreateWindowW(L"Button", L"Calculate Average",
                                        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                        150, 90, 150, 35, hwnd, (HMENU)ID_CALC_BUTTON, NULL, NULL);
            SendMessage(hCalcButton, WM_SETFONT, (WPARAM)hFont, TRUE);

            hResultLabel = CreateWindowW(L"Static", L"Average: --",
                                         WS_VISIBLE | WS_CHILD, 20, 150, 400, 30, hwnd, (HMENU)ID_RESULT_LABEL, NULL, NULL);
            SendMessage(hResultLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_CALC_BUTTON) {
                wchar_t buffer[1024];
                GetWindowTextW(hInputBox, buffer, 1024);

                // Convert wchar_t to char
                char input[1024];
                wcstombs(input, buffer, 1024);

                // Parse commas and calculate average
                double sum = 0;
                int count = 0;
                char *token = strtok(input, ", ");
                while (token != NULL) {
                    sum += atof(token);
                    count++;
                    token = strtok(NULL, ", ");
                }

                wchar_t resultText[256];
                if (count > 0) {
                    swprintf(resultText, 256, L"Average: %.4f (from %d numbers)", sum / count, count);
                } else {
                    swprintf(resultText, 256, L"Error: No valid numbers found.");
                }

                SetWindowTextW(hResultLabel, resultText);
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void launch_native_gui(void) {
    // Register the window class
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSW wc = {0};
    
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = L"CalcAvgWindowClass";
    
    if (!RegisterClassW(&wc)) {
        printf("Failed to register GUI window class. Error Code: %lu\n", GetLastError());
        return;
    }

    // Create the window
    HWND hwnd = CreateWindowW(
        wc.lpszClassName,
        L"Calc-Avg V5.5 Graphical User Interface",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 460, 250,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        printf("Failed to create GUI window.\n");
        return;
    }

    printf("\n>>> Launching Native Windows GUI... Close the GUI Window to return to the menu. <<<\n");

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // Unregister so it can be re-launched from CLI loop
    UnregisterClassW(wc.lpszClassName, hInstance);
}
#else
void launch_native_gui(void) {
    printf("The Native GUI feature is currently only supported on Windows systems.\n");
}
#endif
