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
	tipoEntrega * aux = first(nodos);

	while(aux != NULL){

		if(aux->identificacion == destino) return 0;	
		
		aux = next(nodos);

	}
	return 1;
}


double distanciaDosPuntos(tipoEntrega * posicion1, tipoEntrega * posicion2)
{
	double distancia = sqrt(pow(posicion1->coordenadaX - posicion2->coordenadaX, 2) + pow(posicion1->coordenadaY - posicion2->coordenadaY, 2));
	return distancia;
}

List * nodosadyacentes(HashMap * mapaIdentificacion,tipoEntrega * actual,tipoRuta * ruta,int limite,int print){
	    tipoEntrega * node = firstMap(mapaIdentificacion);
		List * nodos = createList();
        double mayord = 0;
		double menord;
	    double ultimad = 0;
		int impresiones = 0;
		tipoEntrega * minimo;
        
		///Se recoge el mayor recorrido
		do{

			if(yarecorrido(ruta->recorrido,node->identificacion));
			else{

			    if(distanciaDosPuntos(node,actual)>mayord) mayord = distanciaDosPuntos(node,actual);

			}


			node = nextMap(mapaIdentificacion);
		}while (node != NULL);

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
			if(print) printf("%d)%.2lf\n",minimo->identificacion,distanciaDosPuntos(actual,minimo));
			pushFront(nodos,minimo);

			ultimad = menord;

            //Función unicamente utilizada en caso de que el usuario requiera solo una cantidad determinada por el de nodos adyacentes
			impresiones++;
			if(impresiones==limite){
				return nodos;
			}

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

	tipoRuta * ruta=nuevodatoruta(posicion1->coordenadaX,posicion1->coordenadaY,mapaIdentificacion);
    
	printf("\nEntregas mas cercanas: \n");
	List * aux = nodosadyacentes(mapaIdentificacion,posicion1,ruta,5,1);

	
}

void crearruta(HashMap *mapaIdentificacion,List *listarutas){

	long long x,y;
	int destino;
	tipoEntrega *actual;

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
        
		///Se crea el tipo de dato en caso de ser su primer uso
		if(entregasrealizadas == 0){

			actual = (tipoEntrega*)calloc(1,sizeof(tipoEntrega));
			actual->coordenadaX = x;
			actual->coordenadaY = y;
		}
        
        List * nodos=nodosadyacentes(mapaIdentificacion,actual,ruta,size(mapaIdentificacion),1);

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
	tipoRuta * ruta = first(listarutas);
	double mayord=0;
	double menord;
	double ultimad=0;
	tipoRuta * minimo;

    //Busqueda peor ruta
	do{

		if(ruta->total_recorrido > mayord) mayord = ruta->total_recorrido;
		ruta = next(listarutas);

	}while(ruta != NULL);

    //Se busca la ruta que sea menor a
	do{

		ruta = first(listarutas);
		menord = mayord;
		do{


			if(ruta->total_recorrido <= menord && ruta->total_recorrido > ultimad){
			   minimo = ruta;
				menord = ruta->total_recorrido;
					
			}
	
			ruta = next(listarutas);

		}while(ruta != NULL);


		printf(blue"\nRuta %s:\n"reset,minimo->nombreruta);
		printf("\nCoordenada inicial: %lli,%lli\n",minimo->cordenada_inicial[0],minimo->cordenada_inicial[1]);
		printf("Recorrido: [");

		for(int j = 0;minimo->recorrido[j] != '\0';j++){

			if(minimo->recorrido[j+1] == '\0') printf("%d]",minimo->recorrido[j]);
			else printf("%d,",minimo->recorrido[j]);

		}

		printf("\nCantidad a recorrer: %.2lf\n",minimo->total_recorrido);

		ultimad = menord;

	}while(menord != mayord);
}

void rutaleatoria(HashMap *mapaIdentificacion,List *listarutas){

	long long x,y;
	tipoEntrega * actual;
	int destino;

    //Se solicitan las coordenadas iniciales
	printf("\nIngrese las coordenadas correspondientes: \n");
	printf("Coordenada x: ");
	getchar();
	scanf("%lli",&x);
	printf("Coordenada y: ");
	getchar();
	scanf("%lli",&y);

    tipoRuta * ruta = nuevodatoruta(x,y,mapaIdentificacion); 

	int entregasrealizadas = 0;

	while(entregasrealizadas < size(mapaIdentificacion)){

		tipoEntrega *node;
        
		///Se crea el tipo de dato en caso de ser su primer uso
		if(entregasrealizadas == 0){

			actual = (tipoEntrega*)calloc(1,sizeof(tipoEntrega));
			actual->coordenadaX = x;
			actual->coordenadaY = y;
		}
        
        List * nodos=nodosadyacentes(mapaIdentificacion,actual,ruta,size(mapaIdentificacion),0);

		do{

			destino = rand() % size(mapaIdentificacion) + 1;

		}while(estaenlista(nodos,destino));

		node = busquedaPosicion(mapaIdentificacion,destino);
		ruta->recorrido[entregasrealizadas] = destino;
		ruta->total_recorrido += distanciaDosPuntos(actual,node);


		actual->coordenadaX = node->coordenadaX;
		actual->coordenadaY = node->coordenadaY;

		entregasrealizadas++;

	}

	///Finalmente se le pregunta el nombre de la ruta y se envía  a la lista de rutas
	printf("\nIngrese un nombre para la ruta: ");
	getchar();
	fscanf(stdin,"%20[^\n]s",ruta->nombreruta);
	pushFront(listarutas,ruta);
	printf(green"Ruta ingresada correctamente\n"reset);

}

//funcion 6 mejorar ruta
/*
Void * mejorar_ruta(HashMap *mapaIdentificacion,List *listarutas){
	printf("Escoja la ruta que desea modificar.");
	tipoRuta * ruta_modificar = busquedaPosicion(HashMap * mapaIdentificacion, int identificacion);
	fscanf(stdin, "%20[^\n]s", listarutas);
	printf(Las ruta son: &listarutas);
	printf("Escoja cual ruta desea intercambiar");
	fscanf()
	fscanf()
	//calcular la distancia
	printf("La distancia entre la mejora de ruta es: ");
	printf("La distancia antes del cambio es:");
	if(si la distancia es mayor a la anterior)
	{
		printf("La nueva distancia es mayor");
		printf("No se realizo ningun cambio");
	}else{
		printf("La nueva distacia es menor");
		printf("Se guardo la nueva mejora de ruta");
	}
}
*/
