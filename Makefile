CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

app: main.o functions.o
	g++ main.o functions.o -o app

main.o: main.cpp header.h
	g++ -c main.cpp

functions.o: functions.cpp header.h
	g++ -c functions.cpp

run: app
	@./app

clean:
	rm -f *.o app