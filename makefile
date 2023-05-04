CC = gcc
CFLAGS = -O1 -Wall -Wextra -std=c11 -I$(INC)

BINDIR = bin
BIN = $(BINDIR)/twin

SRC = src
SRCS = $(wildcard $(SRC)/*.c)

LIB = lib
LIBS = $(wildcard $(LIB)/*.c $(LIB)/*.h)

INC = include

OBJ = obj
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(BINDIR)/* $(OBJ)/*
