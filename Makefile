CC = g++
SOURCES = animaux.cpp univers.cpp
HEADERS = animaux.hpp univers.hpp
OBJ = animaux.o univers.o main.o
PROG_NAME = univers

all : ${OBJ}
	${CC} ${OBJ} -o ${PROG_NAME}

animaux.o : animaux.cpp animaux.hpp
	${CC} -c animaux.cpp

univers.o : univers.cpp univers.hpp
	${CC} -c univers.cpp

main.o : main.cpp
	${CC} -c main.cpp

clean :
	rm ${OBJ} ${PROG_NAME}
