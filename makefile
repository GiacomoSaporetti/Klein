FLAGS?= 
export FLAGS
export BASE_DIR = ${CURDIR}
SRC = ${wildcard *.cpp}
OBJECTS = ${patsubst %.cpp,./obj/%.o,${SRC}}
export INCLUDES = -I../include/ -I../SDL3/include -I../SDL3/src
EXECUTABLE = klein.exe

default: main
	@g++ ${wildcard ./obj/*.o} -o ./$(EXECUTABLE) 
	@./$(EXECUTABLE)

main:
	@cd ./src && ${MAKE}

clean: 
	@cd ./src/ && $(MAKE) clean
	@rm -rf *.o

rebuild: clean default