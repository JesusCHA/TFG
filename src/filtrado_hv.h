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

namespace std{

	class filtrado_hv : public calculos{

		individuo *soluciones;
		individuo *aux;
		individuo *solR;
		
		string fichero;

		string nFich;
		int nsol;
		int nR_sol;

		public:
		filtrado_hv();
		void principal(int total_sol, int num);
		
		void inicializar();
		
		void lectura_datos();

		void aum_tam();
		void redu_sol();
		void calculate_HV();
		~filtrado_hv();
	};

} /*namespace std*/

#endif