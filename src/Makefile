SRC = ${wildcard *.cpp}
OBJECTS = ${patsubst %.cpp,../obj/%.o,${SRC}}


default: ${OBJECTS}
	@echo
	@echo [KLEIN] SOURCE COMPILED
	@echo

../obj/%.o: %.cpp ../include/%.h
	@echo Compiling $< 
	@g++ -c $< ${INCLUDES} -o ../obj/${patsubst %.cpp,%.o,$<} ${FLAGS}

# Per .cpp senza header corrispondente (es. main.cpp)
../obj/main.o: main.cpp
	@echo Compiling $<
	@g++ -c $< ${INCLUDES} -o ../obj/main.o ${FLAGS}

clean:
	@cd ../obj && rm -rf *.o
