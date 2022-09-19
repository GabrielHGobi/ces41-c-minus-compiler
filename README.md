# C- Compiler
Compilers (CES41) project.

Prof. Ana Carolina Lorena

### Students:
| [<img src="https://avatars.githubusercontent.com/u/67982371?v=4" width="115"><br><sub>Gabriel Gobi</sub>](https://github.com/GabrielHGobi) | [<img src="https://avatars.githubusercontent.com/u/78799492?v=4" width="115"><br><sub>Samuel Vendramini</sub>](https://github.com/samuelv8) | [<img src="https://avatars.githubusercontent.com/u/80128519?v=4" width="115"><br><sub>Tiago Lopes</sub>](https://github.com/TL1981) |
|:-:|:-:|:-:|


## Preparation
### Dependencies
Install `flex`. On Debain-based Linux distributions, you can run the following command:
```
sudo apt-get install flex
```

### Directory structure

```
├── src
│   ├── cminus.l
│   ├── globals.h
│   ├── main.c
│   ├── scan.h
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
 make lexical
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

When running the scanner, the result is printed to standart output (`stdout`). It mainly consists of the program lines (numbered) followed by the tokens and it's lexeme (if not a reserved word or symbol). Next, we provide the scanning result for mdc example program.

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
