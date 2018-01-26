GGJ2018: main.o
	g++ main.o -o GGJ2018 -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp

clean :
	-rm *.o $(objects) GGJ2018
