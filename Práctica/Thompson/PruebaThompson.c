/*	Por hacer:

- Aceptar parentesis		
- Imprimir en forma de estados o similar
- Error declarar una variable *		
*/

#include "ARBOL.c"
#include <stdio.h>
#include <string.h>
//_________________________ Sin parentesis por el momento 

char C_aux;

 void AFD(char mensaje[], ARBOLBINARIO raiz){
	if(raiz!=NULL){
		AFD("Nodo Izquierda: ",raiz->izq);
		printf("%s %c\n",mensaje,raiz->dato);
		AFD("Nodo derecho: ",raiz->der);
	}
}


ARBOLBINARIO Proceso(char cadena[],int i, int longt){
	ARBOLBINARIO raiz=NULL, Rizq=NULL, Rder=NULL, Rizq_aux=NULL, raizAux;

	if(/*cadena[i]!='(' ||*/ cadena[i]!='*' || cadena[i]!='|'){
		Rizq=crearNodo(cadena[i]);
		printf("%d", i);
		++i;
	}
	
	while(i<longt){
		if(cadena[i]=='*'){
			printf("C=>%c\n", cadena[i]);
			C_aux=raiz->dato;		
			raiz->der=NULL;
			Rizq_aux=crearNodo(cadena[i-1]);
			raizAux=crearNodo(cadena[i]);
			raizAux->der=Rizq_aux;
			raiz->der=raizAux;
			i=i+1;
		} else if(cadena[i]=='|'){
			Rder=crearNodo(cadena[i+1]);
			nuevoArbol(&raiz,Rizq, Rder, '|');
			i=i+2;
		} else{
			Rder=crearNodo(cadena[i]);
			nuevoArbol(&raiz,Rizq,Rder,'o');
			i++;
		}
		Rizq=raiz;
	printf("\n");
	}

	if(raiz==NULL){
		nuevoArbol(&raiz,Rizq, NULL,cadena[i]);
	}

	if(i==longt){
		AFD("Raiz: ",raiz);
		//printf("->E0, inicial\n");
	}
	return raiz;
}


void PedirCadena(){
	char cad[50];	
	printf("Ingrese la expresion: \n");
	gets(cad);
	int tam=strlen(cad);
	Proceso(cad,0, tam);
}

int main(){
	PedirCadena();
	return 1;
}
