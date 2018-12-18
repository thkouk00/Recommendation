OBJS= main.o
SOURCE= ./src/main.cpp

all:	$(OBJS)
	g++ -o recommendation $(OBJS)

main.o:	./src/main.cpp
	g++ -c ./src/main.cpp		


clean:
	rm -rf *.o recommendation	