CC = gcc
CFLAGS = -std=c11 -g2 -ggdb -pedantic -W -Wall -Wextra

.SUFFIXES:   
.SUFFIXES: .c .o

DEBUG   = ./build/debug
RELEASE = ./build/release
OUT_DIR = $(DEBUG)
vpath %.c src
vpath %.h src
vpath %.o build/debug

ifeq ($(MODE), release)
  CFLAGS = -std=c11 -pedantic -W -Wall -Wextra -Werror
  OUT_DIR = $(RELEASE)
  vpath %.o build/release
endif


objects =  $(OUT_DIR)/main.o $(OUT_DIR)/cpu.o $(OUT_DIR)/instructions.o $(OUT_DIR)/cli.o $(OUT_DIR)/registers.o $(OUT_DIR)/operations.o $(OUT_DIR)/file.o

prog = $(OUT_DIR)/curse

all: $(prog) 

$(prog) : $(objects) 
	$(CC) $(CFLAGS) $(objects) -o $@

$(OUT_DIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $^ -o $@

.PHONY: clean 
clean:
	@rm -rf $(DEBUG)/* $(RELEASE)/* test
