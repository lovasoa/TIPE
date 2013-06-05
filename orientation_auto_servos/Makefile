CC=clang
CCOPTS=-g
LIBS=-lm

position_soleil:position_soleil.o
	$(CC) position_soleil.o -o position_soleil $(LIBS)

position_soleil.o:position_soleil.c
	$(CC) -c position_soleil.c -o position_soleil.o $(CCOPTS)
clean:
	rm -rf *.o
