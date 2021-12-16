TARGET = apasp gen FW

all: apasp gen FW

apasp: apasp.cpp
	g++ -o apasp apasp.cpp

gen: graph_gen.cpp
	g++ -o gen graph_gen.cpp

FW: FloydWarshall.cpp
	g++ -o FW FloydWarshall.cpp

clean:
	rm -f ${TARGET}
	rm -rf results