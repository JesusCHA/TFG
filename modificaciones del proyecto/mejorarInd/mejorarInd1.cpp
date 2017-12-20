void calculos::mejorarInd(individuo *peor, individuo *newInd){
	int r1, r2, i;

	change(newInd, peor);
	
	if (newInd->num_req == lf.getLong()) r1 = 0;
	else
		if (newInd->num_req == 0) r1 = 2;
		else r1 = rand() % 3;
		
	switch (r1) {
		case 0: //eliminar un requisito (el que pensamos es peor)
			for (i = lf.getLong() - 1; i >= 0; i--)
				if (newInd->X[fitness[i].req] == 1) {
					newInd->X[fitness[i].req] = 0;
					break;
				}
			break;
		case 1: //cambiar un requisito por otro mejor
			r2 = rand() % 2;
			if (r2 == 0) {
				//Intentamos mejorar la satisfacción, pero si no lo conseguimos intentamos mejorar el esfuerzo
				if (mejorarIndSatis(newInd) == -1) mejorarIndEfor(newInd);				
			}
			else {
				//Intentamos mejorar el esfuerzo, pero si no lo conseguimos intentamos mejorar la satisfacción
				if (mejorarIndEfor(newInd) == -1) mejorarIndSatis(newInd);
			}
			break;
		case 2: //añadir un requisito (el mejor posible)
			for (i = 0; i < lf.getLong(); i++)
				if (newInd->X[fitness[i].req] == 0) {
					newInd->X[fitness[i].req] = 1;
					break;
				}
			break;
	}	
}

En calculos.h, en "struct individuo" incluir el campo: int num_req;

En calculos.h, añadir la variable: tipo_fitness *fitness;

void calculos::calcularSyE(individuo *ind){
	int s = 0, e = 0, r = 0;
	for(int i = 0; i < lf.getLong(); i++){
		if(ind->X[i] == 1){
			s = s + satis[i];
			e = e + efor[i];
			r++;
		}
	}
	ind->S = s;
	ind->E = e;
	ind->num_req = r;
}

void calculos::change(individuo *a, individuo *b){
	for(int i = 0; i < lf.getLong(); i++){
		a->X[i] = b->X[i];
	}
	a->S = b->S;
	a->E = b->E;
	a->violations = b->violations;
	a->num_req = b->num_req;
}

void calculos::generarPobl(){
	individuo *ind;
	int i, lim;
	int max_satis_req, min_satis_req, max_efor_req, min_efor_req;
	double satis_norm, efor_norm;
	
	for (i = 0; i < population; i++) {
		ind = &poblacion[i];
		ind->X = new int[lf.getLong()];//determinar el tamaño de cada individuo
		ind->S = 0;
		ind->E = 0;
		ind->rank = 0;
		ind->crowding = 0;
		ind->violations = 0;
		ind->num_req = 0;
	}
	
	//límite o número máximo de soluciones conscientes del problema (problem aware) generadas
	lim = lf.getLong();
	if (lim > population) lim = population;
	
	//obtener la satisfacción y esfuerzo máximo y mínimo por requisito, para poder normalizar
	max_satis_req = min_satis_req = satis[0];
	max_efor_req = min_efor_req = efor[0];
	for (i = 1; i < lf.getLong(); i++) {
		if (satis[i] > max_satis_req) max_satis_req = satis[i];
		else
			if (satis[i] < min_satis_req) min_satis_req = satis[i];
		
		if (efor[i] > max_efor_req) max_efor_req = efor[i];
		else
			if (efor[i] < min_efor_req) min_efor_req = efor[i];
	}
	
	//este vector nos va a permitir "intuir" qué requisitos nos producen más mejora (una vez normalizados)
	fitness = new tipo_fitness[lf.getLong()];
	for (i = 0; i < lf.getLong(); i++) {
		fitness[i].req = i;
		satis_norm = (satis[i] - min_satis_req)/(double)(max_satis_req - min_satis_req);
		efor_norm = (efor[i] - min_efor_req)/(double)(max_efor_req - min_efor_req);
		fitness[i].fit = satis_norm / efor_norm;
	}
	
	//ordenar los requisitos por mejora "intuida" (los de mayor "fit" primero)
	qsort(fitness,lf.getLong(),sizeof(tipo_fitness),compare_fitness);
	
	//generar "lim" soluciones conscientes del problema (problem aware)
	for (i = 0; i < lim; i++)
		generarProblemAware(&poblacion[i],i+1);
	
	//generar un individuo sin requisitos
	if (lim < population) generarProblemAware(&poblacion[lim],0);
	
	//generar el resto de soluciones de forma aleatoria
	for (i = lim+1; i < population; i++)
		generarRand(&poblacion[i]);
}

calculos::~calculos() {
	int i;

	for(i = 0; i < nCharcos; i++){
		for (int j = 0; j < sizeCharco; j++) delete[] charcos[i].inds[j].X;
		delete[] charcos[i].inds;
	}
	delete[] charcos;

	for(i = 0; i < population; i++){ 
		
		delete [] poblacion[i].X;
		
	}
	delete[] poblacion;

	delete[] satis;
	
	delete[] fitness;
}

void calculos::generarProblemAware(individuo *ind, int pos){
	int i;

	for (i = 0; i < lf.getLong(); i++) {//inicialmente rellenar a 0
		ind->X[i] = 0;
	}

	//rellenar el número de posiciones que se indica con los "pos" mejores requisitos
	for (i = 0; i < pos; i++) {
		ind->X[fitness[i].req] = 1;
	}
	
	ind->violations = lf.chequear(ind->X);
	calcularSyE(ind);//calcular esfuerzo y satisfacción
}

void calculos::mejorar(ofstream &log){

	individuo *newInd;
	newInd = new individuo();
	newInd->X = new int[lf.getLong()];
	newInd->S = 0;
	newInd->E = 0;
	newInd->violations = 0;

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
			
			//mejorarInd(mejor->X, peor->X, newInd); //mejorar individuo con el mejor del charco 
			mejorarInd(peor, newInd);
			
			newInd->violations = lf.chequear(newInd->X);
			calcularSyE(newInd); // cambiar
			
			if(domina(newInd, peor) == 1){
				
				change(&charcos[j].inds[sizeCharco-1],newInd);
				
			}else{
				posMG = rand()%3;
				//mejorarInd(mejorG[posMG]->X, peor->X, newInd); //mejorar individuo con el mejor global
				mejorarInd(peor, newInd);
				
				newInd->violations = lf.chequear(newInd->X);
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
			delete[] auxInd.X;
		}
		//-------------hasta aquí ---	

	}

	/////////////////////////////////////////////
	delete[] newInd->X;
	delete(newInd);	
	/////////////////////////////////////////////
}
