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

int calculos::compare_crowding(const void *a, const void *b) {
	individuo *orderA = (individuo *)a;
	individuo *orderB = (individuo *)b;
	if (orderA->crowding > orderB->crowding) return -1;
	if (orderA->crowding < orderB->crowding) return 1;
	return 0;
}

leerfich::~leerfich() {
	delete(esfuerzo);
	delete(w);
	delete(cond);
	
	for (int i = 0; i < length; i++) delete(cl[i]);
	delete(cl);
}
