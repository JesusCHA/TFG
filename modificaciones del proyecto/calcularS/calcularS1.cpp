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