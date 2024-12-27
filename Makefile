BIN_DIR = .
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
CC = gcc
CFLAGS = -Wall -I$(INCLUDE_DIR)
SHARED_MEMORY_NAME= "barInfo_sdi2200171"

MAIN_FILES = main.c monitor.c receptionist.c visitor.c
COMMON_SRCS = $(filter-out $(addprefix $(SRC_DIR)/, $(MAIN_FILES)), $(wildcard $(SRC_DIR)/*.c))

MONITOR_SRCS = $(SRC_DIR)/monitor.c $(COMMON_SRCS)
RECEPTIONIST_SRCS = $(SRC_DIR)/receptionist.c $(COMMON_SRCS)
VISITOR_SRCS = $(SRC_DIR)/visitor.c $(COMMON_SRCS)
MAIN_SRCS = $(SRC_DIR)/main.c $(COMMON_SRCS)

MONITOR_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(MONITOR_SRCS))
RECEPTIONIST_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(RECEPTIONIST_SRCS))
VISITOR_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(VISITOR_SRCS))
MAIN_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(MAIN_SRCS))

all: main monitor receptionist visitor

# Main target with SHARED_MEMORY_NAME macro passed to the compiler
main: $(MAIN_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Other targets without SHARED_MEMORY_NAME macro
monitor: $(MONITOR_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

receptionist: $(RECEPTIONIST_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

visitor: $(VISITOR_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the object directory if it does not exist
$(OBJ_DIR):
	mkdir -p $@

.PHONY: all clean

# Clean up all build files
clean:
	rm -f $(OBJ_DIR)/*.o main monitor receptionist visitor

# Runtime commands
rmain: all 
	./main

valgrind_main: all
	valgrind --leak-check=full --trace-children=yes ./main

rvisitor: all 
	./visitor -d 10 -s "${SHARED_MEMORY_NAME}"

rrecep: all 
	./receptionist -d 2 -s "${SHARED_MEMORY_NAME}"

rmonitor: all 
	./monitor -s "${SHARED_MEMORY_NAME}"
