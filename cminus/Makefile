# Name of the project
PROJ_NAME=cminus
 
# .c files
C_SOURCE=$(wildcard *.c)
 
# flex files
FLEX_SOURCE=$(wildcard *.l)
 
# Object files
OBJ=$(C_SOURCE:.c=.o)
 
# Compiler
CC=gcc

# Lexical Analyzer
LA=flex

# Flags
FLAGS=-lfl
     
# Compilation and linking
all: $(PROJ_NAME)
 
$(PROJ_NAME): 
	$(LA) $(FLEX_SOURCE) 
	$(CC) -c $(C_SOURCE) 
	$(CC) -o $(PROJ_NAME) $(OBJ) $(FLAGS) 
 
clean:
	rm -rf $(OBJ) $(PROJ_NAME)  