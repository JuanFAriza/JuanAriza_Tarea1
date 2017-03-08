Energia.pdf Tiempo.pdf:	datos.dat datosTiempo.dat
	python Energia.py
	python Tiempo.py
datos.dat datosTiempo.dat: FermiPastaUlamTsingou
	./FermiPastaUlamTsingou 1
	./FermiPastaUlamTsingou 2
	./FermiPastaUlamTsingou 4
FermiPastaUlamTsingou: FermiPastaUlamTsingou.c
	gcc FermiPastaUlamTsingou.c -lm -fopenmp -o FermiPastaUlamTsingou
clean:
	rm datos.dat
	rm datosTiempo.dat
	rm FermiPastaUlamTsingou
