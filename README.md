# C- Compiler
Compilers (CES41) project.

Prof. Ana Carolina Lorena

### Students

| [<img src="https://avatars.githubusercontent.com/u/67982371?v=4" width="115"><br><sub>Gabriel Gobi</sub>](https://github.com/GabrielHGobi) | [<img src="https://avatars.githubusercontent.com/u/78799492?v=4" width="115"><br><sub>Samuel Vendramini</sub>](https://github.com/samuelv8) | [<img src="https://avatars.githubusercontent.com/u/80128519?v=4" width="115"><br><sub>Thiago Lopes</sub>](https://github.com/TL1981) |
|:-:|:-:|:-:|


## Preparation

### Dependencies

Install `flex` and `bison`. On Debain-based Linux distributions, you can run the following commands:
```
sudo apt-get install flex
sudo apt-get install bison
```

### Directory structure

```
├── src
│   ├── cgen.c
│   ├── cgen.h
│   ├── cminus.l
|   ├── cminus.y
│   ├── code.c
│   ├── code.h
│   ├── globals.h
│   ├── main.c
│   ├── parse.h
│   ├── scan.h
│   ├── symtab.c
│   ├── symtab.h
│   ├── util.c
│   └── util.h
├── examples
├── Makefile
└── README.md
```

Inside `src` we have all source files used in project (some may be generated in compilation time, like `flex yy.c` files).
In `examples` we provide some example programs to execute tests.

 ## Execution

 ### Compile project

Run using provided `Makefile`, with:
 ```
 make element
 ```
Will generate `build` folder and a executable file. For example, to generate the lexical analyzer, one can run:
 ```
 make scanner
 ```
Which generates `cminus_scanner` file.

Some program examples were provided in `examples` folder. After compiling, you can run the element with any valid file with:
 ```
./cminus_element path-to-file
 ```
 For example, to run the scanner to mdc example program, we can execute:
```
./cminus_scanner examples/mdc.cm
```
 ### Other

 To clean project generated files, run:
 ```
 make clean
 ```

## Explanation
### Lexical analyzer - _scanner_

In order to build a lexical analyzer for C- language we used the support of the `flex` tool. 

The core of the lexical analysis is found in `cminus.l` file. In this file, we have the basic rules (regex) for matching strings as: number, identifier, whitespace and newline. There are some reserved words, namely: if, else, int, return, void and while. The symbols supported by the language are treated specially too. For each matching pattern, there is a `TokenType enum` associated with it, which represents the token. Basically, the scan will return the next token (string matching any reserved word, symbol or other pattern) in file, using provided `flex` interface. 

There's a line number tracking while processing tokens (incremented in each "newline" pattern match). For comment processing (`/* comments come like this */`), we implemented a finite state machine to ensure that file will be scanned only after the comment is closed by a `*/`, i.e. everything inside is ignored. In this implementation, if the comment isn't closed, all characters after an comment opening will be considered as a comment, and no error will be raised. If some character or string matches none of the allowed patterns, it will be treated as error, and scan continues.

When running the scanner, the result is printed to standart output (`stdout`). It mainly consists of the program lines (numbered) followed by the tokens and it's lexeme (if not a reserved word or symbol). Next, we provide the scanning result for mdc example program, and for a program with invalid character.

```
C- COMPILATION: examples/mdc.cm
1: /* Um programa para calcular mdc 
2:    Segundo o algoritmo de Euclides*/
3: int gdc (int u, int v)
        3: reserved word: int
        3: ID, name= gdc
        3: (
        3: reserved word: int
        3: ID, name= u
        3: ,
        3: reserved word: int
        3: ID, name= v
        3: )
4: {
        4: {
5:     if (v == 0) return u;
        5: reserved word: if
        5: (
        5: ID, name= v
        5: ==
        5: NUM, val= 0
        5: )
        5: reserved word: return
        5: ID, name= u
        5: ;
6:     else return gdc(v,u-u/v*v);
        6: reserved word: else
        6: reserved word: return
        6: ID, name= gdc
        6: (
        6: ID, name= v
        6: ,
        6: ID, name= u
        6: -
        6: ID, name= u
        6: /
        6: ID, name= v
        6: *
        6: ID, name= v
        6: )
        6: ;
7:     /* u-u / v*v == u mod v */
8: }
        8: }
9: void main(void)
        9: reserved word: void
        9: ID, name= main
        9: (
        9: reserved word: void
        9: )
10: {
        10: {
11:     int x;
        11: reserved word: int
        11: ID, name= x
        11: ;
12:     int y;
        12: reserved word: int
        12: ID, name= y
        12: ;
13:     x = input();
        13: ID, name= x
        13: =
        13: ID, name= input
        13: (
        13: )
        13: ;
14:     y = input();
        14: ID, name= y
        14: =
        14: ID, name= input
        14: (
        14: )
        14: ;
15:     output(gdc(x,y));
        15: ID, name= output
        15: (
        15: ID, name= gdc
        15: (
        15: ID, name= x
        15: ,
        15: ID, name= y
        15: )
        15: )
        15: ;
16: }   16: }
        16: EOF
```

```
C- COMPILATION: examples/lexical_errors/invalid_ch.cm
1: /* Program that uses a invalid character */
2: 
3: void main(void)
        3: reserved word: void
        3: ID, name= main
        3: (
        3: reserved word: void
        3: )
4: {
        4: {
5:     int x@test = 99; /* @ is an illegal character */ 
        5: reserved word: int
        5: ID, name= x
        5: ERROR: @
        5: ID, name= test
        5: =
        5: NUM, val= 99
        5: ;
6:     output(x@test);
        6: ID, name= output
        6: (
        6: ID, name= x
        6: ERROR: @
        6: ID, name= test
        6: )
        6: ;
7: }    7: }
        7: EOF
```

### Syntactic analyzer - _parser_

In order to build a syntactic analyzer for C- language we used the support of the `bison` (YACC) tool. Moreover, this implementation depends on the already explained `flex` scanner (the parser will call scanner function while processing the input).

The core of the syntactic analysis is found in `cminus.y` file. In this file, we have the C- grammar (as provided in the documentation) to generate the syntactic tree. First, we define the tokens that will be used, in order of precedence (the last will have more precedence). Then, we define the grammar rules with the previous tokens. In the right side of the rule, we also do the necessary processing to build the syntactic tree, with our defined structure (found in `globals.h`).

In each node of the tree, we store the node kind (statement or expression). Furthermore, we store the node sub-kind. For statement nodes, it can be and if, while, assingment or return kind. For expression nodes, it can be operator, constant, identifier or function activation kind. If it is an expression node, we also store the type: void, int or boolean. The node stores the lexical data (token type, lexeme etc) too. Finnally, we have the line number and pointers to children and sibling nodes.

That said, the tree is built with the following conventions. First of all, as we already said, we define node type, so we don't have type nodes for identifiers. For function nodes, the first child is function argument list (as siblings nodes), and the second child is the function body, in case of function definition. Expression subtrees are defined as usual, with variables or constants as leaf nodes, and operator as internal nodes, until the root. For array variable nodes, the first child is the array length (in case of definition) or the array index (in case of indexing). For if-else statements, the first child is the condition expression, the second and third children are the if and else bodies respectively (in case of no else statement, the last child is null). While statements are analogous to if statements. Return statements have their child as the return expression (possibly none). Assignment statements have the first child as the variable node and the second as the expression node. Finnally, all sequential program statements (same scope) are siblings in the tree.

The generated tree is printed to standart output (`stdout`), in form of indented text lines, when executing the parser. Next, we provide the parsing result for mdc example program.

***
`examples/mdc.cm`

```
C- COMPILATION: examples/mdc.cm

(...)

Syntax tree:
  Type: int
    Id: gcd
      Type: int
        Id: u
      Type: int
        Id: v
      If
        Op: ==
          Id: v
          Const: 0
        Return
          Id: u
        Return
          Activation: gcd
            Id: v
            Op: -
              Id: u
              Op: *
                Op: /
                  Id: u
                  Id: v
                Id: v
  Type: void
    Id: main
      Type: int
        Id: x
      Type: int
        Id: y
      Assign:
        Id: x
        Activation: input
      Assign:
        Id: y
        Activation: input
      Activation: output
        Activation: gcd
          Id: x
          Id: y
```

### Syntactic errors handling

Next, we provide some results for a set of possible syntactic errors that can occur in the context of compiling C- programs. All the following examples are based on previous correct mdc.cm program for comparative purpose.

***
`examples/syntax_errors/missing_key.cm`

```
C- COMPILATION: examples/syntax_errors/missing_key.cm

(...)

15:     output(gcd(x,y));
        15: ID, name= output
        15: (
        15: ID, name= gcd
        15: (
        15: ID, name= x
        15: ,
        15: ID, name= y
        15: )
        15: )
        15: ;
16:     output(gcd(x,y));
        16: EOF
Syntax error at line 16: syntax error
Current token: EOF

Syntax tree:
```

***
`examples/syntax_errors/missing_comma.cm`

```
C- COMPILATION: examples/syntax_errors/missing_comma.cm
1: /* Um programa para calcular mdc 
2:    Segundo o algoritmo de Euclides*/
3: int gcd (int u int v)
        3: reserved word: int
        3: ID, name= gcd
        3: (
        3: reserved word: int
        3: ID, name= u
        3: reserved word: int
Syntax error at line 3: syntax error
Current token: reserved word: int

Syntax tree:
```

***
`examples/syntax_errors/missing_semicolon.cm`

```
C- COMPILATION: examples/syntax_errors/missing_semicolon.cm

(...)

9: void main(void)
        9: reserved word: void
        9: ID, name= main
        9: (
        9: reserved word: void
        9: )
10: {
        10: {
11:     int x
        11: reserved word: int
        11: ID, name= x
12:     int y;
        12: reserved word: int
Syntax error at line 12: syntax error
Current token: reserved word: int

Syntax tree:
```

***
`examples/syntax_errors/missing_parentheses.cm`

```
C- COMPILATION: examples/syntax_errors/missing_parentheses.cm

1: /* Um programa para calcular mdc 
2:    Segundo o algoritmo de Euclides*/
3: int gcd (int u, int v
        3: reserved word: int
        3: ID, name= gcd
        3: (
        3: reserved word: int
        3: ID, name= u
        3: ,
        3: reserved word: int
        3: ID, name= v
4: {
        4: {
Syntax error at line 4: syntax error
Current token: {

Syntax tree:
```

### Semantic analyzer - _analyzer_

In order to implement the semantic analysis of the code as method `buildSymtab`, we initially built the symbol table through a linked hash table whose lines contain fields: name, scope, ID type, data type and list of lines in which the element appears in the source code.

The table manipulation interface described above was built based on methods:
- `st_insert`  
    ```
    inserts element and its information in the symbol table, if element already exists, just adds line where it was observed again.
    ```
- `st_lookup` 
    ```
    returns the position in the table of the desired element or NULL if it does not exist.
    ```
- `printSymTab` 
    ```
    prints table for visualization purposes.
    ```
It is worth mentioning the use of the `insertNode` method in the pre-order construction of the table, which encapsulates the `st_insert` function and implements the following semantic checks:
- undeclared variable:
`notDeclaredError`
- undeclared function: also
`notDeclaredError`
- variable already declared: `varAlreadyDeclaredError`
- invalid declaration, with name already declared as function:
`funAlreadyDeclaredError`

Next, the existence of the `main` function declaration is checked using the `mainCheck` method, which encapsulates performing a `st_lookup("main")` search.

Then, the other syntactic analysis operations were performed using the `typeCheck` function, which traverses the syntax tree in post-order, performing the semantic check on each node using `checkNode`.

In `checkNode`, semantic checks are carried out for invalid assignments and invalid declarations using void, with specific descriptions of each possibility of error using the `typeError` function, such as:
- `typeError(t->child[0],"if test is not Boolean")`
- `typeError(t, "variable declared void")`

Finally, the construction of the symbol table for the `sort.cm` code is shown below:

```
C- COMPILATION: examples/sort.cm

(...)

Building Symbol Table...

Symbol table:

Variable Name  Scope     ID Type  Data Type  Line Numbers
-------------  --------  -------  ---------  -------------------------
low            sort      var      int        19 21
low            minloc    var      int         4  6  7  8
main                     fun      void       32
sort                     fun      void       19 39
input                    fun      int        36
minloc                   fun      int         4 24
a              sort      array    int        19 24 25 26 27
a              minloc    array    int         4  7 10 11
i              main      var      int        33 34 35 36 37 40 41 42 43
i              sort      var      int        20 21 22 24 26 27 28
i              minloc    var      int         5  8  9 10 11 12 14
k              sort      var      int        20 24 25 26
k              minloc    var      int         5  6 12 16
t              sort      var      int        23 25 27
output                   fun      void       42
x              minloc    var      int         5  7 10 11
x                        array    int         2 36 39 42
high           sort      var      int        19 22 24
high           minloc    var      int         4  9

Checking for main...

Checking types...

Semantic analysis finished
```


### Semantic errors handling

Next, we provide some results for a set of possible semantic errors that can occur in the context of compiling C- programs.

***
`examples/semantic_errors/1_variable_not_declared.cm`

```
C- COMPILATION: examples/semantic_errors/1_variable_not_declared.cm

(...)

Building Symbol Table...
Semantic error at line 4: 'b' was not declared in this scope

Symbol table:

Variable Name  Scope     ID Type  Data Type  Line Numbers
-------------  --------  -------  ---------  -------------------------
input                    fun      int
a              exemplo   var      int         3  4
exemplo                  fun      void        1
output                   fun      void

Checking for main...
Semantic error: undefined reference to 'main'
```
***
`examples/semantic_errors/2_invalid_void_assignment.cm`

```
C- COMPILATION: examples/semantic_errors/2_invalid_void_assignment.cm

(...)

Building Symbol Table...

Symbol table:

Variable Name  Scope     ID Type  Data Type  Line Numbers
-------------  --------  -------  ---------  -------------------------
main                     fun      void        6
input                    fun      int
a              main      var      int         8  9
exemplo                  fun      void        1  9
output                   fun      void        3

Checking for main...

Checking types...
Semantic error at line 9: invalid use of void expression

Semantic analysis finished
```
***
`examples/semantic_errors/3_invalid_decl_void_variable.cm`

```
C- COMPILATION: examples/semantic_errors/3_invalid_decl_void_variable.cm

(...)

Building Symbol Table...

Symbol table:

Variable Name  Scope     ID Type  Data Type  Line Numbers
-------------  --------  -------  ---------  -------------------------
main                     fun      void        1
input                    fun      int
a              main      var      void        3
output                   fun      void

Checking for main...

Checking types...
Semantic error at line 3: variable declared void

Semantic analysis finished
```
***
`examples/semantic_errors/4_variable_already_declared.cm`

```
C- COMPILATION: examples/semantic_errors/4_variable_already_declared.cm

(...)

Building Symbol Table...
Semantic error at line 4: 'a' was already declared as a variable

Symbol table:

Variable Name  Scope     ID Type  Data Type  Line Numbers
-------------  --------  -------  ---------  -------------------------
main                     fun      void        1
input                    fun      int
a              main      var      int         3
output                   fun      void

Checking for main...
```
***
`examples/semantic_errors/5_function_not_declared.cm`

```
C- COMPILATION: examples/semantic_errors/5_function_not_declared.cm

(...)

Building Symbol Table...
Semantic error at line 12: 'fun' was not declared in this scope

Symbol table:

Variable Name  Scope     ID Type  Data Type  Line Numbers
-------------  --------  -------  ---------  -------------------------
main                     fun      void        9
funTwo                   fun      int         5
input                    fun      int
funOne                   fun      int         1
a              main      var      int        11 12
output                   fun      void        2  6

Checking for main...
```
***
`examples/semantic_errors/6_main_undefined.cm`

```
C- COMPILATION: examples/semantic_errors/6_main_undefined.cm

(...)

Building Symbol Table...

Symbol table:

Variable Name  Scope     ID Type  Data Type  Line Numbers
-------------  --------  -------  ---------  -------------------------
funTwo                   fun      int         5
input                    fun      int
funOne                   fun      int         1
output                   fun      void        2  6 10
funThree                 fun      int         9

Checking for main...
Semantic error: undefined reference to 'main'
```
***
`examples/semantic_errors/7_variable_name_used_for_function.cm`

```
C- COMPILATION: examples/semantic_errors/7_variable_name_used_for_function.cm

(...)

Building Symbol Table...
Semantic error at line 14: 'xyz' was already declared as a function

Symbol table:

Variable Name  Scope     ID Type  Data Type  Line Numbers
-------------  --------  -------  ---------  -------------------------
main                     fun      void       11
xyz                      fun      int         5
input                    fun      int
funOne                   fun      int         1
a              main      var      int        13
output                   fun      void        2
x              xyz       var      int         6
y              xyz       var      int         7
z              xyz       var      int         8

Checking for main...
```
### Code generator - _cgen_

Intermediate code generation was implemented through the `codeGen` method, which traverses the syntax tree in pre-order, differentiating the nodes found through the routines:

- `genStmt`: handles `StmK` nodes, which span

   - `DeclK`

      recursive call to second child generator;

   - `IfK`

      call of `genExp` to condition, followed by recursive call to representative nodes of block true and block `else`;

   - `WhileK`

      `genExp` call for condition, followed by recursive call for repeating block;
      
   - `AssignK`

       `genExp` call for the left and right operands;
      

   - `ReturnK`

      `genExp` call to return expression;

- `genExp`: handles `ExpK` nodes, which span

   - `OpK`

      recursive call for left and right operands, followed by final statement with join between recursive operands created with the current node operator;

   - `ConstK`

      value conversion to string;

   - `idK`
      
      Differentiation between variable and array

      - variable:

         name to string conversion;

      - array:
        
         recursive call to child node (index expression), with further formation of array access instruction
         
   - `ActivK`

      recursive call in order to generate instructions for the appropriate parameters, followed by issuing a `call` instruction;
      
Below is an example of how intermediate coding works:

***
`examples/code_gen/array_access_test_code.cm`

```
* C- Compilation to 3 Address Intermediate Code
* File: examples/code_gen/array_access_test_code.cmm

      main:
          i = 0
        L1:
          t1 = i < 10
          if_false t1 goto L2
          t2 = i * 4
          b[t2] = 1
          t3 = i * 4
          c[t3] = 1
          t4 = i + 1
          i = t4
          goto L1
        L2:
        L3:
          t5 = i < 10
          if_false t5 goto L4
          t6 = i * 4
          t7 = i * 4
          t8 = i * 4
          t9 = b[t7] + c[t8]
          a[t6] = t9
          goto L3
        L4:
          halt

* End of execution.

```