SRC = ${wildcard *.cpp}
OBJECTS = ${patsubst %.cpp,../obj/%.o,${SRC}}
INCLUDES = ../include/

default: ${OBJECTS}
	@echo
	@echo [KLEIN] SOURCE COMPILED
	@echo

../obj/%.o: %.cpp ../include/%.h
	@echo Compiling $< 
	@g++ -c $< -I${INCLUDES} -o ../obj/${patsubst %.cpp,%.o,$<}

clean:
	@cd ../obj && rm -rf *.o
