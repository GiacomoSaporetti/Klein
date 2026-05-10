SRC     = ${filter-out main_single.cpp main_multiple.cpp, ${wildcard *.cpp}}
OBJECTS = ${patsubst %.cpp,../obj/%.o,${SRC}}

ifeq ($(MODE),MULTIPLE)
    MAIN_SRC = main_multiple.cpp
else
    MAIN_SRC = main_single.cpp
endif

CXXFLAGS = $(FLAGS) $(MODE_FLAG) $(INCLUDES) -std=c++17

default: ${OBJECTS} ../obj/main.o
	@echo
	@echo [KLEIN] SOURCE COMPILED
	@echo

../obj/%.o: %.cpp ../include/%.h
	@echo Compiling $<
	@g++ -c $< ${CXXFLAGS} -o ../obj/${patsubst %.cpp,%.o,$<}

../obj/main.o: $(MAIN_SRC)
	@echo Compiling $(MAIN_SRC)
	@g++ -c $(MAIN_SRC) ${CXXFLAGS} -o ../obj/main.o

clean:
	@cd ../obj && rm -rf *.o