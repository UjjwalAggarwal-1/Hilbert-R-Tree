hilbert : hilbert_exe
	./hilbert_exe

hilbert_exe : 
	gcc -c rtree.c
	gcc -c main.c
	gcc -c hilbert.c
	gcc -o hilbert_exe rtree.o main.o hilbert.o

clean :
	rm -r *.o
	rm hilbert_exe
