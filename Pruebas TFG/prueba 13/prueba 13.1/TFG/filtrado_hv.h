//============================================================================
// Name        : filtrado_hv.h
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================
#ifndef FILTRADOHV_H
#define FILTRADOHV_H 

#include "calculos.h"
#include <string.h>

namespace std{

	class filtrado_hv : public calculos{

		individuo *soluciones;	
		individuo *solR;		
		
		string fichero;			//nobre del fichero

		string nFich;			//para poder acceder al fichero, accedo con el número en forma de string
		int nsol;				//cantidad de soluciones
		int nR_sol;				//cantidad de soluciones reducidas

		int max_efort;			//máximo esfuerzo que hay en la población
		int max_satis;			//máxima satisfacción que hay en la población
		

		public:
		filtrado_hv();
		void principal(int total_sol, int num);
		
		void inicializar();
		
		void lectura_datos();

		void redu_sol();
		void calculate_HV();
		~filtrado_hv();
	};

} /*namespace std*/

#endif