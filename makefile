CC=g++
CFLAG= -c  -std=c++17 -I ./lib
GTKL= `pkg-config --libs gtkmm-3.0`
GTKC= `pkg-config --cflags gtkmm-3.0`
TSLIB= -pthread #-lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
TSOB= ./src/obj/trueskill.o ./src/obj/matrix.o src/obj/gaussian.o ./src/obj/basemath.o ./src/obj/factorgraph.o
TSOBN=  trueskill.o matrix.o gaussian.o basemath.o factorgraph.o
 

all: superCuteGrab

superCuteGrab: superCuteGrab.o  hashDB.o window.o image.o thumbDB.o $(TSOBN)
	$(CC) ./src/obj/superCuteGrab.o ./src/obj/hashDB.o ./src/obj/thumbDB.o ./src/obj/fileMd5.o  ./src/obj/window.o ./src/obj/image.o -lstdc++fs  -lcrypto -lssl $(TSOB) $(GTKL) $(GTKC)  $(TSLIB)  -o cuteGrab 

superCuteGrab.o:  
	$(CC) $(CFLAG)  -c ./src/superCuteGrab.cpp -o ./src/obj/superCuteGrab.o  -lcrypto -lssl $(GTKC) $(GTKL)

window.o:  
	$(CC) $(CFLAG) -c ./src/graphics/window.cpp -o ./src/obj/window.o $(GTKC) $(GTKL)

thumbDB.o: image.o 
	$(CC) $(CFLAG) -c ./src/thumbDB.cpp -o ./src/obj/thumbDB.o -lstdc++fs $(GTKC) $(GTKL)

hashDB.o: image.o
	$(CC) $(CFLAG) -c ./src/hashDB.cpp -o ./src/obj/hashDB.o -lstdc++fs  

image.o:
	$(CC) $(CFLAG) -c ./src/image.cpp -o ./src/obj/image.o -lstdc++fs

fileMD5.o:  
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
	

