void leerfich::reparar(int individuo[]) {
	int j;
	
	for (j = condicones-1; j >= 0 && cond[j].signo == "+"; j--) {
		calcularS(cond[j].signo, cond[j].primero-1, cond[j].segundo-1, individuo);
	}
	for (int i = 0; i <= j; i++) {
		calcularS(cond[i].signo, cond[i].primero-1, cond[i].segundo-1, individuo);
	}
}

//desglosa el array para poder aplicar las condiciones
void leerfich::calcularS(string signo, int a, int b, int individuo[]) {
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
