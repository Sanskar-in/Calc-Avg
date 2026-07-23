// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/sanskarIN/Calc-Avg"
//
// Follow Sanskar or Creator on:-
// GitHub:- "https://www.github.com/sanskarIN"
// YouTube:- "https://youtube.com/@sanskarIN"
// LinkedIn:- "https://www.linkedin.com/in/sanskarIN"
// X (Formally known as Twitter):- "https://www.x.com/Sanskar_in"
// Need support:- supportramsandesh@gmail.com
// Want to make your own projects, apps, and games via Sanskar:- sanskaryadavfrom2012to2026@gmail.com
// =============================================================================

#include "../include/audio.h"
#include "../include/utils.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#include <wincrypt.h>

#define NUM_MIC_BUFFERS 4
#define MIC_BUFFER_SIZE 1600 // 8000Hz * 1 byte * 0.2s = 1600 bytes (5Hz match for Screen Stream)

HWAVEIN hWaveIn = NULL;
WAVEHDR waveHeaders[NUM_MIC_BUFFERS];
char global_mic_b64[8192] = "";
CRITICAL_SECTION mic_cs;

void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    if (uMsg == WIM_DATA) {
        PWAVEHDR pWaveHdr = (PWAVEHDR)dwParam1;
        if (pWaveHdr->dwBytesRecorded > 0) {
            DWORD b64Len = 0;
            CryptBinaryToStringA((BYTE*)pWaveHdr->lpData, pWaveHdr->dwBytesRecorded, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &b64Len);
            if (b64Len < sizeof(global_mic_b64)) {
                EnterCriticalSection(&mic_cs);
                CryptBinaryToStringA((BYTE*)pWaveHdr->lpData, pWaveHdr->dwBytesRecorded, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, global_mic_b64, &b64Len);
                LeaveCriticalSection(&mic_cs);
            }
        }
        waveInAddBuffer(hwi, pWaveHdr, sizeof(WAVEHDR));
    }
}

void start_microphone_stream_thread() {
    InitializeCriticalSection(&mic_cs);
    strcpy(global_mic_b64, "");
    
    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;
    wfx.nSamplesPerSec = 8000;
    wfx.wBitsPerSample = 8;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

    if (waveInOpen(&hWaveIn, WAVE_MAPPER, &wfx, (DWORD_PTR)waveInProc, 0, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
        printf(COLOR_RED "Failed to open microphone hardware for surveillance streaming." COLOR_RESET "\n");
        return;
    }

    for (int i = 0; i < NUM_MIC_BUFFERS; i++) {
        waveHeaders[i].lpData = (LPSTR)malloc(MIC_BUFFER_SIZE);
        waveHeaders[i].dwBufferLength = MIC_BUFFER_SIZE;
        waveHeaders[i].dwBytesRecorded = 0;
        waveHeaders[i].dwUser = 0;
        waveHeaders[i].dwFlags = 0;
        waveHeaders[i].dwLoops = 0;
        waveInPrepareHeader(hWaveIn, &waveHeaders[i], sizeof(WAVEHDR));
        waveInAddBuffer(hWaveIn, &waveHeaders[i], sizeof(WAVEHDR));
    }

    waveInStart(hWaveIn);
    printf(COLOR_GREEN "Live Microphone Surveillance Thread Initialized (8000Hz 8-Bit Mono @ 5Hz)." COLOR_RESET "\n");
}


// Audio Player Global State
HWAVEOUT hWaveOut = NULL;

void init_audio_player() {
    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;
    wfx.nSamplesPerSec = 8000;
    wfx.wBitsPerSample = 8;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;
    
    if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        printf("Failed to open audio output.\n");
    }
}

void play_audio_chunk(const unsigned char* data, int len) {
    if (!hWaveOut) return;
    
    WAVEHDR* wh = (WAVEHDR*)malloc(sizeof(WAVEHDR));
    wh->lpData = (LPSTR)malloc(len);
    memcpy(wh->lpData, data, len);
    wh->dwBufferLength = len;
    wh->dwBytesRecorded = 0;
    wh->dwUser = 0;
    wh->dwFlags = 0;
    wh->dwLoops = 0;
    
    waveOutPrepareHeader(hWaveOut, wh, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, wh, sizeof(WAVEHDR));
}

void cleanup_audio_player() {
    if (hWaveOut) {
        waveOutClose(hWaveOut);
        hWaveOut = NULL;
    }
}

char* get_latest_mic_base64() {
    static char output[8192];
    EnterCriticalSection(&mic_cs);
    strcpy(output, global_mic_b64);
    LeaveCriticalSection(&mic_cs);
    return output;
}

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

void start_microphone_stream_thread() {}

void init_audio_player() {}
void play_audio_chunk(const unsigned char* data, int len) {
    (void)data;
    (void)len;
}
void cleanup_audio_player() {}

char* get_latest_mic_base64() { return ""; }
#endif
