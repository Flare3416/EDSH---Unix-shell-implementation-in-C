# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -g

# Files
SRC     = edsh.c command.c
OBJ     = $(SRC:.c=.o)
EXEC    = edsh

# Default target
all: $(EXEC)

# Link object files
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(EXEC) $(OBJ)

# Optional: Run the program
run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
