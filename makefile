CC=g++
CFLAG= -c  -I./include 

all: superCuteGrab

superCuteGrab: superCuteGrab.o image.o metaData.o
	$(CC) ./src/obj/superCuteGrab.o ./src/obj/image.o ./src/obj/metaData.o -lexiv2 -lstdc++fs -o cuteGrab

superCuteGrab.o: ./src/superCuteGrab.cpp
	$(CC) $(CFLAG)  -c ./src/superCuteGrab.cpp -o ./src/obj/superCuteGrab.o 

metaData.o: ./src/metaData.cpp
	$(CC) $(CFLAG) -c ./src/metaData.cpp -o ./src/obj/metaData.o

image.o: ./src/image.cpp
	$(CC) $(CFLAG)  -c ./src/image.cpp -o ./src/obj/image.o -lexiv2 -lstdc++fs 


