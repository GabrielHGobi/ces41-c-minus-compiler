# Name of the project
PROJ_NAME=cminus

# Project element
LEX=_scanner
SYNT=_parser
ANALYZ=_analyzer
CGEN=_code_gen

# Base directories
BUILD_DIR=./build/
SRC_DIR=./src/

# Flex and Yacc/Bison files
FLEX_FILE=$(wildcard $(SRC_DIR)*.l)
FLEX_C_RESULT=lex.yy.c
BISON_FILE=$(wildcard $(SRC_DIR)*.y)
BISON_C_RESULT=cminus.tab.c
BISON_H_RESULT=cminus.tab.h

# .c files
C_SOURCE=$(wildcard $(SRC_DIR)*.c)
COMMON_C_SOURCE=$(SRC_DIR)main.c $(SRC_DIR)util.c
FLEX_C_SOURCE=$(SRC_DIR)lex.yy.c
BISON_C_SOURCE=$(SRC_DIR)cminus.tab.c
ANALYZER_C_SOURCE=$(SRC_DIR)symtab.c $(SRC_DIR)analyze.c
CODEGEN_C_SOURCE=$(SRC_DIR)code.c $(SRC_DIR)cgen.c

# .h files
BISON_H=$(SRC_DIR)cminus.tab.h

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
all: code_gen	

scanner: build_dir
	$(LA) $(FLEX_FILE)
	@ mv $(FLEX_C_RESULT) $(SRC_DIR)
	$(CC) -c $(COMMON_C_SOURCE) $(FLEX_C_SOURCE) -DNO_PARSE=TRUE
	@ mv *.o -t $(BUILD_DIR)
	$(CC) -o $(PROJ_NAME)$(LEX) $(BUILD_DIR)*.o $(FLAGS)

parser: build_dir
	$(LA) $(FLEX_FILE)
	$(SP) -d $(BISON_FILE) -Wnone
	@ mv $(FLEX_C_RESULT) $(SRC_DIR)
	@ mv $(BISON_C_RESULT) $(SRC_DIR)
	@ mv $(BISON_H_RESULT) $(SRC_DIR)
	$(CC) -c $(COMMON_C_SOURCE) $(FLEX_C_SOURCE) $(BISON_C_SOURCE) -DNO_ANALYZE=TRUE
	@ mv *.o -t $(BUILD_DIR)
	$(CC) -o $(PROJ_NAME)$(SYNT) $(BUILD_DIR)*.o $(FLAGS)	

analyzer: build_dir
	$(LA) $(FLEX_FILE)
	$(SP) -d $(BISON_FILE)
	@ mv $(FLEX_C_RESULT) $(SRC_DIR)
	@ mv $(BISON_C_RESULT) $(SRC_DIR)
	@ mv $(BISON_H_RESULT) $(SRC_DIR)
	$(CC) -c $(COMMON_C_SOURCE) $(FLEX_C_SOURCE) $(BISON_C_SOURCE) $(ANALYZER_C_SOURCE) -DNO_CODE=TRUE
	@ mv *.o -t $(BUILD_DIR)
	$(CC) -o $(PROJ_NAME)$(ANALYZ) $(BUILD_DIR)*.o $(FLAGS)	

code_gen: build_dir
	$(LA) $(FLEX_FILE)
	$(SP) -d $(BISON_FILE)
	@ mv $(FLEX_C_RESULT) $(SRC_DIR)
	@ mv $(BISON_C_RESULT) $(SRC_DIR)
	@ mv $(BISON_H_RESULT) $(SRC_DIR)
	$(CC) -c $(COMMON_C_SOURCE) $(FLEX_C_SOURCE) $(BISON_C_SOURCE) $(ANALYZER_C_SOURCE) $(CODEGEN_C_SOURCE)
	@ mv *.o -t $(BUILD_DIR)
	$(CC) -o $(PROJ_NAME)$(CGEN) $(BUILD_DIR)*.o $(FLAGS)	

clean:
	rm -rf $(BUILD_DIR) $(FLEX_C_SOURCE) $(BISON_C_SOURCE) $(PROJ_NAME)_*

build_dir:
	@ mkdir -p build

.PHONY: all clean
