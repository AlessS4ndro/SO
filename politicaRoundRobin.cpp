#include <iostream>
#include <list>
#include <stdio.h>
#include <ncurses.h>
#include "kbhit.h"
#include <time.h>

using namespace std;

struct Proceso
{
	long tEjecucion;
	long tLlegada;
	char nombre;
	void informe();
	Proceso(){}
	Proceso(char n,int l,int e){nombre=n;tLlegada=l;tEjecucion=e;}
	~Proceso(){}
};
void Proceso::informe()
{
	cout<<nombre<<"  ->  "<<tEjecucion<<endl;
}


class PoliticaRoundRobin
{
		list<Proceso *> funcionamiento;
		list<Proceso *> listaProcesos;
		int quantum;
		Proceso * procesoActual;
		int tiempoActual;
	public:
		bool anadir_proceso(Proceso *);
		bool eliminar_proceso(Proceso *);
		bool actualizar(Proceso *);
	
		PoliticaRoundRobin(int q);
		~PoliticaRoundRobin(){}
};


PoliticaRoundRobin::PoliticaRoundRobin(int q)
{
	quantum=q;
	tiempoActual=0;

	cout<<"iniciando round robin con quantum "<<quantum<<endl;

	listaProcesos.push_back(new Proceso('A',0,3));
	listaProcesos.push_back(new Proceso('B',2,6));
	listaProcesos.push_back(new Proceso('C',4,4));
	listaProcesos.push_back(new Proceso('D',6,5));
	listaProcesos.push_back(new Proceso('E',8,2));	
	//funcionamiento.push_back(new Proceso('E',10,6));

	int ch = 0;
	init_keyboard();
	int tLlegada,tEjecucion;
	char nombre;

	list<Proceso *>::iterator it;
	
	while(ch != 'q'){
		
		if(kbhit()){
				ch =  readch();
		}
		
		printf("lopping\n");
		if(funcionamiento.size() == 0){
			cout<<"lista de procesos vacia"<<endl;
			actualizar(NULL);
		
		}
		for(it=funcionamiento.begin()	;it!=funcionamiento.end();it++){ // repeticiones para recorrerl la cola de procesos en el funcionamiento
			cout<<"ejntrando al for"<<endl;
			if(*it)
				(*it)->informe();
			else
				cout<<"sin procesos en la cola"<<endl;
		
/*			if(kbhit()){
				ch =  readch();

				cout<<"ingresa un nombre para el proceso"<<endl;
				cin>>nombre;
				cout<<"ingresa tiempo de llegada"<<endl;
				cin>>tLlegada;
				cout<<"ingresa tiempo de ejecucion"<<endl;
				cin>>tEjecucion;
				Proceso *nuevo = new Proceso(nombre,tLlegada,tEjecucion);
				anadir_proceso(nuevo);
				//printf("you hit %c\n",ch);
				cout<<"llegamos aca"<<endl;
			}*/
			if(actualizar(*it))
				eliminar_proceso(*it);		
			sleep(1);
		}
		
	}
	close_keyboard();
	exit(0);

}

bool PoliticaRoundRobin::anadir_proceso(Proceso * nuevo)
{
	if(nuevo){
		funcionamiento.push_back(nuevo);
		return true;
	}
	return false;
}


bool PoliticaRoundRobin::eliminar_proceso(Proceso *procesoFinalizado)
{
	if(procesoFinalizado->tEjecucion==0){
		funcionamiento.pop_front();
		return true;
	}
	return false;
}

bool PoliticaRoundRobin::actualizar(Proceso *procesoActual)
{
	if(!procesoActual){
		if(listaProcesos.size()!=0){
			for(list<Proceso *>::iterator it=listaProcesos.begin();it!=listaProcesos.end();it++){
				cout<<"buscando procesos nuevos para agregar al funcionamiento"<<endl;
				if((*it)->tLlegada==tiempoActual){
					anadir_proceso(*it);
				}
			}		
		}
		tiempoActual++;
	}
	else{
		procesoActual->tEjecucion-=quantum;
		//cout<<"tiempo"<<procesoActual->tEjecucion<<endl;
		tiempoActual++;
		if(procesoActual->tEjecucion <= 0)
			return true;	
	}
	//cout<<"retornando falso "<<endl;
	return false;
}

int main(int argc,char ** argv)
{
	
	cout<<"probando probando"<<endl;
	PoliticaRoundRobin ejemplo(atoi(argv[1]));

	return 0;
}
