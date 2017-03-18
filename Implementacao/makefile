
all: Aresta.o Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o allmst.o dijkstra.o transplex.o sj

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp

Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c  Conjunto.cpp
	
kruskal.o : kruskal.cpp kruskal.h
	g++ -g -c kruskal.cpp

Heap.o : Heap.cpp Heap.h
	g++ -g -c Heap.cpp

margeSort.o : margeSort.cpp margeSort.h
	g++ -g -c margeSort.cpp

allmst.o : allMST.cpp allMST.h
	g++ -g -c allMST.cpp

dijkstra.o : dijkstra.cpp dijkstra.h
	g++ -g -c dijkstra.cpp

main.o : main.cpp
	g++ -g -c  main.cpp

transplex.o : transplex.cpp
	g++ -g -c transplex.cpp
		
sj : main.o  Grafo.o Vertice.o Aresta.o Conjunto.o kruskal.o margeSort.o Heap.o  allmst.o dijkstra.o transplex.o
	g++ -g  Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o allmst.o dijkstra.o main.o transplex.o -lm -o sj -Llib/ -lgurobi_c++ -lgurobi65 -lm
	