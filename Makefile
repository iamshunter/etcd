all : wrapTest.x 

wrapTest.x : wrapTest.o
#	g++ -g -std=gnu++11 -I.  -I../yaml/yaml-cpp/include -I../yaml/yaml-cpp/include/yaml-cpp -L./yaml-cpp/build -lyaml-cpp -o wrapTest.x wrapTest.o 
	g++ -g -std=gnu++11 -L../yaml/yaml-cpp/build -o wrapTest.x wrapTest.o -lyaml-cpp 

wrapTest.o : wrapTest.cpp
	g++ -g -std=gnu++11 -I.  -I../yaml/yaml-cpp/include -I../yaml/yaml-cpp/include/yaml-cpp -c wrapTest.cpp

clean :
	rm -f wrapTest.x wrapTest.o 
