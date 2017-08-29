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

	protected: 
		struct individuo {
		int *X;				//resultado de cada individuo
		int S; 				//satisfaccion que causa al realizarse
		int E; 				//esfuerzo que requiere el individuo
		int rank;			//ranking de dominancia
		double crowding;	//distancia crowding
		};
	
		leerfich lf;			//instancia de leerficheros

		int max_efort;			//máximo esfuerzo que hay en la población
		int min_efort;			//mínimo esfuerzo que hay en la población

		int max_satis;			//máxima satisfacción que hay en la población
		int min_satis;			//mínima satisfacción que hay en la población


	private:		
		int sol_log;
		int aprender;

		int population;			//cantidad de población
		int nCharcos;			//numero de charcos
		int sizeCharco;			//tamaño del charco
		int evaluaciones; 		//cantidad de evaluaciones
		int evCharco;			//evaluaciones por charco


		

		individuo *poblacion; 	//poblacion de individuos

		int *satis;
		int *efor;

		struct memeplex{ //charcos de individuos
			individuo *inds;
		};
		memeplex *charcos;	//puntero a charcos


	public:
		calculos();

		int frog(ofstream &log);

		void generarPobl();
		void generarRand(individuo *ind);

		void writeFile(ofstream &log, individuo ind);

		void repartir();
		void reagrupar();

		void calcularSyE(individuo *ind);
		int  domina(individuo *a,individuo *b);
		void ordIntercambio ();
		void mejorar(ofstream &log);
		void mejorarInd(int mejor[], int peor[], individuo *newInd);
		

		void calculate_bounds(int num_sol,  individuo *listInd);
		static int compare_efort(const void *a, const void *b) ;
		static int compare_satis(const void *a, const void *b) ;
		static int compare_crowding(const void *a, const void *b) ;

		void rank_crowding(int num_sol, individuo *listInd);

		void imprimir();
		void imprimirP();

		virtual ~calculos();
	};

} /* namespace std */

#endif /* CALCULOS_H_ */
