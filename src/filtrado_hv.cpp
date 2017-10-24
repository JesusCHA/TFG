//============================================================================
// Name        : filtrado_hv.cpp
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "filtrado_hv.h"

using namespace std ;

/*
newInd = (individuo *) malloc (sizeof(individuo));
newInd->X = (int*)malloc(lf.getLong() * sizeof(int));
*/

filtrado_hv::filtrado_hv(){
	nsol = 0;
	nFich ="";

	max_satis=893;
	max_efort=85;

}

void filtrado_hv::principal(int total_sol, int num){

	nsol = total_sol;

	inicializar();

	if(num < 10)
		nFich = "0"+std::to_string(num);
	else{
		nFich = std::to_string(num);
	}

	lectura_datos();

	redu_sol();

	calculate_HV();

}

void filtrado_hv::inicializar(){
	int i;
	soluciones = new individuo[nsol];
	solR = new individuo[nsol];
	for (i = 0; i < nsol; i++){
		soluciones[i].X = new int[lf.getLong()];//determinar el tamaño de cada individuo
		soluciones[i].S=0;
		soluciones[i].E=0;
		soluciones[i].rank = 0;
		soluciones[i].crowding = 0;

		solR[i].X = new int[lf.getLong()];//determinar el tamaño de cada individuo
		solR[i].S=0;
		solR[i].E=0;
		solR[i].rank = 0;
		solR[i].crowding = 0;
	}
}

void filtrado_hv::lectura_datos(){  // recordar modificar más tarde para la sucesión de números 


	ifstream sol_file;						//flujo lectura de fichero solXX.csv

	string frase;
	string a;


	fichero = "sol"+nFich+".csv";
 	sol_file.open("../log_res/"+fichero);
	if (!sol_file.is_open()){
		cout << "ERROR: fopen failed to open "<< fichero <<  " for reading"<< endl;
		return;
	}

	for (int i=0;i < nsol;i++) {
		
		getline(sol_file, frase, ';');
		soluciones[i].S = atoi(frase.c_str()); // obtención y guardado de la satisfacción

		getline(sol_file, frase, ';');
		soluciones[i].E = atoi(frase.c_str()); // obtención y guardado del esfuerzo

		getline(sol_file, frase, '{');
		getline(sol_file, frase, '}');

		for (int j = 0; j < lf.getLong(); j++){
			a = frase[j];
			soluciones[i].X[j] = atoi(a.c_str());
		}
	}

}


void filtrado_hv::redu_sol(){
	
	ofstream redu_sol_file;					//flujo escritura de fichero rSolXX.csv
	ofstream hv_sol_file;					//flujo lectura del fichero de hiper volumen
	
	int i,j,k;

	int dominated = 0;
	int equal = 0;
	int eq_ind = 0;
	nR_sol = 0;

	redu_sol_file.open("../redu_hv/sol_r"+nFich+".csv");
	if (!redu_sol_file.is_open()){
		cout << "ERROR: fopen failed to open " << "sol_r" << nFich <<".csv" <<  " for writing"<< endl;
		return;
	}

	hv_sol_file.open("../redu_hv/HV"+nFich+".csv");
	if (!hv_sol_file.is_open()){
		cout << "ERROR: fopen failed to open " << "HV" << nFich << ".csv" <<  " for writing"<< endl;
		return;
	}

	for (i = 0; i < nsol; i++){
		dominated = 0;
		for (j = 0; j < nsol; j++){

			if(std::calculos::domina(&soluciones[i], &soluciones[j]) == -1){
				//std::calculos::imprimirI(&soluciones[i]);
				//cout << "its dominated" << endl;
				dominated = 1;
				break;
			}
		}
		
		
		if(dominated == 0) {
			//cout << "non dominated --- > ";
			//std::calculos::imprimirI(&soluciones[i]);
			equal = 0;
			for(j = 0; j < nR_sol; j++){
				if(soluciones[i].S == solR[j].S && soluciones[i].E == solR[j].E){ // ¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡ HACER MÁS TARDE !!!!!!!!!!!!!!!!!!

					eq_ind=0;
					for (k=0;k < lf.getLong();k++)
						if (soluciones[i].X[k] == solR[j].X[k]) eq_ind++;
						else break;

					if (eq_ind == lf.getLong()) {
						equal=1;
						break;
					}
				}
			}
			if(equal == 0){ 
				hv_sol_file << soluciones[i].S/(double)max_satis << " " << soluciones[i].E/(double)max_efort << endl;					//flujo escritura de fichero rSolXX.csv
				std::calculos::writeFile(redu_sol_file, soluciones[i]);
				std::calculos::change(&solR[nR_sol],&soluciones[i]);
				nR_sol++;
			}
			
		}
		//aum_tam();
	}

	redu_sol_file.close();					
	hv_sol_file.close();
	cout << nsol << ". Total number of non-dominated and different solutions = " << nR_sol <<endl;
}

void filtrado_hv::calculate_HV(){
	
	ifstream hv_value_sol_file;				//flujo lectura del fichero sHVXX.csv
	ofstream valueHV;						//flujo escritura del fichero ALL_HV.csv
	
	string frase;

	string hvFich, vHVFich, hv_command;
	double hypervolume;

	hvFich = "../redu_hv/HV"+nFich+".csv";
	vHVFich = "../redu_hv/valueHV"+nFich+".txt";
	hv_command = "./hyp_ind hyp_ind_param.txt " + hvFich + " reference_set " + vHVFich;
	char y[90];
	strcpy(y,(char *)hv_command.c_str());	
	system(y);
	

	hv_value_sol_file.open("../redu_hv/valueHV"+nFich+".txt");
	if (!hv_value_sol_file.is_open()){
		cout << "ERROR: fopen failed to open " << "valueHV" << nFich <<".txt" << " for reading"<< endl;
		return;
	}
	getline(hv_value_sol_file,frase,'\n');
	hypervolume = atof(frase.c_str());
	hv_value_sol_file.close();
	//
	//  
	//

	valueHV.open( "All_HV.csv", ios::out | ios::app);
	if (!valueHV.is_open()){
		cout << "ERROR: fopen failed to open" << "All_HV.csv" <<  "for reading"<< endl;
		return;
	}

	valueHV << nFich << ";" << -hypervolume*100 << ";" << nR_sol << endl;
	valueHV.close();

}

filtrado_hv::~filtrado_hv(){
	for (int i = 0; i < nsol; i++){
		delete(soluciones[i].X);
		delete(solR[i].X);
	}
	
	delete(soluciones);
	delete(solR);	
}

