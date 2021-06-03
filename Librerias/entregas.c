#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "TDA_Mapa\hashmap.h"
#include "Interfaz\interfaz.h"
#include "TDA_Mapa\list.h"

typedef struct tipoEntrega
{
	int identificacion;
	long long coordenadaX;
	long long coordenadaY;
}tipoEntrega;

typedef struct tipoRuta
{
	char nombreruta[20];
	int *recorrido;
	long long cordenada_inicial[2];
	double total_recorrido;

}tipoRuta;

tipoRuta * nuevodatoruta(long long x,long long y,HashMap *h){
    tipoRuta *nuevo=(tipoRuta*)malloc(sizeof(tipoRuta));
	nuevo->recorrido=(int*)calloc(size(h)+1,sizeof(int));
	nuevo->cordenada_inicial[0]=x;
	nuevo->cordenada_inicial[1]=y;
	nuevo->total_recorrido=0;
	return nuevo;
}

int yarecorrido(int *recorrido,int id){
	for(int i=0;recorrido[i]!='\0';i++){
		if(recorrido[i]==id){
			return 1;
		}
	}
	return 0;
}

int estaenlista(List * nodos,int destino){
	tipoEntrega * aux=first(nodos);
	while(aux!=NULL){
		if(aux->identificacion == destino) return 0;	
		
		aux=next(nodos);

	}
	return 1;

}

double distanciaDosPuntos(tipoEntrega * posicion1, tipoEntrega * posicion2)
{
	double distancia = sqrt(pow(posicion1->coordenadaX - posicion2->coordenadaX, 2) + pow(posicion1->coordenadaY - posicion2->coordenadaY, 2));
	return distancia;
}

List * nodosadyacentes(HashMap * mapaIdentificacion,tipoEntrega * actual,tipoEntrega *minimo,tipoRuta * ruta){
	///Se recoge el mayor recorrido
	    tipoEntrega * node = firstMap(mapaIdentificacion);
		List * nodos = createList();
        double mayord = 0;
		double menord;
	    double ultimad = 0;

		do
		{
			if(yarecorrido(ruta->recorrido,node->identificacion));
			else{
				if(distanciaDosPuntos(node,actual)>mayord){
					mayord = distanciaDosPuntos(node,actual);
				}
			}
			node = nextMap(mapaIdentificacion);
		} while (node != NULL);

        /*Se comienza la impresión desde el menor al mayor dato por medio de iteraciones donde se busca un dato menor al máximo
		pero mayor al último dato impreso*/
		do{
			node = firstMap(mapaIdentificacion);
			menord = mayord;
			do{
			    if(yarecorrido(ruta->recorrido,node->identificacion));
			    else{
				    if(distanciaDosPuntos(actual,node) <= menord && distanciaDosPuntos(actual,node) > ultimad){
					    minimo = node;
					    menord = distanciaDosPuntos(actual,node);
					
				    }
			    }
				node = nextMap(mapaIdentificacion);
			}while(node != NULL);
			printf("%d)%.2lf\n",minimo->identificacion,distanciaDosPuntos(actual,minimo));
			pushFront(nodos,minimo);

			ultimad = menord;


		}while(menord != mayord);

	return nodos;
}

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

void crearruta(HashMap *mapaIdentificacion,List *listarutas){

	long long x,y;
	int destino,impreso;
	int aimprimir=size(mapaIdentificacion);
	tipoEntrega *actual;
	tipoEntrega *minimo;

    ///Se le solicita al usuario el ingreso de las coordenadas iniciales
	printf("\nIngrese las coordenadas correspondientes: \n");
	printf("Coordenada x: ");
	getchar();
	scanf("%lli",&x);
	printf("Coordenada y: ");
	getchar();
	scanf("%lli",&y);

	tipoRuta * ruta = nuevodatoruta(x,y,mapaIdentificacion);


	int entregasrealizadas = 0;


    ///Mientras la cantidad de entregas realizadas en la ruta no sea igual al tamaño del total de las entregas, esta seguirá iterando
	while(entregasrealizadas < size(mapaIdentificacion)){

		printf("\nLista de proximos destinos y su distancia: \n");
		tipoEntrega *node;
		double mayord=0;
		double menord;
	    double ultimad=0;
        
		///Se crea el tipo de dato en caso de ser su primer uso
		if(entregasrealizadas == 0){

			actual = (tipoEntrega*)calloc(1,sizeof(tipoEntrega));
			actual->coordenadaX = x;
			actual->coordenadaY = y;
		}
        
        List * nodos=nodosadyacentes(mapaIdentificacion,actual,minimo,ruta);

        ///Se le pide al usuario que escoja que destino desea tomar
		printf("\nEscoja hacia que destino desea desplazarse: ");

		int contador = 0;
		do{

			if(contador != 0) printf(red"\nPorfavor, ingrese un numero de la lista mostrada en pantalla.\n"reset);

			getchar();
		    scanf("%i",&destino);
			contador++;
			
		}while(estaenlista(nodos,destino));

		node = busquedaPosicion(mapaIdentificacion,destino);
		ruta->recorrido[entregasrealizadas] = destino;
		ruta->total_recorrido += distanciaDosPuntos(actual,node);


		actual->coordenadaX = node->coordenadaX;
		actual->coordenadaY = node->coordenadaY;

		entregasrealizadas++;
		printf(green"\nRecorrido actualizado\n"reset);

	}
	///Finalmente se le pregunta el nombre de la ruta y se envía  a la lista de rutas
	printf("\nIngrese un nombre para la ruta: ");
	getchar();
	fscanf(stdin,"%20[^\n]s",ruta->nombreruta);
	pushFront(listarutas,ruta);
	printf(green"Ruta ingresada correctamente\n"reset);

}

void mostrarrutas(List *listarutas){
	tipoRuta * ruta=first(listarutas);


	do{

		printf("\nRuta %s:\n",ruta->nombreruta);
		printf("\nCoordenada inicial: %lli,%lli\n",ruta->cordenada_inicial[0],ruta->cordenada_inicial[1]);
		printf("Recorrido: [");

		for(int j=0;ruta->recorrido[j]!='\0';j++){

			if(ruta->recorrido[j+1]=='\0') printf("%d]",ruta->recorrido[j]);
			else printf("%d,",ruta->recorrido[j]);

		}

		printf("\nCantidad a recorrer: %.2lf\n",ruta->total_recorrido);
		ruta=next(listarutas);

	}while(ruta!=NULL);
}