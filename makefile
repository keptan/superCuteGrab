CC=g++
CFLAG= -c  -std=c++17 -I ./lib
GTKL= `pkg-config --libs gtkmm-2.4`
GTKC= `pkg-config --cflags gtkmm-2.4`
TSLIB= -pthread #-lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
TSOB= ./src/obj/trueskill.o ./src/obj/matrix.o src/obj/gaussian.o ./src/obj/basemath.o ./src/obj/factorgraph.o
TSOBN=  trueskill.o matrix.o gaussian.o basemath.o factorgraph.o
 

all: superCuteGrab

superCuteGrab: superCuteGrab.o  hashDB.o  $(TSOBN)
	$(CC) ./src/obj/superCuteGrab.o ./src/obj/hashDB.o ./src/obj/fileMd5.o  -lstdc++fs  -lcrypto -lssl $(TSOB)   $(TSLIB)  -o cuteGrab 

superCuteGrab.o: ./src/superCuteGrab.cpp
	$(CC) $(CFLAG)  -c ./src/superCuteGrab.cpp -o ./src/obj/superCuteGrab.o  -lcrypto -lssl  

hashDB.o: ./src/hashDB.cpp fileMD5.o
	$(CC) $(CFLAG) -c ./src/hashDB.cpp -o ./src/obj/hashDB.o -lstdc++fs  

fileMD5.o: ./src/fileMd5.cpp 
	$(CC) $(CFLAG) -c ./src/fileMd5.cpp -o ./src/obj/fileMd5.o -lcrypto -lssl 

trueskill.o:
	$(CC) $(CFLAG) -c ./lib/cskill/trueskill.cpp -o ./src/obj/trueskill.o -g -I ./src
	
matrix.o:
	$(CC) $(CFLAG) -c ./lib/cskill/matrix.cpp -o ./src/obj/matrix.o 

gaussian.o:
	$(CC) $(CFLAG) -c ./lib/cskill/gaussian.cpp -o ./src/obj/gaussian.o 

	
basemath.o:
	$(CC) $(CFLAG) -c ./lib/cskill/basemath.cpp -o ./src/obj/basemath.o 
	
factorgraph.o:
	$(CC) $(CFLAG) -c ./lib/cskill/factorgraph.cpp -o ./src/obj/factorgraph.o 
	

