//============================================================================
// Name        : calculos.cpp
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "calculos.h"

using namespace std ;

calculos::calculos() {
	population = 40;
	nCharcos = 4;
	sizeCharco = population/nCharcos;
	evaluaciones = 24;
	evCharco = 4; 

	satis= new int[lf.getLong()];
	efor = lf.getEsfuerzo();
	lf.calcualrSa(satis);
	poblacion = new individuo[population];
	charcos = new memeplex[nCharcos];
	for(int i = 0; i < nCharcos; i++){
		charcos[i].inds = new individuo[sizeCharco];
	}
}


void calculos::frog(){
	
	int r, cont;
	individuo *ind;
	for (int i = 0; i < population; i++) {
		ind = &poblacion[i];
		ind->X = new int[lf.getLong()];//determinar el tamaño de cada individuo
		ind->S= 0;
		ind->E=0;
		cont = 0;
		for(int j = 0; j < lf.getLong(); j++){//crear aleatorios
			r = rand()%2;
			ind->X[j] = r;
		}
		lf.reparar(ind->X);
		calcularSyE(ind);// calcular esfuerzo y satisfacción
		//ordIntercambio();---------------------------------->>>>> HACER MÁS ADELANTE
		
	}
	cout << "gg";
	rank(population);
	repartir(); //repartir entre los charcos

	//imprimir(); // mostrar la poblacion por charcos

	mejorar();	//determinar el mejor y le peor de cada charco
	//ordIntercambio();

	reagrupar();
	//volver a ordenar y listarx
}

void calculos::repartir(){
	int j,a;
	j=a=0;
	for(int i = 0; i < population; i++){
		charcos[j].inds[a] = poblacion[i];
		if(j+1 == nCharcos ){
			j=0;
			a++;
		}else{
			j++;
		}
	}
}


void calculos::reagrupar(){
	int j, a;
	j=a=0;
	for(int i = 0; i < population; i++){
		poblacion[i] = charcos[j].inds[a];
		if(j+1 == nCharcos ){
			j=0;
			a++;
		}else{ 
			j++;
		}
	}
}

void calculos::calcularSyE(individuo *ind){
	for(int i = 0; i < lf.getLong(); i++){
		if(ind->X[i] == 1){
			ind->S += satis[i];
			ind->E += efor[i];
		}
	}
	//cout << endl;
}


void calculos::mejorar(){
	individuo *mejorG; //no se dónde colocarlo
	individuo *mejor;
	individuo *peor;
	individuo *newInd;
	int r = 0;
	
	for(int i = 0; i < evCharco; i++){
	
	//------------ !!!!!!!!!!!!!!falta obtener mejor global después de cada charco¡¡¡¡¡¡¡¡¡ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
		for(int j = 0; j<nCharcos; j++){
			mejor = &charcos[j].inds[0];
			peor = &charcos[j].inds[sizeCharco];
			newInd->X = new int[lf.getLong()];
			//--------------bucle de mejorar va desde aquí
			for(int k; k < evaluaciones;k++){

				mejorarInd(mejor->X, peor->X, newInd); //mejorar individuo con el mejor del charco 

				lf.reparar(newInd->X);

				calcularSyE(newInd); // cambiar
			
				
				if(domina(newInd, peor) == 1){
					peor = newInd;
				}else{
					mejorarInd(mejorG->X, peor->X, newInd); //mejorar individuo con el mejor global
					if(domina(newInd, peor) == 1){
						
						peor = newInd;
					
					} else {

						for(int j = 0; j < lf.getLong(); j++){//crear aleatorios
							r = rand()%2;
							newInd->X[j] = r;
						}
						lf.reparar(newInd->X);
						calcularSyE(newInd);
					}
				}
			}//-------------hasta aquí ---> lo que falta es ordenarlos después para saber cual es el peor del charco y global.
		}
	}
}

void calculos::mejorarInd(int mejor[], int peor[], individuo *newInd){
	int r = 0;
	for (int i = 0; i < lf.getLong(); i++){
		if(peor[i] == mejor[i]){
			newInd->X[i] = peor[i];
		}else{
			r = rand()%100;
			if(r < 50) newInd->X[i] = mejor[i];
			else newInd->X[i] = peor[i];
		}
	}
}

int calculos::domina(individuo *a,individuo *b) {
        int flag1,flag2;
        flag1=flag2=0;

        if (a->E < b->E) flag1=1;
        else
                if (a->E > b->E) flag2=1;
        if (a->S > b->S) flag1=1;
        else
                if (a->S < b->S) flag2=1;

    if (flag1 == 1 && flag2 == 0) return 1;
    if (flag1 == 0 && flag2 == 1) return -1;
    return 0;
}

void calculos::imprimir(){
	//Mostrar poblacion de los charcos
	for(int i = 0; i < nCharcos; i++){
			cout << "charco nº:"<< i << endl << endl;
		for(int j = 0; j < sizeCharco; j++){
			for(int k = 0; k < lf.getLong(); k++){
				cout << charcos[i].inds[j].X[k] << " " ;
			}
			cout << endl << "Satisfacción: "<< charcos[i].inds[j].S << endl << "Esfuerzo: " << charcos[i].inds[j].E << endl;
			cout << "rank: " << charcos[i].inds[j].rank << endl << endl;
		}
		

	}
}


/* Calculate lower and upper bounds for the different objective functions */
void calculos::calculate_bounds(int num_sol) {
	int i;

	min_efort=max_efort=poblacion[0].E;
	min_satis=max_satis=poblacion[0].S;

	for (i=1;i < num_sol;i++) {
		if (poblacion[i].E > max_efort) max_efort=poblacion[i].E;
		else
			if (poblacion[i].E < min_efort) min_efort=poblacion[i].E;

		if (poblacion[i].S > max_satis) max_satis=poblacion[i].S;
		else
			if (poblacion[i].S < min_satis) min_satis=poblacion[i].S;
	}
}

/* Comparison for sorting in ascending order of magnitude by the total stations */
int calculos::compare_efort(const void *a, const void *b) {
	individuo *orderA = (individuo *)a;
	individuo *orderB = (individuo *)b;
	if (orderA->E > orderB->E) return 1;
	if (orderA->E < orderB->E) return -1;
	return 0;
}

/* Comparison for sorting in ascending order of magnitude by the total water quality */
int calculos::compare_satis(const void *a, const void *b) {
	individuo *orderA = (individuo *)a;
	individuo *orderB = (individuo *)b;
	if (orderA->S > orderB->S) return 1;
	if (orderA->S < orderB->S) return -1;
	return 0;
}

/* Comparison for sorting in descending order of magnitude by the crowding distance */
int calculos::compare_crowding(const void *a, const void *b) {
	individuo *orderA = (individuo *)a;
	individuo *orderB = (individuo *)b;
	if (orderA->crowding > orderB->crowding) return 1;
	if (orderA->crowding < orderB->crowding) return -1;
	return 0;
}



void calculos::rank(int num_sol) {
	cout << "rank";
	int p,q,domination,i,max_front,ini;
	int n[num_sol];			 	//array con la cantidad de los individuos que le dominan a un individuo
	int S_size[num_sol];	 	//conjunto de dominadas por el rank 1
	int S[num_sol][num_sol]; 	// tabla done se ve quien domina a quien
	int ranks_size[num_sol+1];	//
	int rank_pos;			 	//indica la posicion del ranking
	int rank[num_sol];			
	individuo copy_sol[num_sol]; 
	
	/* Fast non-dominated sort */
	//ranks_size = new int[2];
	rank_pos=ranks_size[1]=0;
	for (p=0;p < num_sol;p++) { 					// recorre toda la población con cada individuo y los compara con toda la poblacion
		S_size[p]=n[p]=0;							// inicializamos Sp y np
		for (q=0;q < num_sol;q++) {
			domination = domina(&poblacion[p],&poblacion[q]); 	
			if (domination == 1) {					// si p domina a q
				S[p][S_size[p]]=q;					// Sp incluye a q en el grupod de las que domina
				S_size[p]++;
			}
			else
				if (domination == -1) n[p]++; 		// si q domina a p
		}

		if (n[p] == 0) {							// si np es igual a cero, que es lo mismo que: no ha sido dominada por nadie,
			poblacion[p].rank=1;					// se incluye en el frente de pareto 1 
			rank[rank_pos]=p;			
			rank_pos++;
			ranks_size[1]++;			
		}
	}

	p=0;
	i=1;
	while (ranks_size[i] != 0) {					// mientras que el frente de pareto "i" no esté vacio 
		//ranks_size = (int*)malloc(sizeof(int *)i+1);
		ranks_size [i+1]=0;							// nos vamos al siguiente frente de pareto y lo inicializamos
		ini=p;					
		for (;p < ini+ranks_size[i];p++) {			// ya está inicializada p; mientras haya p en ESE frente de pareto
			int rp=rank[p];					
			for (q=0;q < S_size[rp];q++) {			// mientras haya q en el conjunto de dominadas de Sp
				n[S[rp][q]]--;				
				if (n[S[rp][q]] == 0) {				// si a q no le domina nadie
					poblacion[S[rp][q]].rank=i+1;		
					rank[rank_pos]=S[rp][q];
					rank_pos++;
					ranks_size[i+1]++;
				}
			}
		}
		i++;
	}

	///////////////////////////////////////////////////////////
	//	recordemos que "p" toma otra funcionalidad distinta  //
	///////////////////////////////////////////////////////////

	/* Crowding distance assignment */
	calculate_bounds(num_sol);
	for (p=0;p < num_sol;p++) {
		copy_sol[p]=poblacion[rank[p]]; //copia la poblacion de individuos
		copy_sol[p].crowding=0;			//inicializa el crowding de toda la poblacion a 0
	}


	p=0;
	max_front=i;			//max_front obtiene la cantidad de rangos que hay
	for (i=1;i < max_front;i++) {
			// Primero lo hacemos con el esfuerzo
		qsort(&copy_sol[p],ranks_size[i],sizeof(individuo),compare_efort);   	//se ordena con sort
		copy_sol[p].crowding=copy_sol[p+ranks_size[i]-1].crowding=DBL_MAX;  		//se establece infinito en el primero y en el último
		ini=p-1;		
		for (p++;p < ini+ranks_size[i];p++)											//se recorre el rango p(p es equivalente a un indice)
			copy_sol[p].crowding+=(copy_sol[p+1].E - copy_sol[p-1].E)/(double)(max_efort - min_efort);	//averiguamos la istancia crowding

			//Segundo lo hacemos con la satisfaccion
		p=ini+1;
		qsort(&copy_sol[p],ranks_size[i],sizeof(individuo),compare_satis);
		copy_sol[p].crowding=copy_sol[p+ranks_size[i]-1].crowding=DBL_MAX;
		for (p++;p < ini+ranks_size[i];p++)
			if (copy_sol[p].crowding != DBL_MAX)
				copy_sol[p].crowding+=(copy_sol[p+1].S - copy_sol[p-1].S)/(max_satis - min_satis);

		qsort(&copy_sol[ini+1],ranks_size[i],sizeof(individuo),compare_crowding); //por último volvemos a ordenar
		p=ini+1+ranks_size[i];
	}

	for (p=0;p < num_sol;p++) poblacion[p]=copy_sol[p]; // ponemos nuestra solución en la original
}

calculos::~calculos() {
	delete(satis);
	for(int i = 0; i < nCharcos ; i++){
		delete(charcos[i].inds);
	}
	delete(charcos);
	delete(poblacion);
}
 /* namespace std */


