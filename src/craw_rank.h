//============================================================================
// Name        : leer_fichero.h
// Author      : Jesus CHA
// Version     :
// Copyright   : loloTloloUloloPloloM
// Description : Hello World in C++, Ansi-style
//============================================================================
#ifndef CRAWRANK_H
#define CRAWRANK_H 

#include "calculos.h"

namespace std{

	class craw_rank{
		int j;

	public:
		craw_rank();
		void rank(int num_sol,int sort);
		~craw_rank();
	};

} /*namespace std*/

#endif