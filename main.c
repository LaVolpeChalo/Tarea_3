#include <stdio.h>

#include "Librerias\Interfaz\interfaz.h"

int main()
{
	short opcion;

	do
	{
		mostrarMenu();
		//Eleccion de la opcion del menu
		printf("\nElija una opcion: ");
		scanf("%i", &opcion);
		if(opcion == 0) break;

		switch(opcion)
		{
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			default:
				printf(red "\nNo existe tal opcion\n" reset);
				break;
		}
	}while(opcion != 0);

	printf("\nFin del Programa\n\n");
	return 0;
}