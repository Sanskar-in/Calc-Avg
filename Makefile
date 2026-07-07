# =============================================================================
# (c) Copyright Sanskar. All rights reserved.
# Made by the Sanskar.
# See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
# =============================================================================
# File Purpose: Makefile to build the project.

CC = gcc
WINDRES = windres
CFLAGS = -Wall -Wextra -Iinclude -Ithird_party/sqlite
LDFLAGS = -lm -lws2_32 -lgdi32
SRC = src/main.c src/utils.c src/average.c src/gui.c src/crypto.c src/server.c src/audio.c third_party/sqlite/sqlite3.c
OBJ = $(SRC:.c=.o)
RES = src/calc_avg.res
EXEC = calc_avg

all: $(EXEC)

$(EXEC): $(OBJ) $(RES)
	$(CC) -o $(EXEC) $(OBJ) $(RES) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.res: %.rc
	$(WINDRES) $< -O coff -o $@

clean:
	del /Q calc_avg.exe calc_avg 2>nul || rm -f calc_avg.exe calc_avg

run: all
	calc_avg.exe
