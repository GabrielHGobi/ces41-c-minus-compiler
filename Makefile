# Name of the project
PROJ_NAME=cminus

# Project element
LEX=_scanner
SYNT=_parser

# Base directories
BUILD_DIR=./build/
SRC_DIR=./src/

# .c files
C_SOURCE=$(wildcard $(SRC_DIR)*.c)
 
# flex files
FLEX_SOURCE=$(wildcard $(SRC_DIR)*.l)
FLEX_RESULT=lex.yy.c

# bison files
BISON_SOURCE=$(wildcard $(SRC_DIR)*.y)
BISON_RESULT_C=cminus.tab.c
BISON_RESULT_H=cminus.tab.h

# Object files
OBJ_FILE=$(wildcard *.o)
OBJ=$(subst .c,.o,$(subst src,build,$(C_SOURCE)))
 
# Compiler
CC=gcc

# Lexical Analyzer
LA=flex

# Syntax Parser
SP=bison

# Flags
FLAGS= -lfl
     
# Compilation and linking
all: parser	

scanner: build_dir
	$(LA) $(FLEX_SOURCE)
	@ mv $(FLEX_RESULT) $(SRC_DIR)
	$(CC) -c $(C_SOURCE) $(SRC_DIR)$(FLEX_RESULT) -DNO_PARSE=TRUE
	@ mv *.o -t $(BUILD_DIR)
	$(CC) -o $(PROJ_NAME)$(LEX) $(BUILD_DIR)*.o $(FLAGS)

parser: build_dir
	$(LA) $(FLEX_SOURCE)
	$(SP) -d $(BISON_SOURCE)
	@ mv $(FLEX_RESULT) $(SRC_DIR)
	@ mv $(BISON_RESULT_C) $(SRC_DIR)
	@ mv $(BISON_RESULT_H) $(SRC_DIR)
	$(CC) -c $(C_SOURCE) $(SRC_DIR)$(FLEX_RESULT) $(SRC_DIR)$(BISON_RESULT_C) $(SRC_DIR)$(BISON_RESULT_H)
	@ mv *.o -t $(BUILD_DIR)
	$(CC) -o $(PROJ_NAME)$(SYNT) $(BUILD_DIR)*.o $(FLAGS)	

clean:
	rm -rf $(BUILD_DIR) $(SRC_DIR)$(FLEX_RESULT) $(PROJ_NAME)_*

build_dir:
	@ mkdir -p build

.PHONY: all clean
