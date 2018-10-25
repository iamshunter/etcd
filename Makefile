all : wrapTest.x 

wrapTest.x : wrapTest.o
	g++ -g -std=gnu++11 -I. -o wrapTest.x wrapTest.o 

wrapTest.o : wrapTest.cpp
	g++ -g -std=gnu++11 -I. -c wrapTest.cpp

clean :
	rm -f wrapTest.x wrapTest.o 
