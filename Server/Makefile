CC=g++
CXXFLAGS=-I. -I.. -I/home/erik/RakNet/Source -I/usr/lib/include -L/usr/lib -Wall -g
CFLAGS=-lsfml-system -lraknet
OBJS=main.o server.o player.o

app: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS)  -o Server

all: Server
