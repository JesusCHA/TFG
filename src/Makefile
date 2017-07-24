tfg: main.o calculos.o craw_rank.o leer_fichero.o
	g++ -o tfg main.o calculos.o craw_rank.o leer_fichero.o 

main.o: main.cpp
	g++ -c main.cpp

calculos.o: calculos.cpp calculos.h
	g++ -c calculos.cpp

craw_rank.o: craw_rank.cpp craw_rank.h
	g++ -c craw_rank.cpp

leer_fichero.o: leer_fichero.cpp leer_fichero.h
	g++ -c leer_fichero.cpp

clean: rm *.o tfg

