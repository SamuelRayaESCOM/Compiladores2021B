#include "ARBOL.h"
//#include "HASH.h"

ARBOLBINARIO crearNodo(TipoDatoPD x){
	ARBOLBINARIO nuevo=(ARBOLBINARIO)malloc(sizeof(ELEMENTODEARBOLBINARIO));
	nuevo->dato=x;
	nuevo->der=nuevo->izq=NULL;
	return nuevo;
}

void nuevoArbol(ARBOLBINARIO *raiz, ARBOLBINARIO Ramaizq, ARBOLBINARIO Ramader,TipoDatoPD x){
	*raiz=crearNodo(x);
	(*raiz)->izq=Ramaizq;
	(*raiz)->der=Ramader;
}

void copiarABB(ARBOLBINARIO A, ARBOLBINARIO *B){
	if(A!=NULL){
		*B=crearNodo(A->dato);
		copiarABB(A->izq,(&(*B)->izq));
		copiarABB(A->der,(&(*B)->der));
	}
}

void reemplazar(ARBOLBINARIO *actual){
	ARBOLBINARIO P=*actual,a=P->izq;
	while(a->der){
		P=a;
		a=a->der;
	}
	(*actual)->dato=a->dato;
	if(a->der==NULL){
		P->izq=a->izq;
	}else{
		P->der=a->izq;
	}
	*actual=a;
}

int Vacio(ARBOLBINARIO aux){
   return aux==NULL;
}

void insertarABB(ARBOLBINARIO *raiz, TipoDatoPD x){
	ARBOLBINARIO padre = NULL;
    ARBOLBINARIO actual = *raiz;
    while(!Vacio(actual) && x!=actual->dato){
		padre=actual;
        if(x<actual->dato){
			actual=actual->izq;
     	}else if(x>actual->dato) 
			actual=actual->der;
    }
    if(!Vacio(actual)){ 
		return;
	}
	if(Vacio(padre)){
   		*raiz = (ARBOLBINARIO)malloc(sizeof(ELEMENTODEARBOLBINARIO));
        (*raiz)->dato=x;
        (*raiz)->izq=(*raiz)->der=NULL;
    }else if(x<padre->dato){
    	actual=(ARBOLBINARIO)malloc(sizeof(ELEMENTODEARBOLBINARIO));
        padre->izq=actual;
        actual->dato=x;
        actual->izq=actual->der=NULL;
    }else if(x>padre->dato){
        actual=(ARBOLBINARIO)malloc(sizeof(ELEMENTODEARBOLBINARIO));
        padre->der=actual;
        actual->dato=x;
        actual->izq=actual->der=NULL;
    }
}

ARBOLBINARIO buscarABB(ARBOLBINARIO raiz, TipoDatoPD x){
	if(raiz->dato==x){
		return raiz;
	}else if(raiz==NULL){
		return raiz;
	}else if(raiz->dato>x){
		buscarABB(raiz->izq,x);
	}else{
		buscarABB(raiz->der,x);
	}
}

int Hoja(ARBOLBINARIO raiz){
   return !raiz->der && !raiz->izq;
}

void eliminarABB(ARBOLBINARIO *raiz, TipoDatoPD x){
	ARBOLBINARIO padre = NULL;
    ARBOLBINARIO actual;
    ARBOLBINARIO nodo;
    int aux;
    actual=*raiz;
    while(!Vacio(actual)){
    	if(x==actual->dato){
        if(Hoja(actual)){ 
        	if(padre){
            	if(padre->der==actual){
			    	padre->der=NULL;
            	}else if(padre->izq==actual){
               		padre->izq=NULL;
				}
			}
			free(actual);
            actual=NULL;
            return;
     	}else{
        	padre = actual;
            if(actual->der){
            	nodo = actual->der;
                while(nodo->izq){
                	padre=nodo;
                    nodo=nodo->izq;
               	}
            }else{
            	nodo=actual->izq;
                while(nodo->der){
                	padre=nodo;
                	nodo=nodo->der;
               	}
            }
            aux=actual->dato;
            actual->dato=nodo->dato;
            nodo->dato=aux;
            actual=nodo;
        }
    	}else{
        	padre=actual;
        	if(x>actual->dato){
				actual=actual->der;
        	}else if(x<actual->dato){
				actual=actual->izq;
			}
		}
	}
}

void compararABB(ARBOLBINARIO A,ARBOLBINARIO B, int *band){
	if(A!=NULL && B!=NULL){
		if(A->dato==B->dato){
			compararABB(A->izq,B->izq,band);
			compararABB(A->der,B->der,band);
		}else if(A->dato!=B->dato){
			*band=0;
		}else if((A!=NULL && B==NULL)||(A==NULL && B!=NULL)){
			*band=0;
		}
	}
}





