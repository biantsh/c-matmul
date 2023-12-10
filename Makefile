CC = gcc
CFLAGS = -Wall -g

STANDARD_TARGET = standard
STRASSEN_TARGET = strassen

MAT_UTIL_SRC = mat_util.c
STANDARD_SRC = standard.c
STRASSEN_SRC = strassen.c

OBJ_DIR = obj

MAT_UTIL_OBJ = $(OBJ_DIR)/lib/$(MAT_UTIL_SRC:.c=.o)
STANDARD_OBJ = $(OBJ_DIR)/$(STANDARD_SRC:.c=.o)
STRASSEN_OBJ = $(OBJ_DIR)/$(STRASSEN_SRC:.c=.o)

VPATH = src:src/lib

all: $(STANDARD_TARGET) $(STRASSEN_TARGET)

$(STANDARD_TARGET): $(STANDARD_OBJ) $(MAT_UTIL_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(STRASSEN_TARGET): $(STRASSEN_OBJ) $(MAT_UTIL_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(STANDARD_TARGET) $(STRASSEN_TARGET)
	rm -rf $(OBJ_DIR)

.PHONY: all clean
