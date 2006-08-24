#ifndef ALG
#define ALG

#include "info.h"
#include "params.h"
#include "bands.h"
#include "defines.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
//#include <set>

using namespace std;

/* Global variables (for efficiency) */

pinfo info;
vectorbands spectralbands;
Tmatrix outmatrix;
double *changesvector;


/*************************** funciones auxiliares ************************************/


/*------------------------------------------------------------------------------------
  Nombre:      error	
  Descripción: Rutina encargada del manejo de errores. Imprime un mensaje y sale del programa
  Entrada:     Número de error y parámetro del mensaje a imprimir
  Salida:
  ------------------------------------------------------------------------------------*/

void error(int num, char *cad)
{
     switch (num)
     {
 	case 0: cerr << "Syntax: " << cad << " <filename.wav> [context]\nIf no context is specified, context=0\n";
 	        break;
	case 1: cerr << "Filename " << cad << " not found\n";
		break;
     }
     exit(-1);
}

#endif
