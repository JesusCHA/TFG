void calculos::repartir() {
	int j,a;
	j=a=0;
	for(int i = 0; i < population; i++){
		change(&charcos[j].inds[a],&poblacion[i]);
		if(j+1 == nCharcos ){
			j=0;
			a++;
		}else{
			j++;
		}
	}
}

Añadir en leer_fichero.h las variables:
int maxS;
int maxE;

void leerfich::body() {
	fstream data;
	string nombre;
	string frase;

	data.open("../data_set1.txt");
	if (data.is_open()) {
		leer_maximos(data);
		leer_clientes(data);
		getline(data, frase, '\n');
		length = atoi(frase.c_str());
		inicializar();
		if (!data.eof()) {
			leer_recursos(data);
			leer_condiciones(data);
		}
		data.close();
	}
}

void leerfich::leer_maximos(fstream &data) {
	string frase;

	getline(data, frase, ' ');	
	maxS = atoi(frase.c_str());
	getline(data, frase, '\n');
	maxE = atoi(frase.c_str());
}

int leerfich::getmaxS() {
	return maxS;
}

int leerfich::getmaxE() {
	return maxE;
}

filtrado_hv::filtrado_hv() {
	nsol = 0;
	nFich = "";

	max_satis = lf.getmaxS();
	max_efort = lf.getmaxE();
}

Añadir en el fichero calculos.h la siguiente estructura:
struct tipo_fitness {
		int req;			//número de requisito
		double fit;			//fitness asociado
		};

void calculos::generarPobl() {
	individuo *ind;
	int i, lim;
	tipo_fitness *fitness;
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
		generarProblemAware(&poblacion[i],i+1,fitness);
	
	//generar un individuo sin requisitos
	if (lim < population) generarProblemAware(&poblacion[lim],0,fitness);
	
	delete[] fitness;
	
	//generar el resto de soluciones de forma aleatoria
	for (i = lim+1; i < population; i++)
		generarRand(&poblacion[i]);
}

int calculos::compare_fitness(const void *a, const void *b) {	
	tipo_fitness *orderA = (tipo_fitness *)a;	
	tipo_fitness *orderB = (tipo_fitness *)b;	
	if (orderA->fit > orderB->fit) return -1;	
	if (orderA->fit < orderB->fit) return 1;	
	return 0;
}

void calculos::generarProblemAware(individuo *ind, int pos, tipo_fitness *fitness){
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