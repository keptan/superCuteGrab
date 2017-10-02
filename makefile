CC=g++
CFLAG= -c  -I ./lib
GTKL= `pkg-config --libs gtkmm-2.4`
GTKC= `pkg-config --cflags gtkmm-2.4`
TSLIB= -pthread #-lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
TSOB= ./src/obj/trueskill.o ./src/obj/matrix.o src/obj/gaussian.o ./src/obj/basemath.o ./src/obj/factorgraph.o
TSOBN=  trueskill.o matrix.o gaussian.o basemath.o factorgraph.o
 

all: superCuteGrab

superCuteGrab: superCuteGrab.o image.o metaData.o booruInterface.o skillBase.o imageBase.o  searchWindow.o $(TSOBN)
	$(CC) ./src/obj/superCuteGrab.o ./src/obj/image.o ./src/obj/metaData.o ./src/obj/booruInterface.o ./src/obj/skillBase.o ./src/obj/imageBase.o  ./src/obj/searchWindow.o $(TSOB) $(GTKL)  -lcurl -ljsoncpp -lexiv2 -lstdc++fs -lcrypto -lssl $(TSLIB)  -o cuteGrab 

superCuteGrab.o: ./src/superCuteGrab.cpp
	$(CC) $(CFLAG)  -c ./src/superCuteGrab.cpp -o ./src/obj/superCuteGrab.o  -lcrypto -lssl  $(GTKC)

searchWindow.o:
	$(CC) $(CFLAG) -c ./src/graphics/searchWindow.cpp -o ./src/obj/searchWindow.o $(GTKC)

scalingImage.o:
	$(CC) $(CFLAG) -c ./src/graphics/scalingImage.cpp -o ./src/obj/scalingImage.o $(GTKC)



booruInterface.o:
	$(CC) $(CFLAG) -c ./src/booruInterface.cpp -o ./src/obj/booruInterface.o -lcurl -ljsoncpp 

metaData.o: ./src/metaData.cpp
	$(CC) $(CFLAG) -c ./src/metaData.cpp -o ./src/obj/metaData.o -lcrypto -lssl 

image.o: ./src/image.cpp fileMd5.o
	$(CC) $(CFLAG)  -c ./src/image.cpp  -o ./src/obj/image.o -lexiv2 -lstdc++fs -lcrypto -lssl 

skillBase.o: ./src/skillBase.cpp
	$(CC) $(CFLAG) -c ./src/skillBase.cpp -o ./src/obj/skillBase.o

imageBase.o: ./src/imageBase.cpp
	$(CC) $(CFLAG) -c ./src/imageBase.cpp -o ./src/obj/imageBase.o

fileMd5.o:
	$(CC) $(CFLAG)  -c ./src/fileMd5.h -o ./src/obj/fileMd5.o -lcrypto -lssl 
	


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
	

