//============================================================================
// Name        : main.cpp
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "calculos.h"
#include "filtrado_hv.h"
#include <time.h>

#define num_rep 31 // -- 31

int main(void) {
	

	calculos C;
	filtrado_hv F;
	ofstream log;	
	
	int sol_log;
	string nFichero;

	if(remove("All_HV.csv")==0) // Eliminamos el archivo

	for (int rep=0; rep < num_rep; rep++){
			
		srand(time(NULL)+rep);

		if(rep < 10)
			nFichero = "sol0"+std::to_string(rep);
		else{ 
			nFichero = "sol"+std::to_string(rep);
		}
		nFichero += ".csv";
		
		log.open("../log_res/"+nFichero);
		sol_log = C.frog(log);
		log.close();

		F.principal(sol_log,rep);

	}
}

