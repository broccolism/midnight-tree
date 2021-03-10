main: main.o avl_tree.o
	g++ -o main main.o avl_tree.o

main.o: main.cpp
	g++ -c main.cpp

avl_tree.o: avl_tree.cpp
	g++ -c avl_tree.cpp

all: main

clean:
	rm -f *.o main
