BIN_DIR = .
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
CC = gcc
CFLAGS = -Wall -I$(INCLUDE_DIR)

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

main: $(MAIN_OBJS) | $(BIN_DIR)
	$(CC) -o $@ $^

monitor: $(MONITOR_OBJS) | $(BIN_DIR)
	$(CC) -o $@ $^

receptionist: $(RECEPTIONIST_OBJS) | $(BIN_DIR)
	$(CC) -o $@ $^

visitor: $(VISITOR_OBJS) | $(BIN_DIR)
	$(CC) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

.PHONY: all clean

clean:
	rm -f $(OBJ_DIR)/*.o main monitor receptionist visitor
