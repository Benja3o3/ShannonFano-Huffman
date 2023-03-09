#include <stdlib.h>
#include "vectorShannon.h"
#include "arbolHuffman.h"

using namespace std;

int checkIfFileExists(const char * filename);
string experimentacion(vector<char> t, vector<bool> f, vector<bool> h,vectorShannon v, arbolHuffman a,int cantidad);
void printVector(vector<char> v);
void testDecode(vector<char> texto, int postxt, vector<char> DF);
string transformaDecimal(string numero);

int main(int argc, char **argv){
	if(argc != 3){
		cout << "Error. Debe ejecutarse como ./problema 'data/..' iteraciones_experimentacion " << endl;
		exit(EXIT_FAILURE);
	}
	time_t start = clock();
	string nombre_archivo = argv[1];
	int cantidad = atoi(argv[2]);
	while(cantidad < 0){
		cout << "Ingrese un numero mayor o igual que 0 para la cantidad de iteraciones" << endl; cin >> cantidad;
	}
	// Validacion de archivo
	if(!checkIfFileExists(nombre_archivo.c_str())){ 
		cout << "Archivo no existe" << endl;
		exit(EXIT_FAILURE);
	}
	vectorShannon V(nombre_archivo);// Se crea la estructura de Shannon Fano V
	arbolHuffman A(nombre_archivo);// Se crea la estrucutra de Huffman A
	
	vector<char> T = V.getText(); //Se crea la cadena T[1..n]
	vector<bool> F = V.codifica(T);//Se crea la cadena codificada con ShannonFano
	vector<bool> H = A.codifica(T); //Se crea la cadena codificada con Huffman

	//Experimentos
	//Comparando Cadenas
	cout << "Comparacion de Tamaño entre la cadena original y las codificadas" << endl;
	cout << "T[1..n] = " << (double(T.size())/1000000) << " MBs" << " - " << T.size() << " bits" << endl;
	cout << "F[1..m] = " << (double(F.size())/8000000) << " MBs" << " - " << F.size() << " bits" << endl;
	cout << "H[1..n] = " << (double(H.size())/8000000) << " MBs" << " - " << H.size() << " bits" << endl;

	//Decodificaciones
	//Se generan k aleatorios y se testean las decodificiaciones
	if (cantidad > 0 )string experimento = experimentacion(T,F,H,V,A,cantidad);
	//Se crea un archivo .txt con los datos de los experimentos
	//V.creaArchivo(experimento, "data/experimentos.txt");
	
	time_t end = clock();
	cout << "Tiempo total del programa: " << double(end - start)/CLOCKS_PER_SEC << " [segs]" << endl;	
	return EXIT_SUCCESS;
}

int checkIfFileExists(const char * filename){
    FILE *file;
    if ( (file = (fopen(filename, "r"))) ){
        fclose(file);
        return 1;
    }
    return 0;
}

//Funcion que realiza "c" experimentos entregados en el main y guarda los datos en un string
//para posteriormente devolver(si se requiere) un .txt con los datos
string experimentacion(vector<char> t, vector<bool> f, vector<bool> h, vectorShannon v, arbolHuffman a, int cantidad){
	unsigned t1, t2, t3, t4; //Variables de tiempo
    vector<char> decoded_shannon, decoded_huffman, decoded_shannon2, decoded_huffman2;
	string datos = "i-k-Shannon-Huffman\n"; //Datos para analizar
    int i = 0, j = 0, k; //Variables para experimentacion
	for (int x = 0; x < cantidad; x++){
		k = rand()%t.size();
		t1 = clock();
		decoded_shannon = v.decode(f, 0, k);
		t2 = clock();
		t3 = clock();
		decoded_huffman = a.decodificaHuffman(h, 0, k);
		t4 = clock();
		datos += "0-" + to_string(k) + "-";
		datos += transformaDecimal(to_string(double(t2 - t1)/CLOCKS_PER_SEC)) + "-";
		datos += transformaDecimal(to_string(double(t4 - t3)/CLOCKS_PER_SEC)) + "\n";
	    testDecode(t, 0, decoded_shannon);
        testDecode(t, 0, decoded_huffman);
    }
	for (int x = 0; x < cantidad; x++){
        j = rand()%t.size(); 
        i = rand()%t.size();
        while(i >= j){
            j = rand()%t.size(); 
            i = rand()%t.size();
        }
		t1 = clock();
		decoded_shannon2 = v.decode(f,i, j);
		t2 = clock();
        t3 = clock();
        decoded_huffman2 = a.decodificaHuffman(h, i, j);
        t4 = clock();
        datos += to_string(i) + "-" + to_string(j) + "-";
        datos += transformaDecimal(to_string(double(t2 - t1)/CLOCKS_PER_SEC)) + "-";
		datos += transformaDecimal(to_string(double(t4 - t3)/CLOCKS_PER_SEC)) + "\n";
        testDecode(t, i, decoded_shannon2);
        testDecode(t, i, decoded_huffman2);
	}
	return datos;
}

string transformaDecimal(string numero){
    string nuevo;
    for(char i : numero){
        if(i == '.') nuevo += ',';            
        else nuevo += i;
    }
    return nuevo;
}

void testDecode(vector<char> texto, int postxt, vector<char> DF){
    int posicion_df = 0;
    while (unsigned(long(posicion_df)) < DF.size() && texto[postxt] == DF[posicion_df]){
        postxt++;
        posicion_df++;
    }
	if (unsigned(long(posicion_df)) == DF.size()) cout << "FUNCIONA EL DECODE" << endl;
}

void printVector(vector<char> v){
	for (char i : v) cout << i;
	cout << endl;
}