OBJS= main.o sentiment_analysis.o
SOURCE= ./src/main.cpp ./src/sentiment_analysis.cpp
HEADER= ./include/sentiment_analysis.h

all:	$(OBJS)
	g++ -o recommendation $(OBJS)

main.o:	./src/main.cpp
	g++ -c ./src/main.cpp		

sentiment_analysis.o:	./src/sentiment_analysis.cpp
	g++ -c ./src/sentiment_analysis.cpp

clean:
	rm -rf *.o recommendation	