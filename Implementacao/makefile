
all: Aresta.o Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o allmst.o dijkstra.o transplex.o transgenetico


Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp -o Aresta.o -O3 -w -Wall -mms-bitfields 
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp -o Vertice.o -O3 -w -Wall -mms-bitfields 

Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp -o Grafo.o -O3 -w -Wall -mms-bitfields 
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c  Conjunto.cpp -o Conjunto.o -O3 -w -Wall -mms-bitfields 
	
kruskal.o : kruskal.cpp kruskal.h
	g++ -g -c kruskal.cpp -o kruskal.o -O3 -w -Wall -mms-bitfields 

Heap.o : Heap.cpp Heap.h
	g++ -g -c Heap.cpp -o Heap.o -O3 -w -Wall -mms-bitfields 

margeSort.o : margeSort.cpp margeSort.h
	g++ -g -c margeSort.cpp -o margeSort.o -O3 -w -Wall -mms-bitfields 

allmst.o : allMST.cpp allMST.h
	g++ -g -c allMST.cpp -o allMST.o -O3 -w -Wall -mms-bitfields 

dijkstra.o : dijkstra.cpp dijkstra.h
	g++ -g -c dijkstra.cpp -o dijkstra.o -O3 -w -Wall -mms-bitfields 

main.o : main.cpp
	g++ -g -c  main.cpp -o main.o -O3 -w -Wall 

transplex.o : transplex.cpp
	g++  -pthread -g -c transplex.cpp -o transplex.o -O3 -w -Wall 
		
transgenetico : main.o  Grafo.o Vertice.o Aresta.o Conjunto.o kruskal.o margeSort.o Heap.o  allmst.o dijkstra.o transplex.o
	g++  -pthread -g  Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o margeSort.o allmst.o dijkstra.o main.o transplex.o -lm -o transgenetico -Llib/ -lgurobi_c++ -lgurobi65 -lm -O3 -w -Wall -mms-bitfields 
	