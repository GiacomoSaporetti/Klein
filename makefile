SRC = ${wildcard *.cpp}
LIST = ${patsubst %.cpp,%.o,${SRC}}
EXECUTABLE = klein.exe

default: $(LIST)
	@g++ $(LIST) -o $(EXECUTABLE)
	@.\$(EXECUTABLE)

%.o: %.cpp %.h
	@g++ -c $<

clean: 
	@rm -rf *.o