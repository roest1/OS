
 * Name: Oest, Riley

 * Email: roest1@lsu.edu
  
 * Project: PA-1 (Multithreading)
  
 * Instructor: Feng Chen
 
 * Class: cs4103-sp23
 
 * Login ID: cs410355

---

# Multithreaded Sudoku Validator 

### To compile : 

1. Open the directory `Sudoku-Validator` where **Makefile** and **main.c** are located.

2. Run the `make` command to build the executable file. This will compile **main.c**.


```go
% make
```

This should run:

```go
gcc    -c -o main.o main.c
gcc -Wall -g -pthread -o main main.o
```

3. Run the `./main` command and specify input and output files following it, to execute the application. For example:
   
```go
% ./main ./read_file.txt ./write_file.txt
```

4. To run the `main.o` with the provided `sudoku.txt` and `results.txt` files run `make run`.

```go
$ make run
```

This should run:

```go
gcc    -c -o main.o main.c
gcc -Wall -g -pthread -o main main.o
./main ./sudoku.txt ./results.txt
```

5. Run the `make clean` command to delete the executable files

```go
% make clean
```

This should run:

```go
rm -f main.o main
```

---

1. To submit to `/classes/cs4103/cs410355/prog1`, run `make submit`:

```go
% make submit
```

This should run:

```go
make clean
/classes/cs4103/cs410355/prog1/p_copy -o 1
```

2. To verify the submission, run `make verify`:

This should run:

```go
/classes/cs4103/cs410355/prog1/verify -o 1
```



