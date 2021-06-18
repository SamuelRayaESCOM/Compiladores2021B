#ifndef __ARBOL_H
#define __ARBOL_H
#include <stdio.h>
#include <stdlib.h>
//#include "HASH.h"
typedef char TipoDatoPD;

typedef struct elementodearbolbinario{
	TipoDatoPD dato;
	struct elementodearbolbinario *izq,*der;
}ELEMENTODEARBOLBINARIO;

typedef ELEMENTODEARBOLBINARIO *ARBOLBINARIO;

ARBOLBINARIO crearNodo(TipoDatoPD x);
void nuevoArbol(ARBOLBINARIO *raiz, ARBOLBINARIO Ramaizq, ARBOLBINARIO Ramader,TipoDatoPD x);
void copiarABB(ARBOLBINARIO A, ARBOLBINARIO *B);
int Vacio(ARBOLBINARIO aux);
void insertarABB(ARBOLBINARIO *raiz, TipoDatoPD x);
void reemplazar(ARBOLBINARIO *actual);
ARBOLBINARIO buscarABB(ARBOLBINARIO raiz, TipoDatoPD x);
int Hoja(ARBOLBINARIO raiz);
void eliminarABB(ARBOLBINARIO *a, TipoDatoPD dat);
void compararABB(ARBOLBINARIO A, ARBOLBINARIO B, int *band);
//void preorden(ARBOLBINARIO raiz, TABLADISPERSION *tabla);
//void orden(ARBOLBINARIO raiz, TABLADISPERSION *tabla);
//void postoden(ARBOLBINARIO raiz, TABLADISPERSION *tabla);
#endif
