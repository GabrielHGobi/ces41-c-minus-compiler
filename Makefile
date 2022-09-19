# Name of the project
PROJ_NAME=cminus

# Project element
LEX=_scanner

# Base directories
BUILD_DIR=./build/
SRC_DIR=./src/

# .c files
C_SOURCE=$(wildcard $(SRC_DIR)*.c)
 
# flex files
FLEX_SOURCE=$(wildcard $(SRC_DIR)*.l)
FLEX_RESULT=lex.yy.c

# Object files
OBJ_FILE=$(wildcard *.o)
OBJ=$(subst .c,.o,$(subst src,build,$(C_SOURCE)))
 
# Compiler
CC=gcc

# Lexical Analyzer
LA=flex

# Flags
FLAGS= -lfl
     
# Compilation and linking
all: scanner	

scanner: build_dir
	$(LA) $(FLEX_SOURCE)
	@ mv $(FLEX_RESULT) $(SRC_DIR)
	$(CC) -c $(C_SOURCE) $(SRC_DIR)$(FLEX_RESULT)
	@ mv *.o -t $(BUILD_DIR)
	$(CC) -o $(PROJ_NAME)$(LEX) $(BUILD_DIR)*.o $(FLAGS)  

clean:
	rm -rf $(BUILD_DIR) $(SRC_DIR)$(FLEX_RESULT) $(PROJ_NAME)_*

build_dir:
	@ mkdir -p build

.PHONY: all clean
