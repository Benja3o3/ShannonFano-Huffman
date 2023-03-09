#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <numeric>
#include <map>
#include <math.h>
#include <ctime>

using namespace std;

struct nodo{
    char simbolo;
    double probabilidad;
    nodo *izq;
    nodo *der;
    string bitStream;
    int largo;
};

struct comparador{
    bool operator()(nodo *a, nodo *b){
        return (a->probabilidad > b->probabilidad);
    }
};


class arbolHuffman{
private:
    unsigned int totalCaracteres = 0;
    int ascii = 256;
    string filename = "";
    vector<char> texto;

    priority_queue<nodo *, vector<nodo *>, comparador> colaDePrioridad;
    map<int, vector<nodo *>> largosCodigos;
    map<char, nodo *> dictCodigos;
    vector<int> posiciones;
    nodo* arbol;

    void cuentaLinea(vector<int> &V, string linea);
    void imprime(vector<int> &V);
    void setColaDePrioridad(vector<int> V);
    void getArbolHuffman();

    void asignaBitStream(nodo *r, string bit);
    void obtenerLargosCodigos(nodo *r, int l);
    void creaClavesCanonicas(map<int, vector<nodo *>> const &largosCodigos);
    string toBinary(int n, int largo);

public:
    arbolHuffman(string a);
    ~arbolHuffman();
    
    vector<bool> codifica(vector<char> const &texto);
    vector<char> decodificaHuffman(vector<bool> const &textoCodificado, int rangoInicial, int rangoFinal);
    bool usaFormula(int m,string lado,int N,vector<string> const &codigo,int largoMaximo);
    int buscaSecuencialmente(int N,vector<string>const &codigo,int l, int r,int largoNulo,int largoMaximo);
    int buscaBinariamente(int N,vector<string> const &codigo,int l, int r,int largoNulo,int largoMaximo);
    void imprimeDiccionario(map<int, vector<nodo *>>);
    void printBT(const string &prefix, const nodo *node, bool isLeft);
    void printBT(const nodo *node);

    void getTotalCaracteres();
    vector<char> getText();
    unsigned int getAscii();
    void setAscii(unsigned ascii);
};