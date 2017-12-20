En "calculos.h" crear la variable:
int limiteReq;

int calculos::compare(const void *a, const void *b){
	return ( *(int*)a - *(int*)b );
}

calculos::calculos() {

	population = 40;
	nCharcos = 5;
	sizeCharco = population/nCharcos;
	evCharco = 1;
	evaluaciones = 10000 / (nCharcos * evCharco);
	aprender = 75;
	prob_mut = 20;

	limiteEsfuerzo = (double)(lf.getmaxE() * lf.getlimE())/100;

	sol_log = 0;

	satis= new int[lf.getLong()];
	lf.calcualrSa(satis);
	efor = lf.getEsfuerzo();

	int *efor_aux = new int[lf.getLong()];
	
	for (int i = 0; i < lf.getLong(); i++)
		efor_aux[i] = efor[i];
	
	qsort(efor_aux,lf.getLong(),sizeof(int),compare);
	
	int totE = 0;
	for (limiteReq = 0; (limiteReq < lf.getLong) && (totE+efor_aux[limiteReq] <= limiteEsfuerzo); limiteReq++)
		totE += efor_aux[limiteReq];
	
	delete [] efor_aux;
	
	poblacion = new individuo[population];
	charcos = new memeplex[nCharcos];
	for(int i = 0; i < nCharcos; i++){
		charcos[i].inds = new individuo[sizeCharco];
		for (int j = 0; j < sizeCharco; j++) charcos[i].inds[j].X = new int[lf.getLong()];
	}	
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
	for (i = 0; i < lf.getLong(); i++){
		fitness[i].req = 0;
		fitness[i].fit = 0.0;
	}

	for (i = 0; i < lf.getLong(); i++) {
		fitness[i].req = i;
		satis_norm = (satis[i] - min_satis_req)/(double)(max_satis_req - min_satis_req);
		efor_norm = (efor[i] - min_efor_req)/(double)(max_efor_req - min_efor_req);
		fitness[i].fit = satis_norm / efor_norm;
	}
	
	//ordenar los requisitos por mejora "intuida" (los de mayor "fit" primero)
	qsort(fitness,lf.getLong(),sizeof(tipo_fitness),compare_fitness);
	
	//límite o número máximo de soluciones conscientes del problema (problem aware) generadas
	lim = limiteReq;
	if (lim < population) {
		//generar un individuo sin requisitos
		generarProblemAware(&poblacion[0],0);
	
		//generar "lim" soluciones conscientes del problema (problem aware)
		for (i = 1; i <= lim; i++)
			generarProblemAware(&poblacion[i],i);
		
		//generar el resto de soluciones de forma aleatoria
		for (i = lim+1; i < population; i++)
			generarRand(&poblacion[i]);
	}
	else {
		int totE = 0;
		double dec;
		
		//generar un individuo sin requisitos
		generarProblemAware(&poblacion[0],0);
		
		for (i = 0; (i < lf.getLong()) && (totE+efor[fitness[i].req] <= limiteEsfuerzo); i++)
			totE += efor[fitness[i].req];
		
		lim = i;
		if (lim > (population-1)) {
			dec = (double)(lim)/(population-1);
			//generar "population - 1" soluciones conscientes del problema (problem aware)
			for (i = 1; i < population; i++) {
				generarProblemAware(&poblacion[i],lim);
				lim = round(lim - dec);
			}
		}
		else {
			//generar "lim" soluciones conscientes del problema (problem aware)
			for (i = 1; i <= lim; i++)
				generarProblemAware(&poblacion[i],i);
		
			//generar el resto de soluciones de forma aleatoria
			for (i = lim+1; i < population; i++)
				generarRand(&poblacion[i]);
			}
		}
	}
}

void calculos::generarRand(individuo *ind){
	int i, r, nr;
	
	for (i = 0; i < lf.getLong(); i++) { //inicialmente rellenar a 0
		ind->X[i] = 0;
	}
	
	nr = 1 + rand() % limiteReq; //número de requisitos a usar
		
	for (i = 0; i < nr; i++){ //usar requisitos aleatorios
		r = rand() % lf.getLong();
		while (ind->X[r] != 0)
			if (r == lf.getLong()-1) r=0;
			else r++;

		ind->X[r] = 1;
	}
	
	ind->violations = lf.chequear(ind->X);
	calcularSyE(ind); //calcular esfuerzo y satisfacción
}

void calculos::mejorarInd(individuo *peor, individuo *newInd){
	int r1, r2, i;
	
	change(newInd, peor);
			
	if (newInd->num_req == lf.getLong()) r1 = 0;
	else
		if (newInd->num_req == 0) r1 = 2;
		else
			if (newInd->E > limiteEsfuerzo) {
				if (newInd->num_req > limiteReq) r1 = 0;
				else r1 = rand() % 2;	
			}
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
			if (newInd->E > limiteEsfuerzo) r2 = 1;
			else r2 = rand() % 2;
			
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
