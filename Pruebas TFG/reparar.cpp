void leerfich::reparar(int individuo[]){
	for (int i = 0; i < condicones; i++) {

		calcularS(cond[i].signo, cond[i].primero-1, cond[i].segundo-1, individuo);
	}
}

//desglosa el array para poder aplicar las condiciones
void leerfich::calcularS(string signo, int a, int b, int individuo[]) {
	switch (signo[0]) {
	case '>':
		if (individuo[a] == 1) {
			individuo[b] = 1;
		}
		break;
	case '+':
		if (individuo[a] == 1) {
			individuo[b] = 1;
		} else if (individuo[b] == 1) {
			individuo[a] = 1;
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


void leerfich::reparar(int individuo[]){
	for (int i = 0; i < condicones; i++) {
		//IMPORTANTE LEER
		//¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡
		//Si modificamos aquí a +1 o +2, nuestro hipervolen incrementa, en 63'3 y 63'9 respectivamente
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		calcularS(cond[i].signo, cond[i].primero, cond[i].segundo, individuo);
	}
}

//desglosa el array para poder aplicar las condiciones
void leerfich::calcularS(string signo, int a, int b, int individuo[]) {
	switch (signo[0]) {
	case '>':
		if (individuo[a] == 1) {
			individuo[b] = 1;
		}
		break;
	case '+':
		if (individuo[a] == 1) {
			individuo[b] = 1;
		} else if (individuo[b] == 1) {
			individuo[a] = 1;
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
