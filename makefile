CC=g++ -g -O3
CFLAG= -c  -std=c++17 -I ./lib
GTKL= `pkg-config --libs gtkmm-3.0`
GTKC= `pkg-config --cflags gtkmm-3.0`
TSLIB= -pthread #-lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
TSOB= ./src/obj/trueskill.o ./src/obj/matrix.o src/obj/gaussian.o ./src/obj/basemath.o ./src/obj/factorgraph.o
TSOBN=  $(OBJ)trueskill.o $(OBJ)matrix.o $(OBJ)gaussian.o $(OBJ)basemath.o $(OBJ)factorgraph.o
OBJ= ./src/obj/
 

all: superCuteGrab

superCuteGrab: $(OBJ)superCuteGrab.o  $(OBJ)cskill_instance.o $(OBJ)collectionMan.o $(OBJ)hashDB.o $(OBJ)imageIcons.o  $(OBJ)infoPopup.o $(OBJ)window.o $(OBJ)tagSet.o $(OBJ)tag.o $(OBJ)image.o $(OBJ)thumbDB.o $(OBJ)tagDB.o $(OBJ)filetypes.o $(OBJ)scoreDB.o $(OBJ)fileMd5.o $(OBJ)compMan.o $(OBJ)scalingImage.o $(TSOBN)
	$(CC) ./src/obj/superCuteGrab.o ./src/obj/cskill_instance.o ./src/obj/hashDB.o ./src/obj/collectionMan.o ./src/obj/thumbDB.o ./src/obj/scoreDB.o ./src/obj/imageIcons.o ./src/obj/fileMd5.o ./src/obj/infoPopup.o  ./src/obj/window.o ./src/obj/image.o ./src/obj/tag.o ./src/obj/tagSet.o ./src/obj/tagDB.o ./src/obj/filetypes.o ./src/obj/compMan.o ./src/obj/scalingImage.o -lstdc++fs  -lcrypto -lssl $(TSOB) $(GTKL) $(GTKC)  $(TSLIB)  -o cuteGrab 

$(OBJ)superCuteGrab.o: ./src/superCuteGrab.cpp 
	$(CC) $(CFLAG)  -c ./src/superCuteGrab.cpp -o ./src/obj/superCuteGrab.o  -lcrypto -lssl $(GTKC) $(GTKL)

$(OBJ)infoPopup.o: ./src/graphics/infoPopup.cpp ./src/graphics/infoPopup.h $(OBJ)collectionMan.o $(OBJ)thumbDB.o $(OBJ)image.o $(OBJ)window.o $(OBJ)scalingImage.o $(OBJ)hashDB.o  $(OBJ)imageIcons.o 
	$(CC) $(CFLAG) -c ./src/graphics/infoPopup.cpp -o ./src/obj/infoPopup.o $(GTKC) $(GTKL)

$(OBJ)window.o: ./src/graphics/window.cpp ./src/graphics/window.h $(OBJ)collectionMan.o $(OBJ)thumbDB.o $(OBJ)image.o $(OBJ)scalingImage.o $(OBJ)imageIcons.o 
	$(CC) $(CFLAG) -c ./src/graphics/window.cpp -o ./src/obj/window.o $(GTKC) $(GTKL)

$(OBJ)imageIcons.o: ./src/graphics/imageIcons.cpp ./src/graphics/imageIcons.h $(OBJ)image.o 
	$(CC) $(CFLAG) -c ./src/graphics/imageIcons.cpp -o ./src/obj/imageIcons.o $(GTKC) $(GTKL)

$(OBJ)scalingImage.o: ./src/graphics/scalingImage.cpp ./src/graphics/scalingImage.h $(OBJ)image.o $(OBJ)thumbDB.o
	$(CC) $(CFLAG) -c ./src/graphics/scalingImage.cpp -o ./src/obj/scalingImage.o $(GTKC) $(GTKL)

$(OBJ)collectionMan.o: ./src/collectionMan.cpp ./src/collectionMan.h  $(OBJ)compMan.o 
	$(CC) $(CFLAG) -c ./src/collectionMan.cpp -o ./src/obj/collectionMan.o 

$(OBJ)compMan.o: ./src/compMan.cpp ./src/compMan.h $(OBJ)cskill_instance.o 
	$(CC) $(CFLAG) -c ./src/compMan.cpp -o ./src/obj/compMan.o

$(OBJ)cskill_instance.o : ./src/cskill_instance.cpp ./src/cskill_instance.h $(OBJ)trueskill.o
	$(CC) $(CFLAG) -c ./src/cskill_instance.cpp -o ./src/obj/cskill_instance.o 

$(OBJ)scoreDB.o : ./src/scoreDB.cpp ./src/scoreDB.h $(OBJ)tagSet.o 
	$(CC) $(CFLAG) -c ./src/scoreDB.cpp -o ./src/obj/scoreDB.o -lstdc++fs

$(OBJ)tagDB.o : ./src/tagDB.cpp ./src/tagDB.h $(OBJ)tagSet.o 
	$(CC) $(CFLAG) -c ./src/tagDB.cpp -o ./src/obj/tagDB.o -lstdc++fs 

$(OBJ)tagSet.o: ./src/tagSet.cpp ./src/tagSet.h $(OBJ)tag.o
	$(CC) $(CFLAG) -c ./src/tagSet.cpp -o ./src/obj/tagSet.o 

$(OBJ)tag.o: ./src/tag.cpp ./src/tag.h
	$(CC) $(CFLAG) -c ./src/tag.cpp -o ./src/obj/tag.o

$(OBJ)thumbDB.o: ./src/thumbDB.cpp ./src/thumbDB.h $(OBJ)image.o 
	$(CC) $(CFLAG) -c ./src/thumbDB.cpp -o ./src/obj/thumbDB.o -lstdc++fs $(GTKC) $(GTKL)

$(OBJ)hashDB.o: ./src/hashDB.cpp ./src/hashDB.h $(OBJ)image.o $(OBJ)filetypes.o
	$(CC) $(CFLAG) -c ./src/hashDB.cpp -o ./src/obj/hashDB.o -lstdc++fs  

$(OBJ)image.o: ./src/image.cpp ./src/image.h
	$(CC) $(CFLAG) -c ./src/image.cpp -o ./src/obj/image.o -lstdc++fs

$(OBJ)filetypes.o: ./src/filetypes.cpp ./src/filetypes.h 
	$(CC) $(CFLAG) -c ./src/filetypes.cpp -o ./src/obj/filetypes.o -lstdc++fs

$(OBJ)fileMd5.o:  ./src/fileMd5.cpp ./src/fileMd5.h
	$(CC) $(CFLAG) -c ./src/fileMd5.cpp -o ./src/obj/fileMd5.o -lcrypto -lssl 

$(OBJ)trueskill.o: ./lib/cskill/trueskill.cpp ./lib/cskill/trueskill.h 
	$(CC) $(CFLAG) -c ./lib/cskill/trueskill.cpp -o ./src/obj/trueskill.o -g -I ./src
	
$(OBJ)matrix.o: ./lib/cskill/matrix.cpp ./lib/cskill/matrix.h
	$(CC) $(CFLAG) -c ./lib/cskill/matrix.cpp -o ./src/obj/matrix.o 

$(OBJ)gaussian.o: ./lib/cskill/gaussian.cpp ./lib/cskill/gaussian.h
	$(CC) $(CFLAG) -c ./lib/cskill/gaussian.cpp -o ./src/obj/gaussian.o 

$(OBJ)basemath.o:  ./lib/cskill/basemath.cpp  	
	$(CC) $(CFLAG) -c ./lib/cskill/basemath.cpp -o ./src/obj/basemath.o 
	
$(OBJ)factorgraph.o: ./lib/cskill/factorgraph.cpp ./lib/cskill/factorgraph.h
	$(CC) $(CFLAG) -c ./lib/cskill/factorgraph.cpp -o ./src/obj/factorgraph.o 
	

