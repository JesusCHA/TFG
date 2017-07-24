//============================================================================
// Name        : calculos.h
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================
#ifndef CALCULOS_H_
#define CALCULOS_H_

#include "leer_fichero.h"
#include <float.h>

namespace std {

class calculos {

	leerfich lf;			//instancia de leerficheros

	int population;			//cantidad de poblacion
	int nCharcos;			//numero de charcos
	int sizeCharco;			//tamaño del charco
	int evaluaciones; 		//cantidad de evaluaciones
	int evCharco;			//evaluaciones por charco

	int min_efort;
	int max_efort;
	int min_satis;
	int max_satis;

	struct individuo {
		int *X;				//resulatado de cada individuo
		int S; 				//satisfaccion que causa al realizarse
		int E; 				//esfurezo que requiere el individuo
		int rank;			//raning de dominancia
		double crowding;	//distancia crowding
	};
	individuo *poblacion; 	//poblacion de individuos

	int *satis;
	int *efor;

	struct memeplex{ //charcos de individuos
		individuo *inds;
	};
	memeplex *charcos;

	public:
		calculos();
		void frog();

		void repartir();
		void reagrupar();

		void calcularSyE(individuo *ind);
		int  domina(individuo *a,individuo *b);
		void ordIntercambio ();
		void mejorar();
		void mejorarInd(int mejor[], int peor[], individuo *newInd);

		void calculate_bounds(int num_sol);
		static int compare_efort(const void *a, const void *b) ;
		static int compare_satis(const void *a, const void *b) ;
		static int compare_crowding(const void *a, const void *b) ;

		void rank(int num_sol);

		void imprimir();
		virtual ~calculos();
	};

} /* namespace std */

#endif /* CALCULOS_H_ */
