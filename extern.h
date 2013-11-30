#include "random.h"

extern TipoConfiguracion Conf, Conf_mutado;
extern TipoConfiguracion Tam;
extern TipoCentros Centros;
extern TTABLA tabla;

extern int n_datos, n_variables, k_inicial, k_actual, *Seleccionados, *LRC;
    
extern float semilla;

extern unsigned long Seed;

extern char fich_datos[100];

extern void Input (int, char**);

extern double *Ganancia;