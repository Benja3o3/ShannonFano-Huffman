#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

struct nodeList {
	vector<bool> bitStream;
    char simbolo;
	nodeList* next;
};
typedef struct nodeList nodoHash;

class hashing{
    private: 
        nodoHash **root;
        int largo;
    public:

        hashing(int largo);
        ~hashing();

        nodoHash* search(int key, vector<bool> val);

        int insertInT(int key, vector<bool> bs, char caracter);
        int removeFromT(int m, int val);
        void print(int m);
        void eliminarNodos(nodoHash *t);
};
