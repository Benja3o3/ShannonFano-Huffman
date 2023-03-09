#include "arbolHuffman.h"

// Funcion que genera un "vector de nodos" a partir de la ruta del archivo
arbolHuffman::arbolHuffman(string filename){
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
    // Comienza a generar la estructura de vector con sus probabilidades
    setColaDePrioridad(contadores);  // Crea el vector
    //imprime(contadores); //imprime cada simbolo con su probabilidad
    getArbolHuffman();
    asignaBitStream(arbol, "");  // asigna al nodo raiz los bitstream 
    obtenerLargosCodigos(arbol, 0);
    creaClavesCanonicas(largosCodigos);
    //imprime arbol
    //imprimeDiccionario(largosCodigos); 
}

vector<bool> arbolHuffman::codifica(vector<char> const &texto){
    time_t inicio = clock();
    vector<bool> textoCodificado;
    
    int n = 0;
    for (char i : texto){
        if (int(i) >= 0 && int(i) <= 256){
            posiciones.push_back(textoCodificado.size()); 
            for(char j : dictCodigos[i]->bitStream){
                if (j == '0'){
                    textoCodificado.push_back(0);
                }
                else{
                    textoCodificado.push_back(1);
                }
            }
        }	
        n++;
    }
    time_t fin = clock();
    cout << "La codificacion de Huffman demoro: " << double(fin - inicio) / CLOCKS_PER_SEC << "[segs]" <<endl;
    return textoCodificado;
}

vector<char> arbolHuffman::decodificaHuffman(vector<bool> const &textoCodificado, int rangoInicial, int rangoFinal){
    time_t inicio = clock();

    vector<char> textoDecodificado;
    int largoMaximo = prev(largosCodigos.end())->first ;

    vector<char> simbolos;                      // arreglo de los simbolos de orden ascendente
    vector<int> largoC(largoMaximo + 1, 0);    // largoC[l] = i indica que el primer código con largo l es simbolo[i]
    vector<string> codigo(largoMaximo + 1, "null"); // donde codigo[l] almacena el primer código de largo l (codigo[l] = simbolo[largoC[l]])
    int c = 0;
    // recorrer mapa para asignar elementos a los arreglos;
    int k = 0;
    auto iter = largosCodigos.begin(); // recorrera los largos
    while (iter != largosCodigos.end()){
        vector<nodo *> vectorDeNodos = iter->second; // almacena el vector del largo actual
		
        for (int i = vectorDeNodos.size()-1; i >= 0; i--){
            if (unsigned(i) == vectorDeNodos.size() - 1){
                largoC[iter->first] = k;
                codigo[iter->first] = vectorDeNodos[i]->bitStream;
            }
            simbolos.push_back(vectorDeNodos[i]->simbolo);
            k++;
        }
        iter++;
    }
    
    ////////////// AHORA APLICAR DECODIFICACION /////////////////////////////////
    int h = largoMaximo;
    string secuencia = "";
    int comienzo = 0;
    int l, r, m;  // l izquierda, r derecha, m posicion
    //bool tipo = false; DESCOMENTAR PARA PROBAR LA BUSQUEDA BINARIA
	int largoNulo = -1;
    for(unsigned int i = largosCodigos.begin()->first; i < codigo.size(); i++){
        if(codigo[i] == "null"){
			largoNulo = i;
            // tipo = true; DESCOMENTAR PARA PROBAR LA BUSQUEDA BINARIA
        }//Busca en el arreglo de codigos si existe algun salto en los largos de los bitstreams
		 //Si existe no ocupará busqueda secuencial
    }
    //tipo =True significa que el vector contiene espacios vacios por lo que no se puede aplicar el algoritmo de BB
    //tipo = false significa que el vector no tiene problemas para aplicar el algoritmo de  BB

    int rango = rangoInicial;
    //Para el caso cuando se tiene un rango inicial y final:
    if(rangoInicial != 0){
        comienzo = posiciones[rangoInicial]; // rango sera la posicion en el texto codificado en donde comenzara la decodificacion 
        h = comienzo + largoMaximo; 
    }
    l = largosCodigos.begin()->first;
    r = codigo.size()-1;
    while(rango < rangoFinal){
        for (int i = comienzo; i < h; i++){ //recorriendo texto de h bits en h bits (El largo del bitstream menos significativo)
            if(secuencia.size() != unsigned(largoMaximo)){
                if(textoCodificado[i] == 0){
                    secuencia.push_back('0');
                }
                else{
                    secuencia.push_back('1');
                }
            }
        }
        
        int N = stoi(secuencia, 0, 2);     // convierte la secuencia a decimal entero 
        m=buscaSecuencialmente(N,codigo,l,r,largoNulo,largoMaximo); //Busqueda del largo correcto (usaremos m en vez de l)
        
		//Sale de la busqueda y obtiene un m (largo) valido
        
        if(c == 0){ // sirve para verificar el primer caso donde comienzo = 0 o k 
            comienzo = comienzo+largoMaximo;
            h = comienzo+m;
            c++;
        }
        else{
            comienzo = h; 
            h = comienzo+m;
        }
        int n_prima = stoi(secuencia.substr(0,m),0,2); // es el n prima
        textoDecodificado.push_back(simbolos[largoC[m]+n_prima-stoi(codigo[m],0,2)]); //Formula para obtener el simbolo
        secuencia = secuencia.substr(m,largoMaximo); 
        //En vez de reiniciar la secuencia, reutilizamos lo que ya sobra
        rango++; //Reinicia el rango para seguir a buscar el siguiente simbolo
    }
    time_t fin = clock();
    cout << "La decodificacion de Huffman demoro: " << double(fin - inicio) / CLOCKS_PER_SEC << endl;
    return textoDecodificado;
}

bool arbolHuffman::usaFormula(int m,string lado,int N,vector<string> const &codigo, int largoMaximo){
    
    if(lado=="izq"){    
        return N >= (stoi(codigo[m],0,2) )*(pow(2,(largoMaximo-m)));
    }
    return N < (stoi(codigo[m+1],0,2))*(pow(2,(largoMaximo-(m+1))));
    
}
int arbolHuffman::buscaSecuencialmente(int N,vector<string> const &codigo,int l, int r,int largoNulo,int largoMaximo){
    bool der,izq;
    int m=l; //Se le asigna m al largo minimo y se comienza a buscar que la condicion derecha y izquierda calcen
    der = usaFormula(m,"der",N,codigo,largoMaximo);
    izq = usaFormula(m,"izq",N,codigo,largoMaximo);
    
    while(!(der&&izq)){ //Mientras no se cumplan ambas condiciones...
        
        m++;
        if(m==r){
            der = true;
            izq = true;
        }
        else{	
            if((m != largoNulo) && ((m+1) != largoNulo)){
                izq = usaFormula(m,"izq",N,codigo,largoMaximo);
                der = usaFormula(m,"der",N,codigo,largoMaximo);
            }
            else if ((m != largoNulo)&& ((m+1) == largoNulo)){ 
                izq = usaFormula(m,"izq",N,codigo,largoMaximo);
                der = usaFormula(m+1,"der",N,codigo,largoMaximo);
            }
            else if ((m == largoNulo) && ((m+1) != largoNulo)){
                m++;
                izq = usaFormula(m,"izq",N,codigo,largoMaximo);
                der = usaFormula(m+1,"der",N,codigo,largoMaximo);
            }
        }
    }
    //Lamentablemente hay que hacerlo caso a caso para que funcione bien cuando se salta un largo de bitstream
    //En "teoria" se pierde eficiencia al hacer tantas comparaciones, pero en la practica,en este trabajo, no será así
    //Ya que los casos donde se anula el largo son con una probabilidad muy baja
    //El programa trabajara el 95% de las veces (O más inclusive) con los largos mas bajos que no tienen problema
    return m;


}   

int arbolHuffman::buscaBinariamente(int N,vector<string> const &codigo,int l, int r,int largoNulo,int largoMaximo){
    int der,izq;
    int m = (l+r)/2;
    der = usaFormula(m,"der",N,codigo,largoMaximo);
    while(!der){ //Realiza busqueda binaria para encontrar el m donde se cumple la condicion derecha
        if((m+1)==r){
            m=r;
            der = true;
            izq = true;
        }
        else{
            l=m;
            m=(l+r)/2;
            der = usaFormula(m,"der",N,codigo,largoMaximo);
        }
    }
    izq = usaFormula(m,"izq",N,codigo,largoMaximo); //Cuando se cumple la condicion derecha, atualiza la condicion izquierda 
                                        //con el m encontradol

    while(!izq){ //Realiza busqueda secuencial para encontrar el m correcto de la condicion izquierda
                //Ya que no quedan muchos datos que comparar
        m--;
        izq = usaFormula(m,"izq",N,codigo,largoMaximo);
    }
    return m;
}

// Destructor e la clase
arbolHuffman::~arbolHuffman(){
    while (!colaDePrioridad.empty()){
        colaDePrioridad.pop();
    }
    totalCaracteres = 0;
}

void arbolHuffman::cuentaLinea(vector <int> &V, string linea){
  for(char i : linea){
		if(int(i) < ascii && int(i) > 0){
			totalCaracteres++;
			V[int(i)]++;
		}
	}
}

void arbolHuffman::setColaDePrioridad(vector<int> V){
	double p;
	for (unsigned int i = 0; i < V.size(); i++){
		p = double(V[i])/double(totalCaracteres);
		if(p > 0){
			nodo *q = new nodo;
            q->simbolo = char(i);
            q->probabilidad = p;
            q->bitStream = "";
            q->der = q->izq = nullptr;
            colaDePrioridad.push(q);
		}
	}
}

void arbolHuffman::getArbolHuffman(){
    while(colaDePrioridad.size() >= 2){
        nodo *z = new nodo; 
        z->simbolo = '\0'; // No es un simbolo, solo es un nodo para sumar probabilidades

        nodo *x = colaDePrioridad.top();
        colaDePrioridad.pop();
        z->izq = x;
        
        nodo *y = colaDePrioridad.top();
        colaDePrioridad.pop();
        z->der = y;

        z->probabilidad = x->probabilidad + y->probabilidad;
        colaDePrioridad.push(z);
    }
    
    this->arbol = colaDePrioridad.top();
    colaDePrioridad.pop();
}

void arbolHuffman::asignaBitStream(nodo *r, string bit){
    if (r != nullptr){
        asignaBitStream(r->izq, bit + "0");
        if(r->simbolo != '\0'){
            r->bitStream = bit;
        }
        asignaBitStream(r->der, bit + "1");
    }
}

void arbolHuffman::obtenerLargosCodigos(nodo *r, int l) { // recorrer el arbol de izquierda a derecha y asigna el largo de cada nodo
  if ((r->der == nullptr && r->izq == nullptr )){
    r->largo = l;
    r->bitStream = "";
    if(r->simbolo != '\0'){
        dictCodigos.insert({r->simbolo, r});
        if(largosCodigos.find(l) != largosCodigos.end()){ // si ya existe la clave en el diccionario
            largosCodigos[l].push_back(r);
        }
        else{ // si no existe, crea el vector y inserta la clave
            vector<nodo *> a;
            a.push_back(r);
            largosCodigos.insert({l, a});
        }
    }
    return;
  }
  obtenerLargosCodigos(r->izq, l + 1);
  obtenerLargosCodigos(r->der, l + 1);
}

void arbolHuffman::creaClavesCanonicas(map<int, vector<nodo *>> const &largosCodigos){
    auto iter = largosCodigos.begin();
    int largo = (iter->second).size();
    vector<nodo *> vector = iter->second;
    int ultimo = vector.size() - 1; // el ultimo elemento del vector

    // El primer caso donde se llena de 0 el primer codigo
    for (int j = 0; j < iter->first; j++){
        vector[ultimo]->bitStream += "0";
    }
    dictCodigos[vector[ultimo]->simbolo]->bitStream = vector[ultimo]->bitStream;
    int entero_anterior;
    for (int i = ultimo - 1; i >= 0; i--){                // solamente caso de los consecutivos, si el primer vector de la clave tuviera un solo elemento entonces no entra al for
        string n = vector[i + 1]->bitStream;                // toma el bitstream del anterior anterior
        entero_anterior = stoi(n, 0, 2) + 1;                // transforma el numero binario anterior en un entero y le suma 1
        string bs = toBinary(entero_anterior, iter->first); // devuelve el numero nuevo a formato binario en string
        vector[i]->bitStream = bs;
        dictCodigos[vector[i]->simbolo]->bitStream = bs;
    }
    ////////////////////////////////Primera linea de diccionario lista/////////////////////////////////
    string bitStreamAux = (iter->second).front()->bitStream; // Ultima clave agregada
    iter++;

    while (iter != largosCodigos.end()){
        string codigo_anterior = bitStreamAux;
        entero_anterior = stoi(codigo_anterior, 0, 2);

        int n = 2 * (entero_anterior + 1);    // formula
        string bs = toBinary(n, iter->first); // devuelve el numero nuevo a formato binario en string
        vector = iter->second;                // este es el vector que contiene los nodos
        vector[vector.size() - 1]->bitStream = bs; //Le asigna al primero de la nueva linea inmediatamente la formula

        largo = (iter->second).size();

        if (largo > 1){
            for (int i = vector.size() - 2; i >= 0; i--){        // recorre de derecha a izquierda el vector de nodos asignar bitstreams siguientes que son consecutivos
                string codigo_anterior = vector[i + 1]->bitStream; // toma el bitstream del anterior anterior
                entero_anterior = stoi(codigo_anterior, 0, 2) + 1; // transforma el numero binario anterior en un entero y le suma 1
                bs = toBinary(entero_anterior, iter->first);       // devuelve el numero nuevo a formato  binario en string
                vector[i]->bitStream = bs;
            }
        }
        bitStreamAux = (iter->second).front()->bitStream;
        iter++;
    }
    

}

string arbolHuffman::toBinary(int n, int largo){
    string r;
    unsigned l = largo;
    while (n != 0){
        r += (n % 2 == 0 ? "0" : "1");
        n /= 2;
    }
    reverse(r.begin(), r.end());
    while (r.length() < l){
        r = "0" + r;
    }
    return r;
}

void arbolHuffman::setAscii(unsigned int ascii){
    if (ascii > 125)
        this->ascii = 256;
    else
        this->ascii = 126;
}

unsigned int arbolHuffman::getAscii(){
	return ascii;
}

vector<char> arbolHuffman::getText(){
	return this->texto;
}

// codigo solo para la visualizacion del arbol sacado de: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
void arbolHuffman::printBT(const string &prefix, const nodo *node, bool isLeft){
  if (node != nullptr){
    cout << prefix;

    cout << (isLeft ? "├──" : "└──");

    // print the value of the node
    std::cout << node->simbolo << "(" << node->probabilidad << ")" << " bitstream: " << node->bitStream << std::endl;

    // enter the next tree level - left and right branch
    printBT(prefix + (isLeft ? "│   " : "    "), node->izq, true);
    printBT(prefix + (isLeft ? "│   " : "    "), node->der, false);
  }
}

void arbolHuffman::printBT(const nodo *node){
  printBT("", node, false);
}

void arbolHuffman::imprime(vector<int> &V){
    double p;
    for (unsigned int i = 0; i < V.size(); i++){ 
        p = double(V[i])/double(totalCaracteres);
        if(p > 0){
            cout <<"Simbolo: " << "(" << char(i) << ") " <<  "Probabilidad: " << p << endl;
        }
    }
}

void arbolHuffman::imprimeDiccionario(map<int, vector<nodo *>> m){
    auto iter = m.begin();
    while (iter != m.end()){
        cout << "[" << iter->first << ",";
        for (unsigned i = 0; i < (iter->second).size(); i++){
            cout << (iter->second)[i]->simbolo << " ";
        }
        cout << "]" << endl;
        ++iter;
    }
}