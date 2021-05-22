#include <stdio.h>

#include "Librerias\TDA_Mapa\hashmap.h"
#include "Librerias\Interfaz\interfaz.h"
#include "Librerias\entregas.h"

int main()
{
	short opcion;
	HashMap * mapaIdentificacion = createMap(10);
	int tamano;

	do
	{
		mostrarMenu();

		//Eleccion de la opcion del menu
		printf("\nElija una opcion: ");
		scanf("%i", &opcion);
		if(opcion == 0) break;
		
		tamano = size(mapaIdentificacion);

		switch(opcion)
		{
			case 1:
				importarArchivo(mapaIdentificacion);
				break;
			case 2:
				if(tamano != 0) distanciaEntregas(mapaIdentificacion);
				else printf(red"\nNo ha recibido entregas\n"reset);
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