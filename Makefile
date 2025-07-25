CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRC_DIR = src
BUILD_DIR = build
INC_DIR = include

TARGET = $(BUILD_DIR)/folder-watcher

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $^

# Ajout de la dépendance sur le dossier build ici :
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

re: clean all

.PHONY: all clean re
