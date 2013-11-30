#include "clustering.h"

#define max_evaluaciones 10000

void solucionInicial() {
	int i;
 	
 	for (i = 0; i < k_inicial; i++)
 		Tam[i] = 0;
 
 	for (i=0;i<n_datos;i++) {
    	Conf[i]=Randint(0,k_inicial-1);
    	Tam[Conf[i]]++;
    }
    	
}

void calculoInicialCentroides() {
	int i, j, k;

	for (k=0;k<k_inicial;k++)
		for (j=0;j<n_variables;j++)
	   		Centros[k][j]=0.0;

	for (k=0;k<k_inicial;k++) {
		for (i=0;i<n_datos;i++){
	    	if (Conf[i]==k) {
	      		for (j=0;j<n_variables;j++)
	       			Centros[k][j] += tabla[i][j];
	      	}
		}

		for (j=0;j<n_variables;j++)
	    	Centros[k][j] /= (double)Tam[k];
	}	
}

void liberarMemoria() {
	
	int i;
	
	for (i=0;i<n_datos;i++)
  		free (tabla[i]);
  		
  	free (tabla);

  	free (Conf);
	free (Conf_mutado);
	free (Seleccionados);
	free (Ganancia);
	free (LRC);
 
	for (i=0;i<k_inicial;i++)
  		free (Centros[i]);
  		
 	free (Centros);
 
	free(Tam);	
}

void recalculoCentroides(int cluster1, int cluster2, int i) {
	int l;
	for (l = 0; l < n_variables; l++) {
		Centros[cluster1][l] = ((Tam[cluster1] * Centros[cluster1][l]) - tabla[i][l]) / (Tam[cluster1] - 1);
	 	Centros[cluster2][l] = ((Tam[cluster2] * Centros[cluster2][l]) + tabla[i][l]) / (Tam[cluster2] + 1);
	 }
}

double distanciaPatronCentroide(double *x, double *y) {
	int i;
 	double suma = 0.0;
 
	for (i=0;i<n_variables;i++)
  		suma += (x[i] - y[i])*(x[i] - y[i]);

	return (sqrt(suma));
 	return (suma);
}

double objetivoInicial() {
	int i;
	double suma = 0.0;
	 
	for (i=0;i<n_datos;i++)
	  	suma += distanciaPatronCentroide(tabla[i],Centros[Conf[i]]);

	return (suma*suma/(double)n_datos);	
	//return suma;
}

void aumentarTamanio(int cluster) {
	Tam[cluster]++;
}

void disminuirTamanio(int cluster) {
	Tam[cluster]--;
}

double objetivoActual() {
	int i;
	double suma = 0.0;
	 
	for (i=0;i<n_datos;i++)
	  	suma += distanciaPatronCentroide(tabla[i],Centros[Conf[i]]);

	return (suma*suma/(double)n_datos);
}

double factorizacionObjetivo(int clusterOrigen, int clusterDestino, int patron) {
	
	double fact=0;

	double x1 = (1.0*Tam[clusterOrigen])/(1.0*Tam[clusterOrigen]-1.0);
	double x2 = distanciaPatronCentroide(tabla[patron],Centros[clusterOrigen]);

	double y1 = (1.0*Tam[clusterDestino])/(1.0*Tam[clusterDestino]+1.0);
	double y2 = distanciaPatronCentroide(tabla[patron],Centros[clusterDestino]);

	fact = (y1*y2)-(x1*x2);
	return fact;		
}

int clusterMasCercano(double *patron) {
	double dist_minima, dist_act;
	int k, cluster;
 
 	dist_minima=10E38;
 	for (k=0;k<k_inicial;k++) {
   		dist_act=distanciaPatronCentroide(patron,Centros[k]);
   		if (dist_act<dist_minima) {
     		cluster=k;
     		dist_minima=dist_act;
     	}
   	}
 	return cluster;
}

int seleccionado (int p, int n_centros) {
 	int sel;
 	int k;
 
 	sel=0; k=0;
 	do {
   		if (Seleccionados[k]==p)
    		sel=1;
   		else
    		k++;

   	} while (!sel && k<n_centros);
 
 	return sel;
}

int crearLRC(double umbral, int n_centros) {
	int i = 0, tam_LRC = 0;
	
	for (i=0; i<n_datos; i++) {
    	if (!seleccionado(i,n_centros-1) && (Ganancia[i] > umbral)) {
			LRC[tam_LRC] = i;
			tam_LRC++;
		}
	}
	
	return tam_LRC;
}

int seleccionLRC(int tam_LRC) {
	int nuevo_centro;
	
	nuevo_centro = LRC[Randint(0, tam_LRC-1)];
	
	return nuevo_centro;
}

void solucionInicialGreedy() {
	int i, j, k, k2, nuevo_centro;
	double dist_minima, dist_act, valor, mejor_ganancia, peor_ganancia;
	
	for (j=0; j<n_variables; j++){
		Centros[1][j]=0.0;
	    for (i=0; i<n_datos; i++)
	    	Centros[1][j] += tabla[i][j];
	}

	for (j=0; j<n_variables; j++)
		Centros[1][j] /= (double) n_datos;

	dist_minima=10E38;
	    
	for (i=0; i<n_datos; i++) {
		dist_act= distanciaPatronCentroide(tabla[i],Centros[1]);
	    if (dist_act<dist_minima) {
	    	nuevo_centro=i;
	        dist_minima=dist_act;
	   	}
	}

	for (j=0; j<n_variables; j++)
	     Centros[0][j] = tabla[nuevo_centro][j];
	
	Seleccionados[0]=nuevo_centro;

	for (k=1;k<k_inicial;k++) {
		for (i=0; i<n_datos; i++) {
	       	if (seleccionado(i,k-1))
	        	Ganancia[i]=-1.0;
	       	else {
	         	Ganancia[i]=0.0;
	         	for (j=0; j<n_datos; j++) {
	          		if (!seleccionado(j,k-1) && i!=j) {
	            		dist_minima=10E38;
	            		for (k2=0;k2<k;k2++) {
	              			dist_act= distanciaPatronCentroide(tabla[j],Centros[k2]);
	              			if (dist_act<dist_minima)
	               				dist_minima=dist_act;
	              		}

	            		valor=dist_minima - distanciaPatronCentroide(tabla[j],tabla[i]);
	            		if (valor>0)
	             			Ganancia[i] += valor;
	            	}
				}
	  		}
		}
		
		mejor_ganancia = -1.0;
		peor_ganancia = 10.0e30;
		
	    for (i=0; i<n_datos; i++) {
	    	if (!seleccionado(i,k-1)) {
		
				if (Ganancia[i] > mejor_ganancia)
		           mejor_ganancia = Ganancia[i];
		
				if (Ganancia[i] < peor_ganancia)
					peor_ganancia = Ganancia[i];
	       	}
		}
	
		double umbral = mejor_ganancia - 0.3 * (mejor_ganancia - peor_ganancia);

		int tam_LRC = crearLRC(umbral, k);
		nuevo_centro = seleccionLRC(tam_LRC);

     	Seleccionados[k]=nuevo_centro;
     	for (j=0; j<n_variables; j++)
      		Centros[k][j] = tabla[nuevo_centro][j];
	}

	for (i = 0; i < k_inicial; i++)
		Tam[i] = 0;
	   
 	for (i=0; i<n_datos; i++) {
	    Conf[i]=clusterMasCercano(tabla[i]);
		Tam[Conf[i]]++;
	}
		
}

void mutacionSolucion() {
	int i, cluster_aleatorio;
	double prob_mutacion = 0.5, aleatorio;
	
	for (i = 0; i < n_datos; i++)
		Conf_mutado[i] = Conf[i];
		
	for (i = 0; i < n_datos; i++){
		cluster_aleatorio = Randint(0,k_inicial-1);
	
		aleatorio = Rand();

		if (aleatorio < prob_mutacion) {
			disminuirTamanio(Conf[i]);
			aumentarTamanio(cluster_aleatorio);
			Conf[i] = cluster_aleatorio;
		}
	}
	
}

void solucionAnterior() {
	int i;
	
	for (i = 0; i < k_inicial; i++)
 		Tam[i] = 0;
	
	for (i = 0; i < n_datos; i++) {
		Conf[i] = Conf_mutado[i];
		Tam[Conf[i]]++;
	}
}

double criterioAceptacion(double objetivo_actual, double objetivo_mutado) {
	double aceptado;
	
	if (objetivo_actual > objetivo_mutado)
		aceptado = objetivo_mutado;
	else
		aceptado = objetivo_actual;
	
	return aceptado;
}

void calculaCentroidesKM() {
 	int i, j, k, n_elementos_cluster;
	double k_actual;
 	
	for (k=0;k<k_inicial;k++)
  		for (j=0;j<n_variables;j++)
   			Centros[k][j]=0.0;
   
 	k_actual = k_inicial;

 	for (k=0;k<k_inicial;k++) {
   		n_elementos_cluster=0;

   		for (i=0;i<n_datos;i++)
    		if (Conf[i]==k) {
      			n_elementos_cluster++;
      			for (j=0;j<n_variables;j++)
       				Centros[k][j] += tabla[i][j];
      		}
   
   			if (n_elementos_cluster==0) {
     			k_actual--;
     
				for (j=0;j<n_variables;j++)
      				Centros[k][j] = 10E38;
     		}
   			else
    			for (j=0;j<n_variables;j++)
     			Centros[k][j] /= (double)n_elementos_cluster;
   }
}

double BL() {
	
	calculoInicialCentroides();	
	
	int evaluaciones = 0, i, j, k;
	int cambio = 1, cluster;
	double min, A, B, coste, objetivo_vecino;
	
	double mejor_objetivo = objetivoInicial();
	
	while ((cambio == 1) && (evaluaciones <= max_evaluaciones)) {
		cambio = 0;
		
		for (i = 0; i < n_datos; i++) {

			A = (distanciaPatronCentroide(tabla[i],Centros[Conf[i]])*distanciaPatronCentroide(tabla[i],Centros[Conf[i]]));
				
			min = 10e30;
			cluster = i;
				
			for (k = 0; k < k_inicial; k++) {
				if (k != Conf[i]) {
					B = (distanciaPatronCentroide(tabla[i],Centros[k])*distanciaPatronCentroide(tabla[i],Centros[k]));
					if (B < min) {
						cluster = k;
						min = B;
					}
				}	
			}
				
			coste = min - A;

			if (Tam[Conf[i]] > 1) {
				if ((cluster != Conf[i]) && (coste < 0)) {
					
					recalculoCentroides(Conf[i], cluster, i);
					disminuirTamanio(Conf[i]);
					aumentarTamanio(cluster);
					//objetivo_vecino = objetivo_vecino + factorizacionObjetivo(Conf[i], cluster, i);
						
					Conf[i] = cluster;

					objetivo_vecino = objetivoActual();
					evaluaciones++;
					
					if (mejor_objetivo > objetivo_vecino) {
						mejor_objetivo = objetivo_vecino;
						cambio = 1;
					}
				}
			}		
		}	
	}
	
	return mejor_objetivo;
}

void BMB(){
	
	printf("\nAlgoritmo seleccionado: BUSQUEDA MULTIARRANQUE BASICA\n");	
	printf("ejecutando...");
	
	int i;
	double mejor_objetivo = 10e30, objetivo_actual;
	
	clock_t start_time = clock();
	
	for (i = 0; i < 25; i++) {
		solucionInicial();
		objetivo_actual = BL();

		if (objetivo_actual < mejor_objetivo)
			mejor_objetivo = objetivo_actual;
	}
	
	double tiempo = clock() - start_time;	
	
	printf("\n\n\nDe las 25 ejecuciones, el mejor resultado es: %f", mejor_objetivo);
	printf("\nTiempo de ejecucion: %f", tiempo/CLOCKS_PER_SEC);	
	printf("\nFin del algoritmo.\n\n");
		
}

void GRASP(){
	printf("\nAlgoritmo seleccionado: GRAPS\n");	
	printf("ejecutando...");

	int i;
	double mejor_objetivo = 10e30, objetivo_actual;
	
	clock_t start_time = clock();
	
	for (i = 0; i < 25; i++) {
		solucionInicialGreedy();
		objetivo_actual = BL();

		if (objetivo_actual < mejor_objetivo)
			mejor_objetivo = objetivo_actual;
	}
	
	double tiempo = clock() - start_time;	
	
	printf("\n\n\nDe las 25 ejecuciones, el mejor resultado es: %f", mejor_objetivo);
	printf("\nTiempo de ejecucion: %f", tiempo/CLOCKS_PER_SEC);	
	printf("\nFin del algoritmo.\n\n");
		
}

void ILS(){
	printf("\nAlgoritmo seleccionado: ILS\n");	
	printf("ejecutando...");
	
	clock_t start_time = clock();
	
	solucionInicial();
	
	int i;
	double mejor_objetivo = 10e30, objetivo_mutado;
	double objetivo_actual = BL();

	for (i = 0; i < 24; i++) {
		mutacionSolucion();
		objetivo_mutado = BL();
		objetivo_actual = criterioAceptacion(objetivo_actual, objetivo_mutado);
		
		if (objetivo_actual != objetivo_mutado)
			solucionAnterior();
			
		if (objetivo_actual < mejor_objetivo)
			mejor_objetivo = objetivo_actual;
			
	}
	
	double tiempo = clock() - start_time;	
	
	printf("\n\n\nDe las 25 ejecuciones, el mejor resultado es: %f", mejor_objetivo);
	printf("\nTiempo de ejecucion: %f", tiempo/CLOCKS_PER_SEC);	
	printf("\nFin del algoritmo.\n\n");
	
}

double KM(){
	
	char cambio;
	int ej, i, su_cluster;
	double min, J_act;
	
	min=10E38;
	
	solucionInicial();

   	do {
    	cambio=0;

		calculaCentroidesKM();

	     for (i=0;i<n_datos;i++) {
		
	       	su_cluster=clusterMasCercano(tabla[i]);

	       	if (su_cluster!=Conf[i]) {
	         	cambio = 1;
	         	Conf[i] = su_cluster;
	       	}
	    }

	} while (cambio);

	J_act=objetivoActual();

	if (J_act<min)
	    min=J_act;
		
	return min;
}

void KMM(){
	printf("\nAlgoritmo seleccionado: K-MEDIAS MULTIARRANQUE\n");	
	printf("ejecutando...");
	
	int i;
	double mejor_objetivo = 10e30, objetivo_actual;
	
	clock_t start_time = clock();
	
	for (i = 0; i < 25; i++) {
		objetivo_actual = KM();
		
		if (objetivo_actual < mejor_objetivo)
			mejor_objetivo = objetivo_actual;
	}
	
	double tiempo = clock() - start_time;	
	
	printf("\n\n\nDe las 25 ejecuciones, el mejor resultado es: %f", mejor_objetivo);
	printf("\nTiempo de ejecucion: %f", tiempo/CLOCKS_PER_SEC);	
	printf("\nFin del algoritmo.\n\n");
}