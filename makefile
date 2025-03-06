# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread -g

# Executable name
TARGET = aubatch

# Source files
SRCS = aubatch.c scheduler.c dispatcher.c
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Compile the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c job.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
