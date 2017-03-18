#ifndef _HEAP_H_
#define _HEAP_H_

class Heap {
	private:
		int size; /*tamanho atual (parcial) da heap*/
		float *heap; //Guarda as chaves a serem ordenadas. Vetor de floats
		int *auxHeap; // Guarda os id das arestas
		//int *auxID; 
		int maximo;
	
	public: 
		Heap(int maximo1);
		~Heap();
	private:
		void subir(int i);
		void descer(int i);
	public:
		bool insert (int id, float chave);
	
	
	/*ESTA FUNÇÃO NÃO RETORNA OS ELEMENTOS MIM. ELA SUMPÕE QUE OS MESMOS FORAM TOMANDOS IMEDIATAMENTE ANTES DA CHAMADA DA FUNÇÃO*/
	 	bool extract (); /*pega o último elemento e coloca no lugar do primeiro, depois desce ele.*/
			
	
		float getChave();
		int getId();
		int getSize();
		void desaloca();

		//bool update(int id, float novaChave); /// new
};
#endif