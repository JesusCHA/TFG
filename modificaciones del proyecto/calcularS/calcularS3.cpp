void leerfich::reparar(int individuo[]) {
	for (int i = condicones-1; i >= 0; i--) {
		calcularS(cond[i].signo, cond[i].primero-1, cond[i].segundo-1, individuo);
	}
}

//desglosa el array para poder aplicar las condiciones
void leerfich::calcularS(string signo, int a, int b, int individuo[]) {
	int posible;
	
	switch (signo[0]) {
	case '>':
		if ((individuo[b] == 0) && (individuo[a] == 1)) {
			posible = 1;
			for (int i = condicones-1; i >= 0 && cond[i].signo == '+'; i--) {
				if ((cond[i].primero == a+1) || (cond[i].segundo == a+1)) {
					posible = 0;
					break;
				}
			}
			if (posible == 1){
				individuo[a] = 0;
			}
			else {
				individuo[b] = 1;
			}
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
