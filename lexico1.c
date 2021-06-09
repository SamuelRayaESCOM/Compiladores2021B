%{
// Comentar solo dentro de las partes donde se use c
#include <stdio.h>	

//separador %% y lenguaje regular no deben llevar un espacio inicial.
%}

%%
hola		{printf("¡Hola Mundo!\n");}

//compilar:
//flex archivo.l
//gcc lx.yy.c -lfl