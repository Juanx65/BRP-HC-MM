CONFIG = c++11

CC = "g++"

PROJECT = main

SRC = main.cpp

INCLUDEPATH = #/usr/local/include/opencv4

LIBS = #`pkg-config opencv4 --cflags --libs`

HEADERS =

FORMS =

${PROJECT} : ${SRC}
	${CC} ${SRC} -o ${PROJECT} ${LIBS}
