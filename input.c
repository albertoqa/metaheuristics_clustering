#include "extern.h"

#define FORMATO_ENT "Fichero de datos = %s \
Numero de datos = %d \
Numero de variables = %d \
Numero de clusters inicial = %d \
Semilla = %f"

#define VAR_ENT  fich_datos,&n_datos,&n_variables,&k_inicial,&semilla

void Input (int argc, char *argv[]) {
 
	int i, j, k;
	char fichero_ent[100], fichero_sal[100];
 	FILE *fp, *ft;

 	/* Se procede a la lectura del fichero de configuracion */
 	
 	sprintf (fichero_ent,"%s",argv[1]);
 	if ((fp=fopen (fichero_ent,"r")) == NULL) {
   		printf ("No se puede abrir el fichero de configuracion %s",fichero_ent);
   		abort ();
   	}
   	
 	fscanf (fp,FORMATO_ENT,VAR_ENT);
 	fclose (fp);

 	Seed=(unsigned long) semilla;

 	/* Creacion de una tabla con los datos del fichero de entrenamiento */
 	if ((ft = fopen(fich_datos, "r")) == NULL) {
   		printf ("No se puede abrir el fichero de datos");
   		abort ();
   	}

 	tabla = (TTABLA) calloc ((unsigned)n_datos,sizeof(double *));
 	if (tabla == NULL) {
   		printf ("Error en Calloc 1");
   		abort ();
   	}

 	for (i=0;i<n_datos;i++) {
   		tabla[i] = (double *) calloc ((unsigned)n_variables,sizeof(double));
   
   		if (tabla[i] == NULL) {
     		printf ("Error en Calloc 2");
     		abort ();
     	}
   	}

 	for (i=0;i<n_datos;i++) {
   		for (j=0;j<n_variables-1;j++)
    		fscanf(ft,"%lf",&tabla[i][j]);
   	fscanf(ft,"%lf\n",&tabla[i][j]);
   	}
 
 	fclose (ft);

 	/* Reserva de memoria para la configuracion de clusters */
 	Conf = (TipoConfiguracion) calloc ((unsigned)n_datos, sizeof(int));
 	if (Conf == NULL) {
   		printf("Error en Calloc 3");
   		abort();
   	}

	Conf_mutado = (TipoConfiguracion) calloc ((unsigned)n_datos, sizeof(int));
 	if (Conf_mutado == NULL) {
   		printf("Error en Calloc 33");
   		abort();
   	}
   
 	/* Reserva de memoria para la estructura que almacena los centros
    de los clusters */
 	Centros = (TipoCentros) calloc ((unsigned)k_inicial,sizeof(double *));
 	if (Centros == NULL) {
   		printf ("Error en Calloc 4");
   		abort ();
   	}

 	for (i=0;i<k_inicial;i++) {
   		Centros[i] = (double *) calloc ((unsigned)n_variables,sizeof(double));
   		if (Centros[i] == NULL) {
     		printf ("Error en Calloc 5");
     		abort ();
     	}
   	}

 	/* Reserva de memoria para los tamaños de los clusters */
 	Tam = (TipoConfiguracion) calloc ((unsigned)k_inicial, sizeof(int));
 	if (Tam == NULL) {
   		printf("Error en Calloc 6");
   		abort();
   	}

	Seleccionados = (int *) calloc ((unsigned)k_inicial,sizeof(int));
	 if (Seleccionados == NULL)
	  {
	   printf ("Error en Calloc 7");
	   abort ();
	   }

	 Ganancia = (double *) calloc ((unsigned)n_datos,sizeof(double));
	 if (Ganancia == NULL)
	  {
	   printf ("Error en Calloc 8");
	   abort ();
	   }
	
	LRC = (int *) calloc ((unsigned)n_datos,sizeof(int));
	 if (LRC == NULL)
	  {
	   printf ("Error en Calloc 9");
	   abort ();
	   }


}