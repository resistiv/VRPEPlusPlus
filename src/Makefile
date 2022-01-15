CC = g++
CFLAGS = -Wall
TARGET = VRPE

$(TARGET): Main.o Extractor.o
	$(CC) $(CFLAGS) -o $(TARGET) Main.o Extractor.o

Main.o: Main.cpp Globals.hpp Extractor.hpp
	$(CC) $(CFLAGS) -c Main.cpp

Extractor.o: Extractor.cpp Extractor.hpp
	$(CC) $(CFLAGS) -c Extractor.cpp