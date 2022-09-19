# Name of the project
PROJ_NAME=cminus
 
# .c files
C_SOURCE=$(wildcard *.c)
 
# flex files
FLEX_SOURCE=$(wildcard *.l)
FLEX_RESULT=$(wildcard *.yy.*)

# Object files
OBJ=$(C_SOURCE:.c=.o)
 
# Compiler
CC=gcc

# Lexical Analyzer
LA=flex

# Flags
FLAGS= -lfl
     
# Compilation and linking
all:
	$(LA) $(FLEX_SOURCE)
	$(CC) -c $(C_SOURCE) 
	$(CC) -o $(PROJ_NAME) $(OBJ) $(FLAGS)  
 
clean:
	rm -rf $(FLEX_RESULT) $(OBJ) $(PROJ_NAME)
