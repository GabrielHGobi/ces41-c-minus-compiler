
flex tiny.l
bison -d tiny.y
gcc -c *.c
gcc -o tiny *.o -ly -lfl
(ou -ll se MACOS)

Para usar a Tiny Machine (TM):
./TM <arquivo>.tm
Enter command: go para executar o programa
Para sair quit
