# =============================================================================
# (c) Copyright Sanskar. All rights reserved.
# Made by the Sanskar.
# See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
# =============================================================================
# File Purpose: Makefile to build the project.

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lm -lgdi32
SRC = src/main.c src/average.c src/utils.c src/gui.c

all: calc_avg

calc_avg: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o calc_avg $(LDFLAGS)

clean:
	del /Q calc_avg.exe calc_avg 2>nul || rm -f calc_avg.exe calc_avg

run: all
	calc_avg.exe
