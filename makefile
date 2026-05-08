FLAGS         ?= -g -fPIE
export FLAGS
export BASE_DIR  = ${CURDIR}
export INCLUDES  = -I../include/ -I../SDL3/include -I../SDL3/src

SDL3_LIB   = -L${CURDIR}/SDL3 -lSDL3
SDL3_RPATH = -Wl,-rpath,${CURDIR}/SDL3

EXECUTABLE = klein

default: main
	@g++ ${wildcard ./obj/*.o} -o ./$(EXECUTABLE) $(SDL3_LIB) $(SDL3_RPATH)

main:
	@cd ./src && ${MAKE}

clean:
	@cd ./src/ && $(MAKE) clean
	@rm -rf *.o

rebuild: clean default