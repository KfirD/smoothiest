MODULES = network network_utils population
HDRSRC  = ${MODULES:=.h}
CPPSRC  = ${MODULES:=.cpp} main.cpp
OBJECTS = ${CPPSRC:.cpp=.o}
EXECBIN = smoothiest
CPP 	= g++ -g -O0 -Wall -Wextra -std=gnu++14

all: ${EXECBIN}

run: ${EXECBIN}
	clear
	./smoothiest

${EXECBIN} : ${OBJECTS}
	${CPP} -o ${EXECBIN} ${OBJECTS}
	@rm *.o

${OBJECTS} : ${CPPSRC}
	${CPP} -c ${CPPSRC}
