//============================================================================
// Name        : main.cpp
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "calculos.h"
#include "filtrado_hv.h"

#define num_rep 31

int main(void) {
	

	calculos C;
	filtrado_hv F;
	ofstream log;	
	
	int sol_log;
	string nFichero;

	for (int rep=0; rep < num_rep; rep++){
		if(rep < 10)
			nFichero = "sol0"+std::to_string(rep);
		else{ 
			nFichero = "sol"+std::to_string(rep);
		}
		nFichero += ".csv";
		
		log.open("../log_res/"+nFichero);
		sol_log = C.frog(log);
		cout << sol_log << "sol" << endl;
		log.close();

		cout << rep << endl;

		F.principal(sol_log,rep);

	}
}

