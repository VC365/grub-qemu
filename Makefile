# Compiler
CC = gcc
   
# "Debug" build
CFLAGS_D = -O0 -g

# "Release" build
CFLAGS_R = -O2 -s -DNDEBUG

# Target executable
TARGET = grub-qemu

# Source files
SRCS = source/main.c

# Appication dir
DIR = /usr/bin/

TEMP = .tmp

# Default rule
all:
	mkdir -p $(TEMP)
	$(CC) $(CFLAGS_R) -o $(TEMP)/$(TARGET) $(SRCS)
	
# Linking the executable
debug:
	mkdir -p $(TEMP)
	$(CC) $(CFLAGS_D) -o $(TEMP)/$(TARGET) $(SRCS)

install:
	install -m 755 $(TEMP)/$(TARGET) $(DIR)

uninstall:
	rm -f $(DIR)/$(TARGET)
	
# Clean up
clean:
	rm -rf $(TEMP)

.PHONY: all clean install uninstall debug

