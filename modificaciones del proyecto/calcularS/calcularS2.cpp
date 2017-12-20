void leerfich::calcularS(string signo, int a, int b, int individuo[]) {  
	switch (signo[0]) {
	case '>':
		if (individuo[a] == 1) {
			individuo[b] = 1;	
		}
		break;
	case '+':
		if (individuo[a] == 1) {
			if (individuo[b] == 0) {
				individuo[b] = 1;	
				for (int i = 0; i < condicones; i++) {
					if ((cond[i].signo[0] == '>') && (cond[i].primero == b+1)){
							individuo[cond[i].segundo-1] = 1;
					}
				}
			}
		} else if (individuo[b] == 1) {
			if (individuo[a] == 0) {
				individuo[a] = 1;	
				for (int i = 0; i < condicones; i++) {
					if ((cond[i].signo[0] == '>') && (cond[i].primero == a+1)){
							individuo[cond[i].segundo-1] = 1;
					}
				}
			}
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