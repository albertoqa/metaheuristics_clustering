#include "random.h"
#include <time.h>


TipoConfiguracion Conf, Conf_mutado;
TipoConfiguracion Tam;
TipoCentros Centros;
TTABLA tabla;

int n_datos, n_variables, k_inicial, *Seleccionados, *LRC;
float semilla;
unsigned long Seed;
char fich_datos[100];
double *Ganancia;

void Input (int, char**);
void solucionInicial();
void calculoInicialCentroides();
void liberarMemoria();

double BL(), KM();
void BMB(), GRASP(), ILS(), KMM();

void recalculoCentroides(int c1, int c2, int i), calculaCentroidesKM();
double distanciaPatronCentroide (double *x, double *y);
double objetivoInicial();
void aumentarTamanio(int cluster), disminuirTamanio(int cluster);
int clusterMasCercano(double *patron), seleccionado(int p, int n_centros);
double objetivoActual();
void solucionInicialGreedy();
double criterioAceptacion(double a, double b);
void solucionAnterior(), mutarSolucion();
int crearLRC(double umbral, int n_centros);
int seleccionLRC(int tam_LRC);
double factorizacionObjetivo(int clusterOrigen, int clusterDestino, int patron);