CC=gcc
CCFLAGS=-Wall -g -pthread

SCRd = /classes/cs4103/cs410355/prog1

OBJ = main.o

INPUT_FILE = ./sudoku.txt
OUTPUT_FILE = ./results.txt

main: $(OBJ)
	$(CC) $(CCFLAGS) -o main $(OBJ)

run: main
	./main $(INPUT_FILE) $(OUTPUT_FILE)


clean:
	- rm -f $(OBJ) main

submit:
	make clean
	$(SCRd)/p_copy -o 1

verify:
	$(SCRd)/verify -o 1
