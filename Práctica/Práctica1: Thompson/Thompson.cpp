#include<bits/stdc++.h>
#include "Biblioteca.cpp"
using namespace std;

int i,j=0;
struct transicion{
	char simbolo;
	int deDonde;
	int aDonde;
};

class AFN{
	public:
	int final;
	vector<int> estados;
	vector<transicion> transiciones;
	
	void numEstados(int num){
		for(int i=0; i<num ;i++){
			estados.push_back(i);
		}
	}
	
	void Transicion(int a, int b, char simbolo){
		transicion t;
		t.deDonde=a;
		t.aDonde=b;
		t.simbolo=simbolo;
		transiciones.push_back(t);
	}

	void mostrar(){
		transicion trans;
		ofstream f("afd.vg");
		f<<"digraph AFN{\nrankdir = LR;"<<endl;
		f<<"node [shape = doublecircle];"<<" "<<final<<";"<<endl;
		f<<"node [shape = point ];"<<" qi"<<endl;
		f<<"node [shape = circle];"<<endl;
		f<<"qi"<<" -> "<<"0"<<endl;
		for(j = 0; j < estados.size(); j++){
			for(i = 0; i < transiciones.size(); i++) {
				trans = transiciones[i];
				if(trans.deDonde == j){
					if(trans.simbolo == 'e'){
						f<<trans.deDonde<<" -> "<<trans.aDonde<<" [label =\"&#949;\"];"<<endl;
					}
					else{
						f<<trans.deDonde<<" -> "<<trans.aDonde<<" [label =\""<<trans.simbolo<<"\"];"<<endl;
					}
					
				}
			}
		}
		f<<"}"<<endl;
		system("dot afd.vg -Tpng > afd.png");
		cout<<"Archivo guardado como afd.vg, quiere ver la imagen de salida? S/N?\n ";
		char res;
		cin>>res;
		if(res=='S' || res=='Y' ||res=='s' || res=='y')
			system("AFD.png");
	}
};

AFN concatenar(AFN opr1, AFN opr2) {
	AFN aux;
	aux.numEstados(opr1.estados.size() + opr2.estados.size()-1);

	for(i=0; i<opr1.transiciones.size(); i++)
		aux.Transicion(opr1.transiciones[i].deDonde , opr1.transiciones[i].aDonde, opr1.transiciones[i].simbolo);

	for(i=0; i<opr2.transiciones.size(); i++)
		aux.Transicion(opr2.transiciones[i].deDonde + opr1.estados.size()-1, opr2.transiciones[i].aDonde + opr1.estados.size()-1, opr2.transiciones[i].simbolo);

	aux.final=opr2.estados.size()+ opr1.estados.size()-2;
	return aux;
}

AFN Disyuncion(AFN opr1, AFN opr2) {
	AFN aux;
	aux.numEstados(opr1.estados.size() + opr2.estados.size()+2);			//Estados, tanto del lado izquierdo, como el derecho, +2 por el inicio y fin.
	aux.Transicion(0,1,'e');
	for(i=0; i<opr1.transiciones.size();i++)
		aux.Transicion(opr1.transiciones[i].deDonde + 1, opr1.transiciones[i].aDonde + 1, opr1.transiciones[i].simbolo);
	aux.final=opr1.estados.size()+opr2.estados.size()+1;			//Nodo final
	aux.Transicion(opr1.estados.size(),aux.final,'e');
	aux.Transicion(0, opr1.estados.size() + 1,'e');
	for(i=0; i<opr2.transiciones.size();i++)
		aux.Transicion(opr2.transiciones[i].deDonde + opr1.estados.size() + 1, opr2.transiciones[i].aDonde + opr1.estados.size() + 1,opr2.transiciones[i].simbolo);
	aux.Transicion(opr1.estados.size() + opr2.estados.size() , aux.final,'e');
	return aux;
}

AFN estrella(AFN origen) {
	AFN aux;
	aux.numEstados(origen.estados.size()+2);			//+2 -> El de entrada y salida.
	aux.Transicion(0, 1, 'e');
	for(i=0; i<origen.transiciones.size(); i++) 
		aux.Transicion(origen.transiciones[i].deDonde + 1,origen.transiciones[i].aDonde+1, origen.transiciones[i].simbolo);
	int num_estados=origen.estados.size();
	aux.Transicion(num_estados,num_estados+1,'e');		//Pen??ltimo al final
	aux.Transicion(num_estados,1, 'e');					//Pen??ltimo al segundo
	aux.Transicion(0,num_estados+1, 'e');					//principio a fin/aceptaci??n
	aux.final = num_estados+1;								//Aceptaci??n; 1 + n??mero de estados totales
	return aux;
}


AFN Proceso(string exp){
	stack<AFN> afnP;
	AFN operando1;
	AFN operando2;
	AFN aux;
	for(char i:exp){
		if((i!='*' && i!='|' &&i!='.')){
			AFN *afn;
			afn=new AFN;
			afn->numEstados(2);
			afn->Transicion(0,1,i);
			afn->final=2;
			afnP.push(*afn);			//Mete en la pila el AFN creado 
		}
		else if(i=='*'){
			operando1 = afnP.top();
			afnP.pop();
			afnP.push(estrella(operando1));
		}
		else if(i=='|'){
			operando2 = afnP.top();
			afnP.pop();
			operando1 = afnP.top();
			afnP.pop();
			afnP.push(Disyuncion(operando1,operando2));
		}
		else{
			operando2 = afnP.top();
			afnP.pop();							//Quita de la PILA 			
			operando1 = afnP.top();
			afnP.pop();
			afnP.push(concatenar(operando1,operando2));
		}
	}
	return afnP.top();
}

int main(){
	Biblioteca bl;
	string post;
	string expresion;
	cout<<"Ingrese la expresion\n";
	cin>>expresion;
	int longt=expresion.size();
	expresion=bl.Agregando_simbolos(longt,expresion);
	post=bl.postfija(expresion);
	AFN respuesta;
	respuesta=Proceso(post);
	respuesta.mostrar();
	return 0;
}
