main: main.o b_tree.o
	g++ -o main main.o b_tree.o

main.o: main.cpp
	g++ -c main.cpp

b_tree.o: b_tree.cpp
	g++ -c b_tree.cpp

all: main

clean:
	rm -f *.o main
