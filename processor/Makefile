# Makefile for building and running myprogram from main.c and helper.c

# Compiler
CC = gcc

# Output executable
TARGET = mips

# Source files
SOURCES = mips.c mips_helper.c reliability_logging.c mips_reliability.c

# Default build rule
all: $(TARGET)

# Compile and link the program
$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) -lm

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up compiled files
clean:
	rm -f $(TARGET) *.o
