calculos::calculos() {
	population = 40;
	nCharcos = 4;
	sizeCharco = population/nCharcos;
	evCharco = 25;
	evaluaciones = 10000 / (nCharcos * evCharco);
	aprender = 75;
	prob_mut = 20;

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

calculos::~calculos() {
	int i;
	delete(satis);

	for(i = 0; i < nCharcos; i++){
		for (int j = 0; j < sizeCharco; j++) delete(charcos[i].inds[j].X);
		delete(charcos[i].inds);
	}
	delete(charcos);

	for(i = 0; i < population; i++) delete(poblacion[i].X);
	delete(poblacion);
}

void calculos::repartir(){
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

void calculos::reagrupar(){
	int a;
	a=0;
	for(int i = 0; i < nCharcos; i ++){
		for(int j = 0; j < sizeCharco; j++){
			change(&poblacion[a],&charcos[i].inds[j]);
			a++;
		}
	}
}

int calculos::compare_crowding(const void *a, const void *b) {
	individuo *orderA = (individuo *)a;
	individuo *orderB = (individuo *)b;
	if (orderA->crowding > orderB->crowding) return -1;
	if (orderA->crowding < orderB->crowding) return 1;
	return 0;
}

filtrado_hv::~filtrado_hv(){
	for (int i = 0; i < nsol; i++){
		delete(soluciones[i].X);
		delete(solR[i].X);
	}
	
	delete(soluciones);
	delete(solR);	
}

leerfich::~leerfich() {
	delete(esfuerzo);
	delete(w);
	delete(cond);
	
	for (int i = 0; i < length; i++) delete(cl[i]);
	delete(cl);
}
