En calculos.h usar:

struct individuo {
		int *X;				//resultado de cada individuo
		int S; 				//satisfaccion que causa al realizarse
		int E; 				//esfuerzo que requiere el individuo
		int rank;			//ranking de dominancia
		double crowding;	//distancia crowding
		int violations;     //número de restricciones violadas (no cumplidas)
};

int leerfich::chequear(int individuo[]) {
	int violations = 0;
	
	for (int i = 0; i < condicones; i++) {
		int a = cond[i].primero-1;
		int b = cond[i].segundo-1;
		
		switch (cond[i].signo[0]) {
		case '>':
			if ((individuo[a] == 1) && (individuo[b] == 0)) {
				violations++;
			}
			break;
		case '+':
			if (((individuo[a] == 1) || (individuo[b] == 1)) && (individuo[a] != individuo[b])) {
				violations++;
			}
			break;
		case 'x':
			if ((individuo[a] == 1) && (individuo[b] == 1)) {
				violations++;
			}
			break;
		}
	}
	
	return violations;
}

void calculos::generarRand(individuo *ind){
	int r;
	for(int i = 0; i < lf.getLong(); i++){//crear aleatorios
		r = rand()%2;
		ind->X[i] = r;
	}

	ind->violations = lf.chequear(ind->X);
	calcularSyE(ind);// calcular esfuerzo y satisfacción
}

void calculos::writeFile(ofstream &log, individuo ind){
	
	if (ind.violations != 0){
		lf.reparar(ind.X);
		calcularSyE(&ind);// calcular esfuerzo y satisfacción	
	}
	
	log << ind.S <<";"<< ind.E << ";" << "{";
	for (int i = 0; i < lf.getLong(); ++i){
		log << ind.X[i];
	}
	log << "}" << endl;
	//por cada solución almacenada en el fichero
}

void calculos::change(individuo *a, individuo *b){
	for(int i = 0; i < lf.getLong(); i++){
		a->X[i] = b->X[i];
	}
	a->S = b->S;
	a->E = b->E;
	a->violations = b->violations;
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

			newInd->violations = lf.chequear(newInd->X);
			calcularSyE(newInd); // cambiar
			
			if(domina(newInd, peor) == 1){
				
				change(&charcos[j].inds[sizeCharco-1],newInd);
				
			}else{
				posMG = rand()%3;
				mejorarInd(mejorG[posMG]->X, peor->X, newInd); //mejorar individuo con el mejor global
				
				
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

int calculos::compare_crowding(const void *a, const void *b) {	
	individuo *orderA = (individuo *)a;	
	individuo *orderB = (individuo *)b;

	if (orderA->violations < orderB->violations) return -1;
	if (orderA->violations > orderB->violations) return 1;
	
	if (orderA->crowding > orderB->crowding) return -1;	
	if (orderA->crowding < orderB->crowding) return 1;	
	return 0;
}
