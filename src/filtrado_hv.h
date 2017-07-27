//============================================================================
// Name        : filtrado_hv.h
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================
#ifndef FILTRADOHV_H
#define FILTRADOHV_H 

#include "calculos.h"

namespace std{

	class filtrado_hv{
		int j;

	public:
		filtrado_hv();
		void rank(int num_sol,int sort);
		~filtrado_hv();
	};

} /*namespace std*/

#endif