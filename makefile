CC=g++
CFLAG= -c  -I./include 

all: superCuteGrab

superCuteGrab: superCuteGrab.o image.o metaData.o booruInterface.o
	$(CC) ./src/obj/superCuteGrab.o ./src/obj/image.o ./src/obj/metaData.o ./src/obj/booruInterface.o -lcurl -ljsoncpp -lexiv2 -lstdc++fs -lcrypto -lssl  -o cuteGrab

superCuteGrab.o: ./src/superCuteGrab.cpp
	$(CC) $(CFLAG)  -c ./src/superCuteGrab.cpp -o ./src/obj/superCuteGrab.o  -lcrypto -lssl

booruInterface.o:
	$(CC) $(CFLAG) -c ./src/booruInterface.cpp -o ./src/obj/booruInterface.o -lcurl -ljsoncpp

metaData.o: ./src/metaData.cpp
	$(CC) $(CFLAG) -c ./src/metaData.cpp -o ./src/obj/metaData.o -lcrypto -lssl

image.o: ./src/image.cpp fileMd5.o
	$(CC) $(CFLAG)  -c ./src/image.cpp  -o ./src/obj/image.o -lexiv2 -lstdc++fs -lcrypto -lssl

fileMd5.o:
	$(CC) $(CFLAG) -c ./src/fileMd5.h -o ./src/obj/fileMd5.o -lcrypto -lssl

