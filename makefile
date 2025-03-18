# Compiler and Flags
CC = gcc
CFLAGS = -Wall -pthread -g
LDFLAGS = -lm

# Source and Object Files
SRC = aubatch.c scheduler.c dispatcher.c
OBJ = $(SRC:.c=.o)

# Default target
all: aubatch

# Build the main executable
aubatch: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o aubatch $(LDFLAGS)

# Compile individual source files into object files
%.o: %.c job.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f *.o aubatch
