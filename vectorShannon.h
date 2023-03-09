#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "hash.h"


using namespace std;

struct nodoShannon{
    char simbolo;
    double probabilidad;
	vector<bool> bitStream;
};
struct posicion{
    int pos_original;
    int pos_codificado;
};

class vectorShannon{
private: 
    //Atributos
	vector<char> texto;
	vector<posicion> posiciones;
    vector<nodoShannon> v_Shannon;
    hashing *root;
    unsigned int totalCaracteres = 0;
    int ascii = 256;
    string filename = "";
    
    
    void cuentaLinea(vector <int> &V, string linea);
    void setV_Shannon(vector<int> V);
    void sortV();

    void shannonFano(int l, int r);
    int partition(vector<nodoShannon> &v, int l, int r);
    void print(string texto);

public:
    vectorShannon(string a);
    
    ~vectorShannon();
    void creaArchivo(string texto, string direccion);
    void imprime(vector<int> &V);
    void imprime(vector<double> &V);
    void print();
    int buscaPosicion(int i, vector<bool> texto_codificado);
    void setRoot();
    int getSize();
    vector<nodoShannon> getV_Shannon();
    bool comparaDouble(double d1, double d2);

    void getTotalCaracteres();
    void setAscii(unsigned ascii);
    unsigned int getAscii();
    vector<bool> codifica(vector<char> texto);
    vector<char> decode_normal(vector<bool> texto, int i, int k);
    vector<char> decode_hashing(vector<bool> texto,int i, int k);
	vector<char> getText();
	void printPos();
	vector<char> decode(vector<bool> texto, int i, int k);
    void liberaMemoria();
};