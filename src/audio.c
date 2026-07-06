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

#include "../include/audio.h"
#include "../include/utils.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

void play_data_sonification(double data_points[], int count) {
    if (count == 0) {
        printf(COLOR_RED "Error: No data to sonify.\n" COLOR_RESET);
        return;
    }

    printf("\n" COLOR_CYAN COLOR_BOLD "--- Audio: Hardware Data Sonification Engine ---" COLOR_RESET "\n\n");
    printf(COLOR_YELLOW "Synthesizing %d data points into sound waves...\n" COLOR_RESET, count);

    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += data_points[i];
        
        // Map data to a frequency between 200Hz and 2000Hz for pleasant listening
        // Base frequency 300Hz, scale by data point.
        DWORD freq = 300 + (DWORD)(data_points[i] * 10);
        if (freq < 37) freq = 37; // Beep requires min 37Hz
        if (freq > 32767) freq = 32767; // Max frequency
        
        printf("Playing Data Point %d (%10.2f) -> %lu Hz\n", i + 1, data_points[i], freq);
        Beep(freq, 200); // Play for 200ms
        Sleep(50); // Pause between notes
    }

    double average = sum / count;
    
    // Grand Finale Beep for the Average
    DWORD avg_freq = 300 + (DWORD)(average * 10);
    if (avg_freq < 37) avg_freq = 37;
    if (avg_freq > 32767) avg_freq = 32767;

    printf(COLOR_GREEN "\nData Sonification Complete!" COLOR_RESET "\n");
    printf(COLOR_YELLOW "Playing Final Mathematical Average (%.2f) -> %lu Hz (Long Note)\n" COLOR_RESET, average, avg_freq);
    
    // Play the final average for 1.5 seconds
    Beep(avg_freq, 1500);
    printf("\n");
}
#else
void play_data_sonification(double data_points[], int count) {
    (void)data_points;
    (void)count;
    printf(COLOR_RED "Error: Hardware Audio Sonification is only supported on Windows OS via the Beep() kernel API.\n" COLOR_RESET);
}
#endif
