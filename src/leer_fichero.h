//============================================================================
// Name        : leer_fichero.h
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================
#ifndef LEERFICH_H
#define LEERFICH_H 

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <malloc.h>


using namespace std;

class leerfich {

private:
	int a;
	int length;
	int clientes;
	int condicones;

	struct conditions{
		int primero;
		int segundo;
		string signo;
	};

	conditions *cond;
	int *w;
	int **cl;
	int *esfuerzo;
	//int *X;

public:

	leerfich();
	void inicializar();
	void lectura_datos(int array[], int j);
	void body();
	void print();
	void calcualrSa(int satis[]);//-----------------------------------------------------------
	void leer_clientes(fstream &data);
	void leer_recursos(fstream &data);
	void leer_condiciones(fstream &data);
	void solucion();//-------------------------------------------------------------
    void reparar(int individuo[]);
	void calcularS(string signo, int a, int b, int individuo[]);

	int getLong();
	int getNClientes();
	int* getEsfuerzo();
	~leerfich();
};

#endif
