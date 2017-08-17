MODULES = network network_utils population innovation_tracker generation species activation utilities trainer
HDRSRC  = ${MODULES:=.h}
CPPSRC  = ${MODULES:=.cpp} main.cpp
OBJECTS = ${CPPSRC:.cpp=.o}
EXECBIN = smoothiest_executable
CPP 	= g++ -g -O0 -Wall -Wextra -std=gnu++14

all: ${EXECBIN}

run: ${EXECBIN}
	clear
	./${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CPP} -o ${EXECBIN} ${OBJECTS}
	#@rm *.o

${OBJECTS} : ${CPPSRC} ${HDRSRC}
	${CPP} -c ${CPPSRC}
