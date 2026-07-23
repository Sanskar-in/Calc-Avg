# =============================================================================
# (c) Copyright Sanskar. All rights reserved.
# Made by the Sanskar.
# See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/sanskarIN/Calc-Avg"
# =============================================================================
# File Purpose: Makefile to build the project.

CC = gcc
WINDRES = windres
CFLAGS = -Wall -Wextra -Iinclude -Ithird_party/sqlite
SRC = src/main.c src/utils.c src/average.c src/gui.c src/crypto.c src/server.c src/audio.c src/neural_net.c src/surveillance.c third_party/sqlite/sqlite3.c
OBJ = $(SRC:.c=.o)
ifeq ($(OS),Windows_NT)
    LDFLAGS = -lm -lws2_32 -lgdi32 -luser32 -lcrypt32 -lwinmm
    RES = src/calc_avg.res
    EXEC = calc_avg.exe
    CLEAN_CMD = del /Q src\\*.o third_party\\sqlite\\*.o calc_avg.exe src\\calc_avg.res 2>nul || rm -f src/*.o third_party/sqlite/*.o calc_avg.exe src/calc_avg.res
    OBJ_ALL = $(OBJ) $(RES)
else
    LDFLAGS = -lm -pthread
    EXEC = calc_avg
    CLEAN_CMD = rm -f src/*.o third_party/sqlite/*.o calc_avg src/calc_avg.res
    OBJ_ALL = $(OBJ)
endif

all: $(EXEC)

$(EXEC): $(OBJ_ALL)
	$(CC) -o $(EXEC) $(OBJ_ALL) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.res: %.rc
	$(WINDRES) $< -O coff -o $@

clean:
	$(CLEAN_CMD)

run: all
	calc_avg.exe
