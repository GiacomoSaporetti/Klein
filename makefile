LIST = main.o TimeHandler.o Entity.o CollisionHandler.o LinkedList.o

default: $(LIST)
	@g++ $(LIST) -o main.exe
	.\main.exe

%.o: %.cpp %.h
	@echo $@
	@g++ -c $<

clean: 
	rm -rf *.o