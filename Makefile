# =============================================================================
# (c) Copyright Sanskar. All rights reserved.
# Made by the Sanskar.
# See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
# =============================================================================
# File Purpose: Makefile to build the project.

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Ithird_party/sqlite
LDFLAGS = -lm -lws2_32 -lgdi32
SRC = src/main.c src/utils.c src/average.c src/gui.c src/crypto.c third_party/sqlite/sqlite3.c
OBJ = $(SRC:.c=.o)
EXEC = calc_avg

all: calc_avg

calc_avg: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o calc_avg $(LDFLAGS)

clean:
	del /Q calc_avg.exe calc_avg 2>nul || rm -f calc_avg.exe calc_avg

run: all
	calc_avg.exe
