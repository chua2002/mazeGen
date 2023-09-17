all: mazeGen

mazeGen: main.cpp
	g++ -o mazeGen main.cpp
	
clean:
	rm -f mazeGen
