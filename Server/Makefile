CC=g++
CXXFLAGS=-I. -I.. -I/home/erik/RakNet/Source -I/usr/lib/include -I/usr/include/mysql -I/usr/local/include/mysql++  -L/usr/lib -L/usr/local/lib -Wall -g
CFLAGS=-lsfml-system -lraknet -lmysqlpp -lmysqlclient
OBJS=main.o server.o player.o usermanager.o charactermanager.o ../Common/messages.o

app: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS)  -o Server

clean:
	rm *.o

all: Server
