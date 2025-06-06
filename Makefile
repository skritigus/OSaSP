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


objects =  $(OUT_DIR)/emulator.o $(OUT_DIR)/cpu.o $(OUT_DIR)/instruction.o $(OUT_DIR)/parser.o $(OUT_DIR)/file_handler.o $(OUT_DIR)/register.o

prog = $(OUT_DIR)/emulator

all: $(prog) help.txt

$(prog) : $(objects) 
	$(CC) $(CFLAGS) $(objects) -o $@

$(OUT_DIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $^ -o $@

help.txt: generate_help.sh
	./generate_help.sh $(MODE)

.PHONY: clean 
clean:
	@rm -rf $(DEBUG)/* $(RELEASE)/* test
