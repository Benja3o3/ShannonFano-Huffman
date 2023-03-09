#include "hash.h"

hashing::hashing(int largo){
    this->largo = largo;
	root = new nodoHash*[largo];		// tabla root de largo largo
	for (int i = 0; i < largo; i++)
		root[i] = NULL;
}
//Destructor de la clase
hashing::~hashing(){
    nodoHash *t;
    for(int i = 0; i < largo; i++){
        t = root[i];
        if(t != nullptr){
            eliminarNodos(t);
        }            
    }
    delete root;
}

void hashing::eliminarNodos(nodoHash *t){
	if (t != nullptr){
		eliminarNodos(t->next);
        cout << "Eliminado" << endl;
		delete t;
	}
}

//Busca la en la key el puntero que tiene el valor de entrada
nodoHash* hashing::search(int key, vector<bool> val){
	nodoHash *p = root[key];
	while(p != nullptr && p->bitStream != val)
		p = p->next;
	if(p != nullptr)
		return p;
	return nullptr;
}

// inserta un nodoHash con valor val en la lista correspondiente a su clave (root[key]),
// lo inserta a la cabeza de la lista (por la izq.) y retorna el indice de root en donde se insertó (key)
int hashing::insertInT(int key, vector<bool> bs, char caracter){
	nodoHash *nuevo = new nodoHash;
	for (bool i : bs){
		nuevo->bitStream.push_back(i);
	}
	nuevo->simbolo = caracter;
	nuevo->next = root[key];	
	root[key] = nuevo;
	return key;
}