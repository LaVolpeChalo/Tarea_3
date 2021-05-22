#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "TDA_Mapa\hashmap.h"
#include "Interfaz\interfaz.h"

typedef struct tipoEntrega
{
	int identificacion;
	long long coordenadaX;
	long long coordenadaY;
}tipoEntrega;

tipoEntrega * lecturaDeInformacion(char * lineaLeida, int id)
{
	tipoEntrega * nuevaPosicion = malloc (sizeof(tipoEntrega));
	char * fragmento;

	//Posicion X
	fragmento = strtok(lineaLeida, " ");
	nuevaPosicion->coordenadaX = strtol(fragmento, NULL, 10);

	//Posicion Y
	fragmento = strtok(NULL, "\n");
	nuevaPosicion->coordenadaY = strtol(fragmento, NULL, 10);

	//ID
	nuevaPosicion->identificacion = id;
	return nuevaPosicion;
}

double distanciaDosPuntos(tipoEntrega * posicion1, tipoEntrega * posicion2)
{
	double distancia = sqrt(pow(posicion1->coordenadaX - posicion2->coordenadaX, 2) + pow(posicion1->coordenadaY - posicion2->coordenadaY, 2));
	return distancia;
}

tipoEntrega * busquedaPosicion(HashMap * mapaIdentificacion, int identificacion)
{
	tipoEntrega * posicionBuscada = searchMap(mapaIdentificacion, &identificacion);
    if(posicionBuscada == NULL)
    {
        printf(red"\nNo existe la entrega\n"reset);
        return NULL;
    }
	return posicionBuscada;
}

void importarArchivo(HashMap * mapaIdentificacion)
{
	char nombreArchivo[50];

	printf("\nIngrese el nombre del archivo a importar: ");
	getchar();
	scanf("%49[^\n]s", nombreArchivo);
	//strcpy(nombreArchivo,"tarea3_tsp.txt"); RECORDAR ELIMINARLO CUANDO SE ENTREGUE LA TAREA

	FILE * archivo = fopen(nombreArchivo, "r");

	if(archivo == NULL)
	{
		printf(red "\nNo se encontro el archivo!\n" reset);
		return;
	}

	printf(green "\nSe encontro el archivo!\n" reset);

	int cantLineas;
	printf("\nIngrese la cantidad de lineas a leer: ");
	scanf("%i", &cantLineas);

	if(cantLineas == 0)
	{
		printf(red "\nNo se leyo ninguna linea!\n" reset);
		fclose(archivo);
		return;
	}

	char lineaLeida[100];
	int cont = 0;
	
	while(cont != cantLineas && fgets(lineaLeida, 100, archivo) != NULL)
	{
		tipoEntrega * nuevaPosicion = lecturaDeInformacion(lineaLeida, cont + 1);
		insertMap(mapaIdentificacion, &nuevaPosicion->identificacion, nuevaPosicion);
		cont++;
	}

	if(cont < cantLineas)
	{
		printf(blue"\nSe llego al final del archivo\n");
		printf("Se leyeron unicamente %i lineas!\n"reset, cont);
	}

	printf(green "\nEl archivo se importo correctamente!\n" reset);
	fclose(archivo);
}

void distanciaEntregas(HashMap * mapaIdentificacion)
{
	int identificacion1, identificacion2;

	printf("\nIngrese el numero de identificacion de la entrega 1: ");
	scanf("%i", &identificacion1);
	tipoEntrega * entrega1 = busquedaPosicion(mapaIdentificacion, identificacion1);
	if(entrega1 == NULL) return;

	printf("\nIngrese el numero de identificacion de la entrega 2: ");
	scanf("%i", &identificacion2);
	tipoEntrega * entrega2 = busquedaPosicion(mapaIdentificacion, identificacion2);
	if(entrega2 == NULL) return;

	printf(green "\nSe encontraron ambas entregas\n" reset);

	printf(green "\nLa distancia entre entregas es de %.2lf\n" reset, distanciaDosPuntos(entrega1, entrega2));
}

void entregasCercanas5(HashMap *mapaIdentificacion)
{
	//PRIMERO ENCONTRAMOS TODO LO QUE VAMOS A OCUPAR, EL ENTREGAAUX SIRVE PARA RECCORER LOS PUNTOS
	int entrega1;

    printf("\nIngrese la identificacion de la entrega: ");
    scanf("%i", &entrega1);

    tipoEntrega * posicion1 = busquedaPosicion(mapaIdentificacion, entrega1);
	if(posicion1 == NULL) return;
    
	tipoEntrega * posicionAux = firstMap(mapaIdentificacion);

	//AHORA DEFINIMOS TODO LO QUE NECESITAREMOS PARA ENCONTRAR LOS MAS CERCANOS
    int arreglo[5]; //identificador
    float arreglo2[5];//distancia

    float distanciaEntregas;
    float distanciaAux;

    int cont = 0;
	float maximo = 0;
	int i,k;

	/*
		AQUI EMPIEZA LA FUNCION PARA ENCONTRAR AL MAS CERCANO
		lo que pense al principio es ir encontrando los mas cercanos y guardarlos
		en un arreglo, pero como pide mostrar el ID y la distancia, ocupo 2.
		quizas se pueda hacer mejor, pero es una idea.
	*/

    while(posicionAux != NULL)
	{
        distanciaEntregas = distanciaDosPuntos(posicion1, posicionAux);
		if(distanciaEntregas == 0)
		{
			posicionAux = nextMap(mapaIdentificacion);
			continue;
		}
        if(cont != 0)
		{
            if(cont > 4)
			{
				//Recorro el arreglo distancia
				for(i = 0 ; i < 5 ; i++)
				{
					//printf("ENTRA2\n\n");
					if(maximo == arreglo2[i])
					{
						if(distanciaEntregas < maximo)
						{
							//ESTOS PRINTF SON PARA VER QUIENES CAMBIAN POR CUALES
							printf(red"ID: %d con distancia %.2lf CAMBIA CON \n"reset,arreglo[i],arreglo2[i]);
							arreglo[i] = posicionAux->identificacion;
							arreglo2[i] = distanciaEntregas;
							printf(green"ID: %d con distancia %.2lf <- \n"reset,arreglo[i],arreglo2[i]);

							//Encontrar otro maximo
							maximo = 0;
							for(k = 0 ; k < 5 ; k++)
								if(maximo < arreglo2[k])	maximo = arreglo2[k];
							
							break;
						}
					}
				}
            }
			else
			{
				//printf("\n%d cont",cont);
                arreglo[cont] = posicionAux->identificacion;
                arreglo2[cont] = distanciaEntregas;
				//ESTE PRINTF ES PARA VER QUIENES SON LOS QUE SE GUARDAN PRIMERO
				printf(green"ID: %d con distancia %.2lf \n"reset,arreglo[cont],arreglo2[cont]);
				if(maximo < distanciaEntregas) maximo = distanciaEntregas;
            }
        }
		else
		{
            arreglo[0] = posicionAux->identificacion;
            arreglo2[0] = distanciaEntregas;
			//ESTE PRINTF PARA VER QUIEN ES EL PRIMERO EN GUARDARSE
			printf(blue"ID: %d con distancia %.2lf \n"reset,arreglo[0],arreglo2[0]);
			maximo = distanciaEntregas;
        }
        distanciaAux = distanciaEntregas;
        posicionAux = nextMap(mapaIdentificacion);
		cont++;
    }

	//mostrar EL RESULTADO
	printf(green"\n\nLas distancias mas cercanas a la posicion con id: %d",posicion1->identificacion);
	
	int largo = 5; 
	//Esto sirve para cuando no hay mas de 5 lugares
	if(cont < 5) largo = cont;

	for(i = 0 ; i < largo ; i++)
		printf("\nID: %d con distancia %.2lf",arreglo[i],arreglo2[i]);
	
	printf("\n"reset);
}