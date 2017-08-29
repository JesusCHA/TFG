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
	solR = (individuo *) malloc (sizeof(individuo));
	solR[0].X = (int*)malloc(lf.getLong() * sizeof(int));
	solR[0].S=0;
	solR[0].E=0;
	solR[0].rank = 0;
	solR[0].crowding = 0;
}

void filtrado_hv::principal(int total_sol, int num){

	nsol = total_sol;
	if(num < 10)
		nFich = "sol0"+std::to_string(num);
	else{
		nFich = "sol"+std::to_string(num);
	}

	lectura_datos();

	redu_sol();

	calculate_HV();
}

void filtrado_hv::inicializar(){
	soluciones = new individuo[nsol];
	for (int i = 0; i < nsol; i++){
		soluciones[i].X = new int[lf.getLong()];//determinar el tamaño de cada individuo
		soluciones[i].S=0;
		soluciones[i].E=0;
		soluciones[i].rank = 0;
		soluciones[i].crowding = 0;
	}
}

void filtrado_hv::lectura_datos(){  // recordar modificar más tarde para la sucesión de números 


	fstream sol_file;						//flujo lectura de fichero solXX.csv

	string frase;

	inicializar();

 	sol_file.open("sol"+nFich+".csv");
	if (!sol_file.is_open()){
		cout << "ERROR: fopen failed to open" << endl/* nombre del fichero */ <<  "for reading"<< endl;
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
			soluciones[i].X[j] = frase[j];
		}
	}
}

void filtrado_hv::aum_tam(){
	solR = new individuo[sizeof(solR)+1];
	solR[sizeof(solR)].X = new int[lf.getLong()];//determinar el tamaño de cada individuo
	solR[sizeof(solR)].S = 0;
	solR[sizeof(solR)].E = 0;
	solR[sizeof(solR)].rank = 0;
	solR[sizeof(solR)].crowding = 0;
}

void filtrado_hv::redu_sol(){
	
	ofstream redu_sol_file;					//flujo escritura de fichero rSolXX.csv
	ofstream hv_sol_file;					//flujo lectura del fichero de hiper volumen
	
	int i,j,k;

	int dominated = 0;
	int equal = 0;
	int eq_ind = 0;
	nR_sol = 0;

	redu_sol_file.open("sol_r"+nFich+".csv");
	if (!redu_sol_file.is_open()){
		cout << "ERROR: fopen failed to open" << "sol_r" << nFich <<".csv" <<  "for reading"<< endl;
		return;
	}

	redu_sol_file.open("HV"+nFich+".csv");
	if (!redu_sol_file.is_open()){
		cout << "ERROR: fopen failed to open" << "HV" << nFich << ".csv" <<  "for reading"<< endl;
		return;
	}

	for (i = 0; i < nsol; i++){
		dominated = 0;
		for (j = 0; j < nsol; j++){
			if(std::calculos::domina(&soluciones[i], &soluciones[j]) == -1){
				dominated = 1;
				return;
			}
		}

		if(dominated == 0) {
			equal = 0;
			for(j = 0; j < nR_sol; j++){
				if(soluciones[i].S == soluciones[j].S && soluciones[i].E == soluciones[j].E){ // ¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡ HACER MÁS TARDE !!!!!!!!!!!!!!!!!!

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
			}
			aum_tam();
			solR[sizeof(solR)] = soluciones[i];
			nR_sol++;
			
		}
	}

	redu_sol_file.close();					
	hv_sol_file.close();
	cout << nsol << ". Total number of non-dominated and different solutions = " << nR_sol<<endl;
	if(nR_sol > nsol)
		cout << "ERROR:  This number is higher than the maximum allowed" << endl;
}
	

void filtrado_hv::calculate_HV(){
	
	fstream hv_value_sol_file;				//flujo escritura del fichero sHVXX.csv
	ofstream valueHV;						//flujo escritura del fichero ALL_HV.csv
	
	string frase;

	string hvFich, vHVFich, hv_command;
	double hypervolume;

	//FALTA ABRIR EL FICHERO QUE CALCULA EL HIPERVOLUMEN
	hvFich = "HV"+nFich+".csv";
	vHVFich = "valueHV"+nFich+".txt";
	hv_command = "hyp_ind hyp_ind_param.txt" + hvFich + "reference_set" + vHVFich;
	char *y = new char[hv_command.length() + 1];
	system(y);
	

	hv_value_sol_file.open("valueHV"+nFich+".txt");
	if (!hv_value_sol_file.is_open()){
		cout << "ERROR: fopen failed to open" << "valueHV" << nFich <<".csv" <<  "for reading"<< endl;
		return;
	}
	getline(hv_value_sol_file,frase,'\n');
	hypervolume = atof(frase.c_str()); 
	hv_value_sol_file.close();
	/*
	* FALTAN COSAS POR HACER
	*/

	valueHV.open( "All_HV.csv", ios::ate);
	if (!valueHV.is_open()){
		cout << "ERROR: fopen failed to open" << "All_HV.csv" <<  "for reading"<< endl;
		return;
	}
	valueHV << nFich << ";" << -hypervolume << ";" << nR_sol << endl;
	valueHV.close();

}

filtrado_hv::~filtrado_hv(){

	//hacer borrar soluciones;
}

