//============================================================================
// Name        : leer_fichero.cpp
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "leer_fichero.h"

using namespace std;

leerfich::leerfich() {
	length = 0;
	a = 0;
	length = 0;
	clientes = 0;
	condicones = 0;
	cond = NULL;
	esfuerzo = NULL;
	w = NULL;
	cl = NULL;
	body();
}

void leerfich::inicializar() {
	esfuerzo = new int[length];
	cl = new int *[length];
	for (int i = 0; i < length; i++) {
		cl[i] = new int[clientes];
	}
}

void leerfich::body() {

	fstream data;
	string nombre;
	string frase;

	data.open("../data_set1.txt");
	if (data.is_open()) {
		leer_clientes(data);
		getline(data, frase, '\n');
		length = atoi(frase.c_str());
		inicializar();
		if (!data.eof()) {
			leer_recursos(data);
			leer_condiciones(data);
		}
		data.close();
	}

}

void leerfich::leer_recursos(fstream &data) {
	string frase;
	int i;
	int array[length];
	for (int j = 0; j <= clientes; j++) {
		getline(data, frase, ' ');
		for (i = 0; i < length; i++) {
			if (i == length - 1) {
				getline(data, frase, '\n');
			} else
				getline(data, frase, ' ');
			array[i] = atoi(frase.c_str());
		}
		lectura_datos(array, j - 1);
	}
}


void leerfich::leer_clientes(fstream &data) {
	string frase;
	getline(data, frase, '\n');
	clientes = atoi(frase.c_str());
	w = new int[clientes];
	getline(data, frase, ' ');
	for (int i = 0; i < clientes; i++) {
		if (i == clientes - 1) {
			getline(data, frase, '\n');
		} else
			getline(data, frase, ' ');
		w[i] = atoi(frase.c_str());
	}
}

void leerfich::lectura_datos(int array[], int j) {
	if (j == -1) {
		for (int i = 0; i < length; i++) {
			esfuerzo[i] = array[i];
		}
		cout << endl;
		
	} else {
		for (int i = 0; i < length; i++) {
			cl[i][j] = array[i];
		}
		cout << endl;
	}
}


void leerfich::leer_condiciones(fstream &data) {
	string frase;
	char val1, val2;
	getline(data, frase, '\n');
	condicones = atoi(frase.c_str());
	cond = new conditions[condicones];
	int a, b;
	string x, y;
	for (int i = 0; i < condicones; i++) {
		getline(data, frase, '\n');
			val1 = frase[0];
			val2 = frase[1];
			x += val1;
			x += val2;
			cond[i].primero = atoi(x.c_str());
			x.clear();

			val1 = frase[3];
			val2 = frase[4];
			y += val1;
			y += val2;
			cond[i].segundo = atoi(y.c_str());
			y.clear();

			cond[i].signo = frase[2];
			//cout << cond[i].primero <<" "<<cond[i].signo<< " "<< cond[i].segundo<<endl;
	}
}

void leerfich::calcualrSa(int satis[]) {
	
	
	for (int i = 0; i < length; i++) {
		int a = 0;
		for (int j = 0; j < clientes; j++) {
			a += cl[i][j] * w[j];
		}
		satis[i] = a;

	}

}

void leerfich::print() {

	cout << endl << endl;
	for (int j = 0; j < clientes; j++) {
		cout << "cliente " << j << ": ";
		for (int i = 0; i < length; i++) {
			cout << cl[i][j];
			cout << " ";
		}
		cout << endl;
	}
}

//va leyendo un array de condiciones y hacen que se cumplan
void leerfich::reparar(int individuo[]){
	for (int i = 0; i < condicones; i++) {
		calcularS(cond[i].signo, cond[i].primero-1, cond[i].segundo-1, individuo);
	}
}

//desglosa el array para poder aplicar las condiciones
void leerfich::calcularS(string signo, int a, int b, int individuo[]) {  
	switch (signo[0]) {
	case '>':
		/////	MODIFICACIÓN //////
		if (individuo[b] == 1) {
			individuo[a] = 1;
		} else individuo[b] = 0;
		break;
		///////////////////////////
	case '+':
		if (individuo[a] == 1) {
			individuo[b] = 1;
		} else if (individuo[b] == 1) {
			individuo[a] = 1;
		}
		break;
	case 'x':
		if (individuo[a] == 1) {
			individuo[b] = 0;
		} else if (individuo[b] == 1) {
			individuo[a] = 0;
		}
		break;
	default:
		break;
	}
}

int leerfich::getLong(){
	return length;
}

int leerfich::getNClientes(){
	return clientes;
}

int* leerfich::getEsfuerzo(){
	return esfuerzo;
}	

leerfich::~leerfich() {
	delete (esfuerzo);
	delete (w);
	delete (cl);
}

