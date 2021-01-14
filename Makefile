CONFIG = c++11

CC = g++

CPPFLAGS = -g

PROJECT = main

SRC = main.cpp

INCLUDEPATH = 

LIBS = 

HEADERS =

FORMS =

${PROJECT} : ${SRC}
	${CC} ${SRC} -o ${PROJECT} ${LIBS}

clean:
	rm -f *.o main