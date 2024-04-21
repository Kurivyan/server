#!/bin/bash
BIN_DIR := bin
SRC_DIR := source
APP_DIR := include
ROUT_DIR := include/routes
BUILD_DIR := build
MON_GOC := /usr/local/include/libmongoc-1.0
MON_LIBS := /usr/local/include/libbson-1.0
OBJ_DIR := $(BUILD_DIR)/object
DEP_DIR := $(BUILD_DIR)/dependency

TARGET := $(BIN_DIR)/server

SRC_EXT := c

SRCS := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OBJS := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(OBJ_DIR)/%.o, $(SRCS))
DEPS := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(DEP_DIR)/%.d, $(SRCS))

CC := gcc
LD := gcc

CFLAGS := -g -Wall -Wextra
APPDIR := -I$(APP_DIR)
ROUTDIR := -I$(ROUT_DIR)
MONGOC := -I$(MON_GOC)
MONLIBS := -I$(MON_LIBS)
DEPFLAGS = -MMD -MT $@ -MF $(DEP_DIR)/$*.d

all: $(TARGET)

#Linking
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(LD) $^ -o $@

#Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT) | $(OBJ_DIR) $(DEP_DIR)
	$(CC) $(CFLAGS) $(APPDIR) $(ROUTDIR) $(MONGOC) $(MONLIBS) -c $< -o $@ $(DEPFLAGS)


$(OBJ_DIR) $(DEP_DIR) $(BIN_DIR):
	@mkdir -p $@

-include $(DEPS)

run: $(TARGET)
	@$<

valrun: $(TARGET)
	@valgrind --leak-check=full --show-leak-kinds=all -s -q $<

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean run valrun