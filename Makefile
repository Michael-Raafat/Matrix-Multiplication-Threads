HEADERS = matrix_parser.h matrix_calculator.h thread_manager.h file_processing.h
OBJECTS = main.o matrix_parser.o matrix_calculator.o thread_manager.o file_processing.o

default: matmult.out

%.o: %.c $(HEADERS)
	gcc -c $< -o $@ 

matmult.out: $(OBJECTS)
	gcc $(OBJECTS) -o $@  -pthread

clean:
	-rm -f $(OBJECTS)
	-rm -f matmult.out
