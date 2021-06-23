#include<bits/stdc++.h>

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
	aux.Transicion(num_estados,num_estados+1,'e');		//Penúltimo al final
	aux.Transicion(num_estados,1, 'e');					//Penúltimo al segundo
	aux.Transicion(0,num_estados+1, 'e');					//principio a fin/aceptación
	aux.final = num_estados+1;								//Aceptación; 1 + número de estados totales
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

int preced(char caracter) {
    if(caracter=='*')
 		return 3;
	else if(caracter=='.')
		return 2;
	else if(caracter=='|')
		return 1;
	else
		return 0;
}
 
string postfija(string exp) {	/* Pone la cadena en formato postfijo '*/
    stack<char> p;											//PILA p
    string post;
    for(char i: exp){										//Desde el inicio al final
        if(i!='(' && i!=')' && i!='.' &&i!='*' && i!='|')	//Si es caracter a_zA_Z0_9
        	post+= i;
        else if(i == '(')
        	p.push('(');
        else if(i == ')'){
        	while(p.top()!='(' && !p.empty()){
                post += p.top();							//
                p.pop();
            }
            p.pop();
        }else{												//Cualquier operando
            while(!p.empty() && (preced(i)<=preced(p.top()))){
                post+= p.top();
                p.pop();			//Saca de la pila 
            }
            p.push(i);		//Incluye el caracter actual
        }
    }

    while(!p.empty()) {			//Incluye en la cadena final cada caracter de la pila
        post+= p.top();			
        p.pop();
    }
 	return post;
}

string Concatenando(int longt, string ex){
	string S_concatenado;
	i=0;
	for(i;i<longt;i++){
		S_concatenado+=ex[i];
		if(ex[i]==')' or ex[i]=='*'){
			if(i+1<longt){
				if(ex[i+1]!='*' && ex[i+1]!='|' && ex[i+1]!=')'){
					S_concatenado+='.';
				}
			}
		}
		else{
			if((ex[i]>='0' && ex[i]<='z')){
				if(ex[i+1]!='*' && ex[i+1]!='|' && ex[i+1]!=')' && i+1<longt){
					S_concatenado+='.';
				}
			}
		}
	}
	return S_concatenado;
}

int main(){
	string post;
	string expresion;
	cout<<"Ingrese la expresion\n";
	cin>>expresion;
	int longt=expresion.size();
	expresion=Concatenando(longt,expresion);
	post=postfija(expresion);
	AFN respuesta;
	respuesta=Proceso(post);
	respuesta.mostrar();
	return 0;
}
