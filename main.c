#include "clustering.h"

int main(int argc, char **argv){

	if (argc != 2) {
		printf("\nFormato: main <nombre_fich_configuracion>\n\n");
		abort();
	}
	
	Input(argc, argv);

	printf("\n------------------------------------------\n");
	printf("\nALGORITMOS INCLUIDOS: \n");
	printf("\n	1. BL		-------->	 1");
	printf("\n	2. BMB		-------->	 2");
	printf("\n	3. GRASP	-------->	 3");
	printf("\n	4. ILS		-------->	 4");
	printf("\n	5. KM		-------->	 5");
	printf("\n	6. KMM		-------->	 6");
	printf("\n------------------------------------------\n");
	
	int algoritmo;
	printf("\nIntroduzca el numero de algoritmo a usar: ");
	scanf("%d", &algoritmo);

	if (algoritmo == 1) {
		printf("\nAlgoritmo seleccionado: BUSQUEDA LOCAL\n");	
		printf("ejecutando...");
		
		clock_t start_time = clock();
		
		solucionInicial();
		double mejor_objetivo = BL();
		
		double tiempo = clock() - start_time;	

		printf("\n\n\nDe las 25 ejecuciones, el mejor resultado es: %f", mejor_objetivo);
		printf("\nTiempo de ejecucion: %f", tiempo/CLOCKS_PER_SEC);	
		printf("\nFin del algoritmo.\n\n");
	}
	else if (algoritmo == 2)
		BMB();
	else if (algoritmo == 3)
		GRASP();
	else if (algoritmo == 4)
		ILS();
	else if (algoritmo == 5) {
		printf("\nAlgoritmo seleccionado: KMEDIAS\n");	
		printf("ejecutando...");
		
		clock_t start_time = clock();
		
		double mejor_objetivo = KM();
		
		double tiempo = clock() - start_time;	

		printf("\n\n\nDe las 25 ejecuciones, el mejor resultado es: %f", mejor_objetivo);
		printf("\nTiempo de ejecucion: %f", tiempo/CLOCKS_PER_SEC);	
		printf("\nFin del algoritmo.\n\n");
	}
	else if (algoritmo == 6)
		KMM();
	else
		printf ("\nAlgoritmo no registrado. El programa finaliza...\n\n");
		
	liberarMemoria();
		
	return (0);
}