

#include "Heap.h"
#include "margeSort.h"
#include <iostream>

using namespace std;

 Heap::Heap(int maximo1){ 
	maximo = maximo1;
	heap = new float[maximo];
	auxHeap = new int[maximo];
	//auxID = new int[maximo];		
	// Lembrar: A posição 0 do heap é ociosa
	for (int i=0; i<maximo; i++){
		//auxID[i] = 0;
		auxHeap[i] = -1;
	}
	size = 0;
}

Heap::~Heap(){
	delete[] auxHeap;
	delete[] heap;
}

void  Heap::subir(int i){ 
	int pai = i/2;
	if (pai>0){ /*vetor com size+1 posições*/
		if (maiorQuefloat(heap[pai],heap[i])==true){
			float aux1 = heap[i];
			heap[i] = heap[pai];
			heap[pai] = aux1;
				
			int aux = auxHeap[i];
			auxHeap[i] = auxHeap[pai];
			auxHeap[pai] = aux;
					
			subir(pai);
		}				
	}
	//if (auxHeap[i]>=0) auxID[auxHeap[i]] = i;
}
void  Heap::descer(int i){
		int filho = i*2;
		if (filho<=size){
			if (filho!=size){ 
				if (maiorIgualQuefloat(heap[filho+1],heap[filho])==false) filho++;
			}
			if (maiorQuefloat(heap[i],heap[filho])==true){
				float aux1 = heap[i];
				heap[i] = heap[filho];
				heap[filho] = aux1;
				
				int aux = auxHeap[i];
				auxHeap[i] = auxHeap[filho];
				auxHeap[filho] = aux;
			
				descer(filho);
			}
		}
		//if (auxHeap[i]>=0)auxID[auxHeap[i]] = i;
	}




bool  Heap::insert (int id, float chave){ /*insere no final e sobe o elemento*/
	//if (size+1<maximo && id<maximo && auxID[id]<maximo && auxHeap[auxID[id]]!=id){
	if (size+1<maximo ){
		heap[size+1]=chave;
		auxHeap[size+1] = id;
		size++;
		subir(size);
		return true;
	} else return false;
}
	
/*ESTA FUNÇÃO NÃO RETORNA OS ELEMENTOS MIM. ELA SUMPÕE QUE OS MESMOS FORAM TOMANDOS IMEDIATAMENTE ANTES DA CHAMADA DA FUNÇÃO*/
bool  Heap::extract (){ /*pega o último elemento e coloca no lugar do primeiro, depois desce ele.*/
	if (size>0){
		heap[1] = heap[size];
		//int id = auxHeap[1];
		auxHeap[1] = auxHeap[size];
		descer(1);
		size--;
		//auxID[id] = 0;
		return true;
	} else return false;
}
	
float  Heap::getChave(){
	return heap[1];
}

int  Heap::getId(){
	return auxHeap[1];
}

int  Heap::getSize(){
	return size;
}

// void  Heap::desaloca(){
// 	delete[] auxHeap;
// 	delete[] heap;
// 	//delete[] auxID; 
// }