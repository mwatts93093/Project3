# Compiler and Flags
CC = gcc
CFLAGS = -Wall -pthread -g
LDFLAGS = -lm

# Source and Object Files
SRC = aubatch.c scheduler.c dispatcher.c evaluation.c
OBJ = aubatch.o scheduler.o dispatcher.o evaluation.o

# Default target
all: aubatch batch_job

# Build the main executable
aubatch: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o aubatch $(LDFLAGS)

# Compile batch_job separately
batch_job: batch_job.o
	$(CC) $(CFLAGS) batch_job.o -o batch_job $(LDFLAGS)

# Compile individual source files into object files
%.o: %.c job.h
	$(CC) $(CFLAGS) -c $< -o $@

# Exclude job.h dependency for batch_job.o
batch_job.o: batch_job.c
	$(CC) $(CFLAGS) -c batch_job.c -o batch_job.o

# Clean compiled files
clean:
	rm -f *.o aubatch batch_job
