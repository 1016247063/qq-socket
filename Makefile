TARGET = hello
HOME=$(PWD)
CC = g++
CFLAGS=--std=c++11
INC = -Icchat -Icepoll -Icsocket -Ithreadpoll
LIBS = -lpthread

SUBDIR = $(shell ls ./ -R | grep /)
SUBDIRS = $(subst :,/,$(SUBDIR))
SOURCE = $(foreach dir, $(SUBDIRS),$(wildcard $(dir)*.o))

OBJS_O = main.o \
	./cchat/cchat_server.o 	\
	./cepoll/cepoll_base.o	\
	./csocket/cipaddr.o	\
	./csocket/cserver_socket.o	\
	./csocket/csocket_base.o	\
	./threadpoll/nthread.o	\
	./threadpoll/task.o	\
	./threadpoll/thread_poll.o	\

all:$(OBJS_O)
	$(CC) -g $(OBJS_O) -o $(TARGET) $(LIBS)

clean:
	rm -rf $(SOURCE)
	rm $(TARGET)

%.o:%.cpp
	$(CC) -g -c $< -o $@ $(INC) $(CFLAGS)