//============================================================================
// Name        : main.cpp
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "calculos.h"

#define num_rep 31

int main() {
	
	calculos C;
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
		cout << nFichero << " ";
		log.open("../log_res/"+nFichero);
		
		sol_log=C.frog(log);
		
		log.close();
		//filtrado_y_hv(rep, sol_log);
	}

	return 0;
}

