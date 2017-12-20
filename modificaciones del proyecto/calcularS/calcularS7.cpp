int calculos::compare_crowding(const void *a, const void *b) {	
	individuo *orderA = (individuo *)a;	
	individuo *orderB = (individuo *)b;
	if (orderA->crowding > orderB->crowding) return -1;	
	if (orderA->crowding < orderB->crowding) return 1;	
	return 0;
}

int calculos::domina(individuo *a,individuo *b) {
    int flag1,flag2;
    flag1=flag2=0;

	if (a->violations < b->violations) return 1;
	if (a->violations > b->violations) return -1;
	if (a->violations != 0) return 0;
		
    if (a->E < b->E) flag1=1;
    else if (a->E > b->E) flag2=1;

    if (a->S > b->S) flag1=1;
    else if (a->S < b->S) flag2=1;

    if (flag1 == 1 && flag2 == 0) return 1;
    if (flag1 == 0 && flag2 == 1) return -1;
    return 0;
}
