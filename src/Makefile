tfg: main.o calculos.o filtrado_hv.o leer_fichero.o
	g++ -o tfg main.o calculos.o filtrado_hv.o leer_fichero.o 

main.o: main.cpp
	g++ -c main.cpp

calculos.o: calculos.cpp calculos.h
	g++ -c calculos.cpp

filtrado_hv.o: filtrado_hv.cpp filtrado_hv.h
	g++ -c filtrado_hv.cpp

leer_fichero.o: leer_fichero.cpp leer_fichero.h
	g++ -c leer_fichero.cpp

clean: rm *.o tfg

