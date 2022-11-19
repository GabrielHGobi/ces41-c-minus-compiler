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
│   ├── cminus.l
|   ├── cminus.y
│   ├── globals.h
│   ├── main.c
│   ├── scan.h
│   ├── parse.c
│   ├── parse.h
│   ├── symtab.c
│   ├── symtab.h
│   ├── analyze.c
│   ├── analyze.h
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
C- COMPILATION: examples/invalid_ch.cm
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
`examples/missing_key.cm`

```
C- COMPILATION: examples/missing_key.cm

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
`examples/missing_comma.cm`

```
C- COMPILATION: examples/missing_comma.cm
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
`examples/missing_semicolon.cm`

```
C- COMPILATION: examples/missing_semicolon.cm

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
`examples/missing_parentheses.cm`

```
C- COMPILATION: examples/missing_parentheses.cm

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