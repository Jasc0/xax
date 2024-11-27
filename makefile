
xax : xax.h package.o config.o
	g++ -o xax xax.h main.cpp package.o config.o 

package.o : package.h config.h 
	g++ -c package.cpp 

config.o : config.h
	g++ -c config.cpp 

clean:
	rm xax package.o config.o

