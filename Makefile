CC = g++
CFLAG = -O4 -Wall

all: main

main: main.cpp
						$(CC) $(CFLAG) main.cpp -o main

clean:
		rm -f main *.o*~