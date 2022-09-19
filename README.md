# ces41-c-minus-compiler
Repositório para projeto de compilador de linguagem C- para a disciplina CES41.

## Preparation
### Dependencies
Install `flex`. On Debain-based Linux distributions, you can run the following command:
```
sudo apt-get install flex
```
### Compile project
Run using provided `Makefile`, with:
 ```
 make
 ```
 Will generate `build` folder and `cminus` executable file.

 ## Execution
 Some program examples were provided in `examples` folder. You can run the scanner with any valid file with:
 ```
./cminus path-to-file
 ```

 ### Other
 To clean project compilation files, run:
 ```
 make clean
 ```
 