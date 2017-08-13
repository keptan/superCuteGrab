CC=g++
CFLAG= -c  -I./include 
TSLIB= -pthread #-lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
TSOB= ./src/obj/mathexpr.o ./src/obj/trueskill.o ./src/obj/ndtri.o src/obj/pdf.o ./src/obj/ndtr.o ./src/obj/const.o
TSOBN= mathexpr.o trueskill.o ndtri.o pdf.o ndtr.o const.o

all: superCuteGrab

superCuteGrab: superCuteGrab.o image.o metaData.o booruInterface.o  $(TSOBN)
	$(CC) ./src/obj/superCuteGrab.o ./src/obj/image.o ./src/obj/metaData.o ./src/obj/booruInterface.o $(TSOB) -lcurl -ljsoncpp -lexiv2 -lstdc++fs -lcrypto -lssl $(TSLIB)  -o cuteGrab

superCuteGrab.o: ./src/superCuteGrab.cpp
	$(CC) $(CFLAG)  -c ./src/superCuteGrab.cpp -o ./src/obj/superCuteGrab.o  -lcrypto -lssl

booruInterface.o:
	$(CC) $(CFLAG) -c ./src/booruInterface.cpp -o ./src/obj/booruInterface.o -lcurl -ljsoncpp

metaData.o: ./src/metaData.cpp
	$(CC) $(CFLAG) -c ./src/metaData.cpp -o ./src/obj/metaData.o -lcrypto -lssl

image.o: ./src/image.cpp fileMd5.o
	$(CC) $(CFLAG)  -c ./src/image.cpp  -o ./src/obj/image.o -lexiv2 -lstdc++fs -lcrypto -lssl

fileMd5.o:
	$(CC) $(CFLAG)  -c ./src/fileMd5.h -o ./src/obj/fileMd5.o -lcrypto -lssl
	

	
trueskill.o:
	$(CC) $(CFLAG) -c ./src/trueskill/trueskill.cpp -o ./src/obj/trueskill.o
	
mathexpr.o:
	$(CC) $(CFLAG) -c ./src/trueskill/mathexpr.cpp -o ./src/obj/mathexpr.o

ndtri.o:
	$(CC) $(CFLAG) -c ./src/trueskill/ndtri.cpp -o ./src/obj/ndtri.o
	
pdf.o:
	$(CC) $(CFLAG) -c ./src/trueskill/pdf.cpp -o ./src/obj/pdf.o
	
ndtr.o:
	$(CC) $(CFLAG) -c ./src/trueskill/ndtr.cpp -o ./src/obj/ndtr.o
	
const.o:
	$(CC) $(CFLAG) -c ./src/trueskill/const.cpp -o ./src/obj/const.o	
