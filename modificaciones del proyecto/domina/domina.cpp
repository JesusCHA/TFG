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

void calculos::mejorar(ofstream &log){

	individuo *newInd;
	newInd = new individuo();
	newInd->X = new int[lf.getLong()];
	newInd->S = 0;
	newInd->E = 0;
	newInd->violations = 0;
	newInd->num_req = 0;

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
				writeFile(log, charcos[j].inds[sizeCharco-1]);// Guardar en el fichero log la solución que hay en charcos[j].inds[sizeCharco-1] 
				sol_log++;
				change(&charcos[j].inds[sizeCharco-1],newInd);				
			}else{
				posMG = rand()%3;
				//mejorarInd(mejorG[posMG]->X, peor->X, newInd); //mejorar individuo con el mejor global
				mejorarInd(peor, newInd);
				
				newInd->violations = lf.chequear(newInd->X);
				calcularSyE(newInd); // cambiar
				k++; //hemos evaluado la s y e una vez más
				
				if(domina(newInd, peor) == 1){
					writeFile(log, charcos[j].inds[sizeCharco-1]);// Guardar en el fichero log la solución que hay en charcos[j].inds[sizeCharco-1] 
					sol_log++;
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

			int g = 2;
			if (domina(mejorG[2],mejorG[1]) == 1) {
				g = 1;
				if (domina(mejorG[1],mejorG[0]) == 1) g = 0;
			}
			else
				if (domina(mejorG[2],mejorG[0]) == 1) g = 0;
													
			mejorG[g] = &charcos[j].inds[pos+1];
			
			delete[] auxInd.X;
		}
		//-------------hasta aquí ---	
	}

	/////////////////////////////////////////////
	delete[] newInd->X;
	delete(newInd);	
	/////////////////////////////////////////////
}
