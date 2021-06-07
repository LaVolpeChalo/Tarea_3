#include <stdio.h>
#include <time.h>

#include "Librerias\TDA_Mapa\hashmap.h"
#include "Librerias\TDA_Mapa\list.h"
#include "Librerias\Interfaz\interfaz.h"
#include "Librerias\entregas.h"

int main()
{
	short opcion;
	HashMap * mapaIdentificacion = createMap(10);
	List * Listarutas = createList();
	int tamano;

	srand(time(NULL));

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
				if(tamano != 0) entregasCercanas5(mapaIdentificacion);
				else printf(red"\nNo ha recibido entregas\n"reset);
				break;
			case 4:
			    if(tamano != 0) crearruta(mapaIdentificacion,Listarutas);
				else printf(red"\nNo ha recibido entregas\n"reset);
				break;
			case 5:
			    if(tamano != 0) rutaleatoria(mapaIdentificacion,Listarutas);
				else printf(red"\nNo ha recibido entregas\n"reset);
				break;
			case 6:
<<<<<<< HEAD
			    printf("Mejorar Ruta");
			 	if(get_size(Listarutas) != 0 ) mejorar_ruta(mapaIdentificacion, Listarutas);
			 	else printf(red"\n No hay rutas creadas" reset);
=======
			Printf("Mejorar Ruta");
>>>>>>> parent of 8313d00 (Ruta Optima funcional)
				break;
			case 7:
			    if(get_size(Listarutas) != 0) mostrarrutas(Listarutas);
				else printf(red"\nNo hay rutas creadas\n"reset); 
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