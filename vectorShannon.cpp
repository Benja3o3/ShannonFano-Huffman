#include "vectorShannon.h"
#include <ctime>
#include <cmath>

//Funcion que genera un "vector shannon" con todos sus componentes a partir de la ruta del archivo
vectorShannon::vectorShannon(string filename){  
	string linea;
	this->filename = filename;
	setAscii(256);
	vector <int> contadores(ascii);
	ifstream archivo(filename.c_str());
	while (getline(archivo, linea)) {
		cuentaLinea(contadores, linea);
		for (char i : linea){
			if (int(i) > 0){
				this->texto.push_back(i);
			}	
		}
		contadores[int('\n')]++;
		this->texto.push_back('\n');
		totalCaracteres++;
	}
	//Comienza a generar la estructura de vector con sus probabilidades
	setV_Shannon(contadores); //Crea el vector
	sortV(); //Ordena de mayor probabilidad a menor probabilidad
	shannonFano(0,int(v_Shannon.size())-1);//Asignacion de los bitstream
	setRoot();//Crea la raiz del hashing
}
//Getter de text
vector<char> vectorShannon::getText(){
	return this->texto;
}
//Destructor de la clase
vectorShannon::~vectorShannon(){ 
    texto.clear();
    posiciones.clear();
	v_Shannon.clear();
}
int vectorShannon::getSize(){
	return int(v_Shannon.size());
}

//Construye el vector Shannon con sus nodos y componentes
void vectorShannon::setV_Shannon(vector<int> V){
	double p;
	for (unsigned int i = 0; i < V.size(); i++){
		p = double(V[i])/double(totalCaracteres);
		if(p > 0){
			nodoShannon nodo;
			nodo.simbolo = char(i);
			nodo.probabilidad = p;
			nodo.bitStream = {};
			v_Shannon.push_back(nodo);
		}
	}
}
//Construye la raiz del hashing
void vectorShannon::setRoot(){
	root = new hashing(v_Shannon.back().bitStream.size() + 1);
	for (int i = v_Shannon.size() - 1; i >= 0; i--){
		root->insertInT(v_Shannon[i].bitStream.size(), v_Shannon[i].bitStream, v_Shannon[i].simbolo);
	}
}
//Recorre una linea del texto,  cuenta sus caracteres y los agrega al vector de contadores 
void vectorShannon::cuentaLinea(vector <int> &V, string linea){
  for(char i : linea){
		if(int(i) < ascii && int(i) > 0){
			totalCaracteres++;
			V[int(i)]++;
		}
	}
}

void vectorShannon::print(string texto){
	for (char caracter : texto){
		cout << caracter << endl;
	}
	
}
//Imprime los nodos del Vector Shannon
void vectorShannon::print(){
	if(int(v_Shannon.size()) < 1){
		cout << "No se puede imprimir" << endl;
		return;
	}
	for(nodoShannon nodo : v_Shannon){
		cout <<"Simbolo: " << "(" << nodo.simbolo << ") " <<  "Probabilidad: " << nodo.probabilidad << " BS: ";
		for (bool i : nodo.bitStream){
			if (i == true) cout << "1";
			else cout << "0";
		} 
		cout << " ";
		cout << "SizeBS: " << nodo.bitStream.size() << endl;
	}
}
//Permite elegir entre el ascii extendido y el normal
void vectorShannon::setAscii(unsigned int ascii){
	if(ascii > 125) this->ascii = 256;
	else this->ascii = 126;
}
//Retorna el Ascii
unsigned int vectorShannon::getAscii(){
	return ascii;
}
//Retorna el Vector Shannon 
vector<nodoShannon> vectorShannon::getV_Shannon(){
	return v_Shannon;
}
//Ordena el Vector Shannon según sus probabilidades
void vectorShannon::sortV(){
	 sort(v_Shannon.begin(), v_Shannon.end(), [](nodoShannon a, nodoShannon b){ //si funciona
		return a.probabilidad > b.probabilidad;	
	});
}

//Funcion que Asigna los bitstream con el algoritmo de Shannon Fano
void vectorShannon::shannonFano(int l, int r){
	int p;
	if(l < r){
		p = partition(v_Shannon, l, r);
		shannonFano(l,p - 1);
		shannonFano(p, r);
	}
}

//Funcion que devuelve el pivote de la particion, quedando lo mas equilibrado 
//respecto a las posiciones
int vectorShannon::partition(vector<nodoShannon> &v, int l, int r){
	double pmax = 0.0;
	//Se calcula la probabilidad maxima de la particion
	for (int x = l; x <= r ; x++){
		pmax += v[x].probabilidad;
	}
	int i = l; //left pos 
	int j = r; //right pos
	double izq = v[i].probabilidad;
	double der = v[j].probabilidad;
	//Se va sumando de hacia el centro para que quede una particion equilibrada
	while(!comparaDouble(izq+der,pmax)){
		if (der <= izq){
			j--;
			der += v[j].probabilidad;
		}
		else{
			i++;
			izq += v[i].probabilidad;
			
		}
	}
	
	//Asigna "1"(true) a los elementos del lado izquierdo 
	//y "0"(false) a los de la derecha	
	for(int x = l; x < j; x++){
		v[x].bitStream.push_back(true);
	}
	for(int x = j; x <= r; x++){
		v[x].bitStream.push_back(false);
	}
	return j;
}
//Funcion usada para comparar doubles con exactitud de e^-9
bool vectorShannon::comparaDouble(double d1, double d2){
	return fabs(d1 - d2) < 0.000000001;
}
//Funcion que codifica el texto, y guarda posiciones del texto original y su posicion en el texto codificado en el vector de posiciones
vector<bool> vectorShannon::codifica(vector<char> texto){
	time_t start = clock();
	vector<bool> texto_codificado; 
	int pos = 0, pos_textocodificado = 0, pos_texto = 0;
	//Se asigna la cantidad y el tamaño de las particiones
	int cantidad_particiones = texto.size()*0.1, tam_particion = texto.size() / cantidad_particiones, pos_particion = 0;
	//Por cada caracter se busca su bitstream asociado
	for (char caracter : texto){
		while(pos < int(v_Shannon.size()) && caracter != v_Shannon[pos].simbolo) pos++;		
		if(pos < int(v_Shannon.size())){
			for (bool i : v_Shannon[pos].bitStream){
				texto_codificado.push_back(i);
			}
			//Se guarda la posicion del texto T y F en el vector de posiciones
			if(pos_particion == pos_texto){
				posicion p;
				p.pos_codificado = pos_textocodificado;
				p.pos_original = pos_texto;
        		this->posiciones.push_back(p);
				pos_particion += tam_particion;
			}
			pos_textocodificado += v_Shannon[pos].bitStream.size();
		}
		pos_texto++;
		pos = 0;
	}
	time_t end = clock();
	cout << "La codificacion de ShannonFano demoro: " << double(end - start)/CLOCKS_PER_SEC << " [segs]" << endl;	
	return texto_codificado;
}

//Busca y retorna la posicion mas cercana a i en el vector de posiciones
int vectorShannon::buscaPosicion(int i, vector<bool> texto_codificado){
	int largo_menor = v_Shannon.front().bitStream.size();
	int postext, postextcod = 0, pp = 0;
	vector<bool> subs;
	// i = 20
	// 10, 20, 30
	while(i > posiciones[pp].pos_original){ //Busca la posicion mas cercana por la izquierda
		pp++;
	}
	//Esto implica que hay que aumentar desde izq a derecha hasta encontrar la posicion i
	if(i == posiciones[pp].pos_original) return posiciones[pp].pos_codificado;
	else{
		pp = pp - 1;
	}
	postextcod = posiciones[pp].pos_codificado;
	postext = posiciones[pp].pos_original;
	while(postext < i){
		if(int(subs.size()) < largo_menor - 1) subs.push_back(texto_codificado[postextcod]);
		else{
			subs.push_back(texto_codificado[postextcod]);
			nodoHash *p = root->search(int(subs.size()), subs);
			if(p != nullptr){
				postext++;
				subs.clear();
			}
		}
		postextcod++;
	}
	return postextcod;
}

//Algoritmo de decodificacion basado en hashing, donde se busca por el tamaño del bitstream
//del caracter a buscar
vector<char> vectorShannon::decode_hashing(vector<bool> texto, int i, int k){
	cout << "Decoding hashing..." << endl;
	time_t start = clock();
	int decodes = 0;
	int largo_menor = v_Shannon.front().bitStream.size();
	int postext = buscaPosicion(i, texto);
	vector<bool> subs;
	vector<char> texto_decodificado;
	while(decodes < k - i){
		if(int(subs.size()) < largo_menor - 1) subs.push_back(texto[postext]);
		else{
			subs.push_back(texto[postext]);
			nodoHash *p = root->search(int(subs.size()), subs);
			if(p != nullptr){
				texto_decodificado.push_back(p->simbolo);
				decodes++;
				subs.clear();
			}
		}
		postext++;
	}
	time_t end = clock();
	if (i == 0) cout << "El decode_hashing demoro: " << double(end - start)/CLOCKS_PER_SEC << " [segs] para k = " << k << endl;
	else cout << "El decode_hashing demoro: " << double(end - start)/CLOCKS_PER_SEC << " [segs] para i =  "<<  i << " y j = " << k << endl;	
	return texto_decodificado;
}
//Algoritmo que decodifica el texto, usando busqueda secuencial.
vector<char> vectorShannon::decode_normal(vector<bool> texto, int i, int k){
	cout << "Decoding..." << endl;
	time_t start = clock();
	int decodes = 0;
	int pos = 0, postext = buscaPosicion(i, texto), largo_menor = v_Shannon.front().bitStream.size();

	vector<bool> subs; 
	vector<char> texto_decodificado;
  
	while(decodes < k - i){
		if(int(subs.size()) < largo_menor - 1) subs.push_back(texto[postext]);
		else{
			subs.push_back(texto[postext]);
			while(pos < int(v_Shannon.size()) && v_Shannon[pos].bitStream!=subs){
				pos++;
			}
			if(pos < int(v_Shannon.size())){
				texto_decodificado.push_back(v_Shannon[pos].simbolo);
				decodes++;
				subs.clear();
			}
			pos = 0;
		}
		postext++;
	}
	time_t end = clock();
	if (i == 0) cout << "El decode demoro: " << double(end - start)/CLOCKS_PER_SEC << " [segs] para k = " << k << endl;
	else cout << "El decode demoro: " << double(end - start)/CLOCKS_PER_SEC << " [segs] para i =  "<<  i << " y j = " << k << endl;	
	return texto_decodificado;
}

//Funcion generica para crear un archivo .txt
void vectorShannon::creaArchivo(string texto, string direccion){
	std::fstream file;
	file.open(direccion, std::ios::out);
	if(file.fail()){
		cout << "No se pudo crear el archivo..." << endl;
		return;
	}
	file << texto;
}

//Funcion que retorna que decode usar, decode_normal para el archivo dna.100MB
//y decode_hashing para el archivo english.100MB
vector<char> vectorShannon::decode(vector<bool> texto, int i, int k){
	if (texto.size() < 231246133) return decode_normal(texto,i,k);
	else return decode_hashing(texto,i,k);
}