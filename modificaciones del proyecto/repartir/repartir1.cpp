void calculos::repartir() {
	int j, a, inc_dec;

	j = a = 0;
	inc_dec = +1;
	for (int i = 0; i < population; i++) {
		change(&charcos[j].inds[a],&poblacion[i]);
		if ((j+inc_dec) == nCharcos) {
			inc_dec = -1;
			a++;
		}
		else {
			if ((j+inc_dec) == -1) {
				inc_dec = +1;
				a++;
			}
			else {
				j += inc_dec;	
			}
		}
	}
}
