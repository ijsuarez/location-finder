all:
	g++ -g -std=c++11 location_finder.cpp -o location_finder.bin

clean:
	rm location_finder.bin
