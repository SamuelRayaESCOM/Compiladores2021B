#include<bits/stdc++.h>
#include "Biblioteca.cpp"
using namespace std;

map<int,vector<int>> tabla;
vector<pair<int,char>> saltos;
set<char> setSimbolos;
vector<char> simbolos;
vector<set<int>> setSiguientes;

struct ARBOL{
	char simbolo;
	bool anulable;
	int num;
	int posicion;
	vector<int> primeros;
	vector<int> ultimos;
    ARBOL *izq;
	ARBOL *der;
};

struct transicion{
	char simbolo;
	int deDonde;
	int aDonde;
};


/*****************************/
class automata{
	public: 
		int final;
	//	setFinal(int final){this->final=final;}
	//	getFinal(){ return final; }
		void Transicion(int a, int b, char simbolo){
			transicion t;
			t.deDonde=a;
			t.aDonde=b;
			t.simbolo=simbolo;
			transiciones.push_back(t);
		}
};

class fd: public automata{
	public:
	vector<pair<int,vector<int>>> estados;
	vector<char> simbolos;
	vector<int> finales;
	
	set<vector<int>> setEstadosCreados;

		vector<transicion> transiciones;
	
	public:

	int numEstado=0 ;

	void establecerFinales(){ 
		for(int i=0; i<estados.size(); i++){
			if(estados[i].second.size()!=0){
				int n = estados[i].second.size()-1;
				if(estados[i].second[n]==final){
					finales.push_back(estados[i].first);
				}
			}
		}
	}

	void nuevoEstado(vector<int> vectorEstado,int numeroEstado){
		estados.push_back(make_pair(numeroEstado,vectorEstado));
	}

	void primerEstado(ARBOL *raiz){
		setEstadosCreados.insert(raiz->primeros);
		nuevoEstado(raiz->primeros,numEstado);
	}

	void comprobarRepeticion(int estadoA, vector<int> estadoRepetido, char simbolo){
		set<vector<int>> analizados;
		for(int i=0; i<estados.size();i++){
			analizados.insert(estados[i].second);
			if(analizados.count(estadoRepetido)==1){
				Transicion(estadoA,estados[i].first,simbolo);
				break;
			}
		}
	}

	void crearEstados(){
		for(int i=0; i<estados.size();i++){
			for(int j=0; j<simbolos.size()-1;j++){
				vector<int> posibleEstado;
				set<int> setPosibleEstado;
				for(int l=0; l<estados[i].second.size();l++){
					for(int k=0; k<saltos.size();k++){
						if(estados[i].second[l]==saltos[k].first && saltos[k].second==simbolos[j]){
							for(int n=0; n<tabla[saltos[k].first].size();n++){
								if(setPosibleEstado.count(tabla[saltos[k].first][n])==0){
									posibleEstado.push_back(tabla[saltos[k].first][n]);
									setPosibleEstado.insert(tabla[saltos[k].first][n]);
								}
							}
						}
					}
				}
				sort(posibleEstado.begin(),posibleEstado.end());
				if(setEstadosCreados.count(posibleEstado)==0 && posibleEstado.size()>0){
					setEstadosCreados.insert(posibleEstado);
					numEstado++;
					nuevoEstado(posibleEstado,numEstado);
					Transicion(estados[i].first,numEstado,simbolos[j]);
				}
				else if(posibleEstado.size()>0){
					comprobarRepeticion(estados[i].first,posibleEstado,simbolos[j]);
				}
			}
		}
	}
	void mostrar(){
		transicion trans;
		ofstream f("Arbol_afd.vg");
		f<<"digraph AFN{\nrankdir = LR;"<<endl;
		f<<"node [shape = doublecircle];";
		for(int i=0; i<int(finales.size());i++){
			f<<" "<<finales[i];
		}
		f<<";"<<endl;
		f<<"node [shape = point ];"<<" qi"<<endl;
		f<<"node [shape = circle];"<<endl;
		f<<"qi"<<" -> "<<"0"<<endl;
		for(int i = 0; i < int(transiciones.size()); i++) {
			trans = transiciones[i];
			f<<trans.deDonde<<" -> "<<trans.aDonde<<" [label =\""<<trans.simbolo<<"\"];"<<endl;
		}
		f<<"}"<<endl;
	system("dot Arbol_afd.vg -Tpng > Arbol_afd.png");
	cout<<"Archivo guardado como Arbol_afd.vg, quiere ver la imagen de salida? S/N?\n ";
	char res;
	cin>>res;
	if(res=='S' || res=='Y' ||res=='s' || res=='y')
		system("Arbol_afd.png");
	}

};

int num=1;
/*  2. */
fd construirAFD(ARBOL *arbol){
	fd *nuevoAFD = new fd;
	nuevoAFD->simbolos=simbolos;
	nuevoAFD->primerEstado(arbol);
	nuevoAFD->crearEstados();
	nuevoAFD->final = num-1;
	nuevoAFD->establecerFinales();
	return *nuevoAFD;
}

vector<int> obtenerPrimeros(ARBOL* arbol){
	vector<int> respuesta;
	switch (arbol->simbolo){
		case '*':
			respuesta = arbol->izq->primeros;
			break;
		case '|':
			respuesta = arbol->izq->primeros;
			for(int i=0;i<arbol->der->primeros.size();i++){
					respuesta.push_back(arbol->der->primeros[i]);
			}
			break;
		case '.':
			respuesta = arbol->izq->primeros;
			if(arbol->izq->anulable){
				for(int i=0;i<arbol->der->primeros.size();i++){
					respuesta.push_back(arbol->der->primeros[i]);
				}
			}
			break;
		default:
			respuesta.push_back(arbol->posicion);
			break;
	}
	return respuesta;
}

vector<int> obtenerUltimos(ARBOL* arbol){
	vector<int> respuesta;
	switch (arbol->simbolo){
		case '*':
			respuesta = arbol->izq->ultimos;
			break;
		case '|':
			respuesta = arbol->izq->ultimos;
			for(int i=0;i<arbol->der->ultimos.size();i++){
				respuesta.push_back(arbol->der->ultimos[i]);
			}
			break;
		case '.':
			respuesta = arbol->der->ultimos;
			if(arbol->der->anulable){
				for(int i=0;i<arbol->izq->ultimos.size();i++){
					respuesta.push_back(arbol->izq->ultimos[i]);
				}
			}
			break;
		default:
			respuesta.push_back(arbol->posicion);
			break;
	}
	return respuesta;
}    

bool esAnulable(char simbolo, bool anulable1, bool anulable2){
	if(simbolo == '.'){
		if(anulable1==true && anulable2==true){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		if(anulable1==true or anulable2==true){
			return true;
		}
		else{
			return false;
		}
	}	
}

void siguientes(ARBOL *arbol){
	vector<int> aux;
	switch(arbol->simbolo){
		case '*':
			for(int i=0;i<arbol->ultimos.size();i++){
				for(int j=0;j<arbol->primeros.size();j++){
					if(setSiguientes[arbol->ultimos[i]-1].count(arbol->primeros[j])==0){
						setSiguientes[arbol->ultimos[i]-1].insert(arbol->primeros[j]);
						tabla[arbol->ultimos[i]].push_back(arbol->primeros[j]);
					}
				}
			}
		break;
		case '.':
			for(int i=0;i<arbol->der->primeros.size();i++){
				for(int j=0;j<arbol->izq->ultimos.size();j++){
					if(setSiguientes[arbol->izq->ultimos[j]-1].count(arbol->der->primeros[i])==0){
						setSiguientes[arbol->izq->ultimos[j]-1].insert(arbol->der->primeros[i]);
						tabla[arbol->izq->ultimos[j]].push_back(arbol->der->primeros[i]);
					}
				}
			}
		break;
	}
}

ARBOL *Concatenar(ARBOL* hijo1, ARBOL* hijo2){
	ARBOL *respuesta= new ARBOL;
	respuesta->simbolo='.';
	respuesta->izq=hijo1;
	respuesta->der=hijo2;
	respuesta->anulable=esAnulable('.',hijo1->anulable,hijo2->anulable);
	respuesta->primeros=obtenerPrimeros(respuesta);
	respuesta->ultimos=obtenerUltimos(respuesta);
	sort(respuesta->primeros.begin(),respuesta->primeros.end());
	siguientes(respuesta);
	return respuesta;
}

ARBOL *Disyuncion(ARBOL* hijo1, ARBOL* hijo2){
	ARBOL *respuesta= new ARBOL;
	respuesta->simbolo='|';
	respuesta->izq=hijo1;
	respuesta->der=hijo2;
	respuesta->anulable=esAnulable('|',hijo1->anulable,hijo2->anulable);
	respuesta->primeros=obtenerPrimeros(respuesta);
	respuesta->ultimos=obtenerUltimos(respuesta);
	sort(respuesta->primeros.begin(),respuesta->primeros.end());
	return respuesta;
}

ARBOL *estrella(ARBOL* hijo){
	ARBOL *respuesta= new ARBOL;
	respuesta->simbolo='*';
	respuesta->izq=hijo;
	respuesta->der=NULL;
	respuesta->anulable = true;
	respuesta->primeros=obtenerPrimeros(respuesta);
	respuesta->ultimos=obtenerUltimos(respuesta);
	sort(respuesta->primeros.begin(),respuesta->primeros.end());
	siguientes(respuesta);
	return respuesta;
}

ARBOL *crearArbol(string exp){
	stack<ARBOL*> P;					//PILA
	ARBOL *izquierdo;
	ARBOL *derecho;
	ARBOL *respuesta;
	for(char i:exp){
		ARBOL* aux = new ARBOL;
		if(i=='.'){
			derecho=P.top();
			P.pop();
			izquierdo=P.top();
			P.pop();
			respuesta=Concatenar(izquierdo,derecho);
			P.push(respuesta);
		}else if(i=='*'){
			izquierdo=P.top();
			P.pop();
			respuesta=estrella(izquierdo);
			P.push(respuesta);
		}else if(i=='|'){
			derecho=P.top();
			P.pop();
			izquierdo=P.top();
			P.pop();
			respuesta=Disyuncion(izquierdo,derecho);
			P.push(respuesta);
		}else{
			aux->simbolo=i;
			aux->izq=aux->der=NULL;
			if(i!='e'){
				aux->anulable=false;
				if(setSimbolos.count(i)==0){
					setSimbolos.insert(i);
					simbolos.push_back(i);
				}
			}else
				aux->anulable=true;
			aux->num=num;
			set<int> auxset;
			setSiguientes.push_back(auxset);
			saltos.push_back(make_pair(num,i));
			aux->posicion=num;
			aux->primeros=obtenerPrimeros(aux);
			aux->ultimos=obtenerUltimos(aux);
			num++;
			P.push(aux);
		}
	}
	return P.top();
}


int main(){
	ARBOL *respuesta;
	Biblioteca Bl;
/*  1.  */
	string expresion,exp_final="(";
	cin>>expresion;
	exp_final+=expresion + ")#";

	exp_final=bl.Agregando_simbolos(exp_final);
	string post;
	post=bl.postfija(exp_final);

/********** 2. **********/
	respuesta=crearArbol(post);
	AFD respuestaAFD;
	respuestaAFD=construirAFD(respuesta);
	respuestaAFD.mostrar();
	return 0;
}