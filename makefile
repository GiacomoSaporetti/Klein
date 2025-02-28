LIST = main.o TimeHandler.o Entity.o CollisionHandler.o LinkedList.o
EXECUTABLE = klein.exe

default: $(LIST)
	@g++ $(LIST) -o $(EXECUTABLE)
	.\$(EXECUTABLE)

%.o: %.cpp %.h
	@echo $@
	@g++ -c $<

clean: 
	@rm -rf *.o