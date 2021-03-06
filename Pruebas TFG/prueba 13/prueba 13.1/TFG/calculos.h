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
#include <climits>

namespace std {

class calculos {

	protected: 
		struct individuo {
		int *X;				//resultado de cada individuo
		int S; 				//satisfaccion que causa al realizarse
		int E; 				//esfuerzo que requiere el individuo
		int rank;			//ranking de dominancia
		double crowding;	//distancia crowding
		int violations;		//numero de restricciones violadas (no cumplidas)
		};
	
		leerfich lf;			//instancia de leerficheros



	private:		
		int sol_log;
		int aprender;			//rango de aprendizaje del individuo con respecto al mejor

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

		int max_efort;			//máximo esfuerzo que hay en la población
		int min_efort;			//mínimo esfuerzo que hay en la población

		int max_satis;			//máxima satisfacción que hay en la población
		int min_satis;			//mínima satisfacción que hay en la población

		int prob_mut;

		struct tipo_fitness {
			int req;			//número de requisito
			double fit;			//fitness asociado
		};

	public:
		calculos();

		int frog(ofstream &log);

		void generarPobl();
		static int compare_fitness(const void *a, const void *b);
		void generarProblemAware(individuo *ind, int pos, tipo_fitness *fitness);

		void generarRand(individuo *ind);

		void writeFile(ofstream &log, individuo ind);

		void repartir();
		void reagrupar();

		void calcularSyE(individuo *ind);
		void change(individuo *a, individuo *b);
		int  domina(individuo *a,individuo *b);
		void mejorar(ofstream &log);

		void mejorarInd(int mejor[], int peor[], individuo *newInd);
		int mejorarIndSatis(individuo *newInd);
		int mejorarIndEfor(individuo *newInd);



		void calculate_bounds(int num_sol,  individuo *listInd);
		static int compare_efort(const void *a, const void *b) ;
		static int compare_satis(const void *a, const void *b) ;
		static int compare_crowding(const void *a, const void *b) ;

		void rank_crowding(int num_sol, individuo *listInd);

		void imprimir();
		void imprimirP();
		void imprimirI(individuo *ind);

		virtual ~calculos();
	};

} /* namespace std */

#endif /* CALCULOS_H_ */
