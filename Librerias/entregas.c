#include <stdio.h>

#include "Interfaz\interfaz.h"

void importarArchivo()
{
	char nombreArchivo[50];

	printf("\nIngrese el nombre del archivo a importar: ");
	getchar();
	scanf("%49[^\n]s", nombreArchivo);

	FILE * archivo = fopen(nombreArchivo, "r");

	if(archivo == NULL)
	{
		printf(red "\nNo se encontro el archivo\n" reset);
		return;
	}

	printf(green "\nSe encontro el archivo!\n" reset);

	int cantLineas;
	printf("\nIngrese la cantidad de lineas a leer: ");
	scanf("%i", &cantLineas);


	if(cantLineas == 0)
	{
		printf(red "\nNo se leyo ninguna linea\n" reset);
		return;
	}

	char lineaLeida[100];
	int cont = 0;
	
	while(cont != cantLineas && fgets(lineaLeida, 100, archivo) != NULL)
	{
		cont++;
	}

	if(cont < cantLineas)
	{
		printf(blue"\nSe llego al final del archivo\n");
		printf("Se leyeron unicamente %i lineas\n"reset, cont);
	}
	
	printf(green "\nEl archivo se importo correctamente!\n" reset);
}