# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/process_create_handler.c \
       $(SRC_DIR)/memory_alloc_handler.c \
       $(SRC_DIR)/file_open_handler.c \
       $(SRC_DIR)/user_login_handler.c \
       $(SRC_DIR)/system_boot_handler.c \
       $(SRC_DIR)/utils.c

# Object files
OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/process_create_handler.o \
       $(OBJ_DIR)/memory_alloc_handler.o \
       $(OBJ_DIR)/file_open_handler.o \
       $(OBJ_DIR)/user_login_handler.o \
       $(OBJ_DIR)/system_boot_handler.o \
       $(OBJ_DIR)/utils.o

# Include path
INCLUDES = -I$(INCLUDE_DIR)

# Target executable
TARGET = event_processor

# Build rules
all: $(TARGET)

# Create object directory if not exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Link object files to create the final executable
$(TARGET): $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/event_handler.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJ_DIR)

# Phony targets to avoid conflicts with file names
.PHONY: all clean
