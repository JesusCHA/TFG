En "leer_fichero.h" añadir la variable:
int limE;

void leerfich::leer_maximos(fstream &data) {
	string frase;

	getline(data, frase, '\n');
	limE = atoi(frase.c_str());
	
	getline(data, frase, ' ');	
	maxS = atoi(frase.c_str());
	getline(data, frase, '\n');
	maxE = atoi(frase.c_str());
}

int leerfich::getlimE() {
	return limE;
}

void leerfich::reparar(int individuo[], int intentos) {
	int j;
	
	for (j = condicones-1; j >= 0 && cond[j].signo == "+"; j--) {
		calcularS(cond[j].signo, cond[j].primero-1, cond[j].segundo-1, individuo, intentos);
	}
	for (int i = 0; i <= j; i++) {
		calcularS(cond[i].signo, cond[i].primero-1, cond[i].segundo-1, individuo, intentos);
	}
}

//desglosa el array para poder aplicar las condiciones
void leerfich::calcularS(string signo, int a, int b, int individuo[], int intentos) {
	int posible;
	
	switch (signo[0]) {
	case '>':
		if ((individuo[a] == 1) && (individuo[b] == 0)) {
			posible = 1;
			for (int i = condicones-1; i >= 0 && cond[i].signo == "+"; i--) {
				if ((cond[i].primero == b+1) || (cond[i].segundo == b+1)) {
					posible = 0;
					break;
				}
			}
			
			if (intentos == 3) posible = 0;
			
			if (posible == 1) {
				individuo[b] = 1;
			}
			else {
				individuo[a] = 0;
			}
		}
		break;
	case '+':
		if (((individuo[a] == 1) || (individuo[b] == 1)) && (individuo[a] != individuo[b])) {
			individuo[a] = individuo[b] = 0;
		}
		break;
	case 'x':
		if (individuo[a] == 1) {
			individuo[b] = 0;
		} else if (individuo[b] == 1) {
			individuo[a] = 0;
		}
		break;
	default:
		break;
	}
}

En "calculos.h" añadir la variable:
double limiteEsfuerzo;

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

	poblacion = new individuo[population];
	charcos = new memeplex[nCharcos];
	for(int i = 0; i < nCharcos; i++){
		charcos[i].inds = new individuo[sizeCharco];
		for (int j = 0; j < sizeCharco; j++) charcos[i].inds[j].X = new int[lf.getLong()];
	}
}


void calculos::mejorarInd(individuo *peor, individuo *newInd){
	int r1, r2, i;
	
	change(newInd, peor);
			
	if (newInd->num_req == lf.getLong()) r1 = 0;
	else
		if (newInd->num_req == 0) r1 = 2;
		else
			if (newInd->E > limiteEsfuerzo) r1 = rand() % 2;
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

void calculos::writeFile(ofstream &log, individuo ind){
	int i, intentos = 1;
	
	do {
		while (ind.E > limiteEsfuerzo) {
			for (i = lf.getLong() - 1; i >= 0; i--)
				if (newInd->X[fitness[i].req] == 1) {
					newInd->X[fitness[i].req] = 0;
					ind.S -= satis[fitness[i].req];
					ind.E -= efor[fitness[i].req];
					ind.num_req--;
					break;
				}
		}
				
		if (ind.violations != 0){
			lf.reparar(ind.X, intentos);
			calcularSyE(&ind);// calcular esfuerzo y satisfacción	
		}
		
		intentos++;
	}
	while (ind.E > limiteEsfuerzo);
	
	log << ind.S <<";"<< ind.E << ";" << "{";
	for (i = 0; i < lf.getLong(); ++i){
		log << ind.X[i];
	}
	log << "}" << endl;
	//por cada solución almacenada en el fichero
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
	if (lim > population) lim = population-1;
	
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
	
	//generar "lim" soluciones conscientes del problema (problem aware)
	for (i = 0; i < lim; i++)
		generarProblemAware(&poblacion[i],i+1);
	
	//generar un individuo sin requisitos
	if (lim < population) generarProblemAware(&poblacion[lim],0);
	
	//generar el resto de soluciones de forma aleatoria
	for (i = lim+1; i < population; i++)
		generarRand(&poblacion[i]);
}
