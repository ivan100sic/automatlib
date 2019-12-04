CC=g++ -g -std=c++11

relacija.o: relacija.cpp relacija.h
	$(CC) relacija.cpp -c -o relacija.o

skup.o: skup.cpp skup.h
	$(CC) skup.cpp -c -o skup.o

sr-cross.o: sr-cross.cpp sr-cross.h
	$(CC) sr-cross.cpp -c -o sr-cross.o

test.o: test.cpp
	$(CC) test.cpp -c -o test.o

nfa.o: nfa.cpp nfa.h
	$(CC) nfa.cpp -c -o nfa.o

ALLO=relacija.o skup.o sr-cross.o nfa.o

test: $(ALLO) test.o
	$(CC) $(ALLO) test.o -o test

clean:
	rm -f relacija.o skup.o sr-cross.o test.o test