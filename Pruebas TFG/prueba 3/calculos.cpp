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
	evCharco = 25;
	evaluaciones = 10000 / (nCharcos * evCharco);

	aprender=75;//75

	sol_log = 0;

	satis= new int[lf.getLong()];
	efor = lf.getEsfuerzo();
	lf.calcualrSa(satis);

	poblacion = new individuo[population];
	charcos = new memeplex[nCharcos];
	for(int i = 0; i < nCharcos; i++){
		charcos[i].inds = new individuo[sizeCharco];
	}
}


int calculos::frog(ofstream &log){
	
	generarPobl();

	sol_log = 0;

	for (int i = 0; i < evaluaciones; ++i){
		
		rank_crowding(population, poblacion);

		repartir(); //repartir entre los charcos
		
		mejorar(log); //determinar el mejor y le peor de cada charco
		
		//ordIntercambio();
		reagrupar();
		
	}

	// Guardar en el fichero log todas las soluciones que hay en la población
	// satisfaccion;esfuerzo;vector X en cada línea del fichero
	for (int i = 0; i < population; i++){
		writeFile(log, poblacion[i]);
		sol_log++;
	}
		
	//volver a ordenar y listarx
	

	return sol_log;
}

void calculos::generarPobl(){
	individuo *ind;
	for (int i = 0; i < population; i++) {
		ind = &poblacion[i];
		ind->X = new int[lf.getLong()];//determinar el tamaño de cada individuo
		ind->S=0;
		ind->E=0;
		ind->rank = 0;
		ind->crowding = 0;
		generarRand(ind);
	}
}

void calculos::generarRand(individuo *ind){
	int r;
	for(int i = 0; i < lf.getLong(); i++){//crear aleatorios
		r = rand()%2;
		ind->X[i] = r;
	}

	lf.reparar(ind->X);
	calcularSyE(ind);// calcular esfuerzo y satisfacción
}


void calculos::writeFile(ofstream &log, individuo ind){
	log << ind.S <<";"<< ind.E << ";" << "{";
	for (int i = 0; i < lf.getLong(); ++i){
		log << ind.X[i];
	}
	log << "}" << endl;
	//por cada solución almacenada en el fichero
	
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
	int a;
	a=0;
	for(int i = 0; i < nCharcos; i ++){
		for(int j = 0; j < sizeCharco; j++){
			poblacion[a]= charcos[i].inds[j];
			a++;
		}
	}

}

void calculos::calcularSyE(individuo *ind){
	int s = 0, e = 0;
	for(int i = 0; i < lf.getLong(); i++){
		if(ind->X[i] == 1){
			s = s + satis[i];
			e = e + efor[i];
		}
	}
	ind->S = s;
	ind->E = e;
}

void calculos::change(individuo *a, individuo *b){
	for(int i = 0; i < lf.getLong(); i++){
		a->X[i] = b->X[i];
	}
	a->S = b->S;
	a->E = b->E;
}


void calculos::mejorar(ofstream &log){


	individuo *newInd;
	newInd = new individuo();
	newInd->X = new int[lf.getLong()];
	newInd->S = 0;
	newInd->E = 0;

	individuo *mejorG[3]; 
	individuo *mejor;
	individuo *peor;
	int posMG;

	mejorG[0] = &charcos[0].inds[0];
	mejorG[1] = &charcos[1].inds[0];
	mejorG[2] = &charcos[2].inds[0];

	for(int j = 0; j<nCharcos; j++){

		mejor = &charcos[j].inds[0];
		peor = &charcos[j].inds[sizeCharco-1];
		
		//--------------bucle de mejorar va desde aquí
		for(int k=0; k < evCharco;k++){
			
			mejorarInd(mejor->X, peor->X, newInd); //mejorar individuo con el mejor del charco 

			lf.reparar(newInd->X);
			calcularSyE(newInd); // cambiar
			
			if(domina(newInd, peor) == 1){
				
				change(&charcos[j].inds[sizeCharco-1],newInd);
				
			}else{
				posMG = rand()%3;
				mejorarInd(mejorG[posMG]->X, peor->X, newInd); //mejorar individuo con el mejor global
				
				
				lf.reparar(newInd->X);
				calcularSyE(newInd); // cambiar
				k++; //hemos evaluado la s y e una vez más
				
				if(domina(newInd, peor) == 1){
					
					change(&charcos[j].inds[sizeCharco-1],newInd);
				} else {
					
					generarRand(newInd);
					writeFile(log, charcos[j].inds[sizeCharco-1]);// Guardar en el fichero log la solución que hay en charcos[j].inds[sizeCharco-1] 
					sol_log++;		

					change(&charcos[j].inds[sizeCharco-1],newInd);
				}
			}

			// Insertar la nueva solución en la posición que le corresponda (ordenación parcial)
			individuo auxInd;
			auxInd.X=new int[lf.getLong()];
			change(&auxInd,&charcos[j].inds[sizeCharco-1]);
			int pos = sizeCharco - 2;
			while (domina(&auxInd,&charcos[j].inds[pos]) == 1){
				change(&charcos[j].inds[pos+1],&charcos[j].inds[pos]);
				pos--;
				if (pos < 0) break;
			}

			change(&charcos[j].inds[pos+1],&auxInd);
			for (int g = 2; g >= 0; g--){
				if (domina(&auxInd,mejorG[g]) == 1){
					 mejorG[g] = &auxInd;
					 break;
				}
			}
			delete(auxInd.X);
		}
		//-------------hasta aquí ---	

	}

	/////////////////////////////////////////////
	delete(newInd->X);
	delete(newInd);	
	/////////////////////////////////////////////

}

void calculos::mejorarInd(int mejor[], int peor[], individuo *newInd){
	int r = 0;

	for (int i = 0; i < lf.getLong(); i++){
		if (peor[i] == mejor[i]){
			newInd->X[i] = peor[i];
		}else{
			r = rand()%100;
			if (r < aprender) newInd->X[i] = mejor[i]; 
			else newInd->X[i] = peor[i];
		}
	}
	r = rand()%2;
	switch (r){
		case 0:
			//Intentamos mejorar la satisfacción, pero si no lo conseguimos intentamos mejorar el esfuerzo
			if (mejorarIndSatis(newInd) == -1) mejorarIndEfor(newInd);
		break;   
		case 1:
			//Intentamos mejorar el esfuerzo, pero si no lo conseguimos intentamos mejorar la satisfacción
			if (mejorarIndEfor(newInd) == -1) mejorarIndSatis(newInd);
		break;
	}			
}

int calculos::mejorarIndSatis(individuo *newInd){
	int j;
	int minS, pmS, pMS; 

	do{
		minS = INT_MAX;
		pmS = pMS = -1; //Inicialmente desconocemos sus posiciones
		for (j = 0; j < lf.getLong(); j++){ //Busca el requisito usado con la mínima satisfacción
			if (newInd->X[j] == 1){
				if (satis[j] < minS){
					minS = satis[j];
					pmS = j;
				}		
			}
		}
		if (pmS != -1){ //Si pmS == -1 -> La solución no se puede mejorar respecto a satisfacción
			for (j = 0; j < lf.getLong(); j++){ //Busca el requisito no usado con la máxima satisfacción sin empeorar el esfuerzo
				if (newInd->X[j] == 0){
					if (satis[j] > satis[pmS] && efor[j] <= efor[pmS]){
						if (pMS != -1){
							if (satis[j] > satis[pMS]) pMS = j;
						}
						else pMS = j;
					}
				}
			}
			if (pMS == -1) //Este requisito no es mejorable, lo intentamos con el siguiente con menor satisfacción
				newInd->X[pmS] = -1; //Desactivamos temporalmente este requisito para que no influya en la búsqueda de otros requisitos
			else{ //Mejoramos la solución
				newInd->X[pMS] = 1;
				newInd->X[pmS] = 0;
				pmS = -1; //Para salir del bucle
			}
		}
	}
	while (pmS != -1);
	//Restablecemos (ponemos otra vez a "1" todas las posiciones que hemos puesto a "-1" de) la solución antes de terminar
	for (j = 0; j < lf.getLong(); j++)
		if (newInd->X[j] == -1) newInd->X[j] = 1;

	return pMS; //Valdrá -1 sólo si la solución no se puede mejorar respecto a satisfacción
}

int calculos::mejorarIndEfor(individuo *newInd){
	int j;
	int maxE, pME, pmE;

	do{
		maxE = 0;
		pME = pmE = -1; //Inicialmente desconocemos sus posiciones
		for (j = 0; j < lf.getLong(); j++){ //Busca el requisito usado con el máximo esfuerzo
			if (newInd->X[j] == 1){
				if (efor[j] > maxE){
					maxE = efor[j];
					pME = j;
				}		
			}
		}
		if (pME != -1){ //Si pME == -1 -> La solución no se puede mejorar respecto al esfuerzo
			for (j = 0; j < lf.getLong(); j++){ //Busca el requisito no usado con el mínimo esfuerzo sin empeorar la satisfacción
				if (newInd->X[j] == 0){
					if (efor[j] < efor[pME] && satis[j] >= satis[pME]){
						if (pmE != -1){
							if (efor[j] < efor[pmE]) pmE = j;
						}
						else pmE = j;
					}
				}
			}
			if (pmE == -1) //Este requisito no es mejorable, lo intentamos con el siguiente con mayor esfuerzo
				newInd->X[pME] = -1; //Desactivamos temporalmente este requisito para que no influya en la búsqueda de otros requisitos
			else{ //Mejoramos la solución
				newInd->X[pmE] = 1;
				newInd->X[pME] = 0;
				pME = -1; //Para salir del bucle
			}
		}
	}
	while (pME != -1);
	//Restablecemos (ponemos otra vez a "1" todas las posiciones que hemos puesto a "-1" de) la solución antes de terminar
	for (j = 0; j < lf.getLong(); j++)
		if (newInd->X[j] == -1) newInd->X[j] = 1;

	return pmE; //Valdrá -1 sólo si la solución no se puede mejorar respecto al esfuerzo
}



int calculos::domina(individuo *a,individuo *b) {
        int flag1,flag2;
        flag1=flag2=0;

        if (a->E < b->E) flag1=1;
        else if (a->E > b->E) flag2=1;

        if (a->S > b->S) flag1=1;
        else if (a->S < b->S) flag2=1;

    if (flag1 == 1 && flag2 == 0) return 1;
    if (flag1 == 0 && flag2 == 1) return -1;
    return 0;
}

void calculos::imprimir(){
	ofstream all;
	//all.open( "CHARCOS.txt", ios::ate);
	//Mostrar poblacion de los charcos
	for(int i = 0; i < nCharcos; i++){
			cout << "charco nº:"<< i << endl << endl;
		for(int j = 0; j < sizeCharco; j++){
			cout << charcos[i].inds[j].S  << ";" << charcos[i].inds[j].E << " - ";

			for(int k = 0; k < lf.getLong(); k++){
				cout << charcos[i].inds[j].X[k] << " ";
			}
			//all << "rank: " << charcos[i].inds[j].rank << endl << endl;
			cout << endl;
		}
			
	}

	//all.close();
}

void calculos::imprimirP(){
	//Mostrar poblacion
	int i, j;
	for(i = 0; i < population; i++){
		cout << poblacion[i].E << ";" << poblacion[i].S << ";";
		for(j = 0; j < lf.getLong(); j++){
			cout << poblacion[i].X[j] << " ";
		}
		cout << endl;
		//cout << poblacion[i].rank << endl << endl;
	}
}

void calculos::imprimirI(individuo *ind){
	cout << ind->S << ";" << ind->E << " - ";
	for(int i = 0; i < lf.getLong(); i++){
		cout << ind->X[i] << " ";
	}
	cout << endl;
}

/* Calculate lower and upper bounds for the different objective functions */
void calculos::calculate_bounds(int num_sol, individuo *listInd) {
	int i;

	min_efort=max_efort=listInd[0].E;
	min_satis=max_satis=listInd[0].S;

	for (i=1;i < num_sol;i++) {
		if (listInd[i].E > max_efort) max_efort=listInd[i].E;
		else
			if (listInd[i].E < min_efort) min_efort=listInd[i].E;

		if (listInd[i].S > max_satis) max_satis=listInd[i].S;
		else
			if (listInd[i].S < min_satis) min_satis=listInd[i].S;
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



void calculos::rank_crowding(int num_sol, individuo *listInd) {
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
			domination = domina(&listInd[p],&listInd[q]); 	
			if (domination == 1) {					// si p domina a q
				S[p][S_size[p]]=q;					// Sp incluye a q en el grupod de las que domina
				S_size[p]++;
			}
			else
				if (domination == -1) n[p]++; 		// si q domina a p
		}

		if (n[p] == 0) {							// si np es igual a cero, que es lo mismo que: no ha sido dominada por nadie,
			listInd[p].rank=1;						// se incluye en el frente de pareto 1 
			rank[rank_pos]=p;			
			rank_pos++;
			ranks_size[1]++;			
		}
	}

	p=0;
	i=1;
	while (ranks_size[i] != 0) {					// mientras que el frente de pareto "i" no esté vacio 
		ranks_size [i+1]=0;							// nos vamos al siguiente frente de pareto y lo inicializamos
		ini=p;					
		for (;p < ini+ranks_size[i];p++) {			// ya está inicializada p; mientras haya p en ESE frente de pareto
			int rp=rank[p];					
			for (q=0;q < S_size[rp];q++) {			// mientras haya q en el conjunto de dominadas de Sp
				n[S[rp][q]]--;				
				if (n[S[rp][q]] == 0) {				// si a q no le domina nadie
					listInd[S[rp][q]].rank=i+1;		
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
	calculate_bounds(num_sol, listInd);
	for (p=0;p < num_sol;p++) {
		copy_sol[p]=listInd[rank[p]]; //copia la poblacion de individuos
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
				copy_sol[p].crowding +=(copy_sol[p+1].S - copy_sol[p-1].S)/(max_satis - min_satis);

		qsort(&copy_sol[ini+1],ranks_size[i],sizeof(individuo),compare_crowding); //por último volvemos a ordenar
		p=ini+1+ranks_size[i];
	}

	for (p=0;p < num_sol;p++) listInd[p]=copy_sol[p]; // ponemos nuestra solución en la original
}

calculos::~calculos() {
	delete(efor);
	delete(satis);
	for(int i = 0; i < nCharcos ; i++){
		delete(charcos[i].inds);
	}
	delete(charcos);
	delete(poblacion);
}
 /* namespace std */