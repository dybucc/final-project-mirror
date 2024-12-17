#include <iostream>
#include <fstream>
#include <stdlib.h>   
#include <ctime>

using namespace std;

//info del juego
const int MAX_MINAS = 8; 
const int FIL = 8;
const int COL = 8;

//info de los jugadores
struct Fecha 
{
	// completar
};

struct Jugador
{
   //completar
            
};

const int MAX_JUGADORES = 100;
typedef Jugador Vector[MAX_JUGADORES];

struct Estado
{
	unsigned int nMinas; // numero de bombas vecinas
	bool mina; //existe una mina en esa celda
	bool destapada; //la celda está descubierta ya o no
	bool bandera; //la celda está marcada con una bandera o no
};

typedef Estado Tablero[FIL][COL];


char Menu();
//funciones de inicializacion
void InicializaDesdeFichero(Tablero, ifstream &);
void InicializaAleatoriamente(Tablero);

//funciones para mostrar el tablero actual del juego
void MuestraTablero(const Tablero);
unsigned int NumeroMinasVecinas(const Tablero, unsigned int, unsigned int);

//funciones del juego
void LeeCelda(unsigned int &, unsigned int &);
void AbreCelda(Tablero, unsigned int,unsigned nt); 
bool FinJuego(const Tablero);

//funciones auxiliares
bool MinaAbierta(const Tablero);
bool TodasCeldasProcesadas(const Tablero);

//funciones del jugador
Jugador LeeInfoJugador(unsigned int);
void LeeJugadoresFichero(Vector, unsigned int &, ifstream &);

bool InsertaJugadorVector(Jugador, Vector, unsigned int &);
void EscribeJugadoresFichero(const Vector, unsigned int, ofstream &);
  
int main() 
{   
    char op; //opcion del menu
    Tablero tab;

    ifstream fIn, f; 
    ofstream fOut;
 
    unsigned int x, y, nIntentos = 0;

    Jugador p;
    Vector v; //vector con la info de los jugadores
    unsigned int tam = 0; //numero de elementos rellenados del vector
    string fichNombre;
    
    do 
    {
       op = Menu();
       switch (op) 
       { 
    
         case 'a': 
        	   cout << "Fichero con las posiciones de las minas?: " << endl;
        	   cin >> fichNombre;
			   fIn.open(fichNombre.c_str());
        	   
             if (fIn.fail())
                cout << "Error. Fichero no encontrado." << endl;
          	 else 
             {
            	   InicializaDesdeFichero (tab, fIn); 
                fIn.close();
             }
          	  break;

         case 'b': 
            InicializaAleatoriamente(tab); 
            break; 

         case 'c':
            LeeCelda(x,y);
            AbreCelda(tab, x, y); 
            nIntentos++; 
            break;

         case 'd':
            LeeCelda(x,y);
            tab[x][y].bandera = true;                 
            break;

         case 'e':
            LeeCelda(x,y);
            tab[x][y].bandera = false;               
            break;
       }
       MuestraTablero(tab); 
    } 
    while (FinJuego(tab) == false); 
    
  
    if  (MinaAbierta(tab) == true)
       cout << endl << "Fin Juego. Mina ha explotado!" << endl;
    else 
       cout << endl << "Fin Juego. Victoria!" << endl; 

    cout << "Numero de Intentos: " << nIntentos << endl;

    if (TodasCeldasProcesadas(tab) == true ) 
	{ 
       cout << "Fichero para leer la información jugadores?: " << endl;
       cin >> fichNombre;

       f.open(fichNombre.c_str());
	   if (!f)
          cerr << "Error.  No se puede abrir para lectura." << endl;
       else 
       {
          LeeJugadoresFichero(v, tam, f);
          cout << "El fichero contiene actualmente " << tam << " jugadores." << endl;
          f.close();
       }
       
      p = LeeInfoJugador(nIntentos);
       
      if (InsertaJugadorVector(p, v, tam) == false)
         cout << "No suficiente memoria para insertar el jugador." << endl;
      else 
         cout << "Jugador insertado correctamente." << endl;
       
      cout << "Fichero para guardar la información jugadores?: " << endl; 
      cin >> fichNombre;
	  fOut.open(fichNombre.c_str());
      
      if (fOut.fail())
        cerr << "Error. No se puede abrir el fichero para escritura." << endl;
      else 
      {
           EscribeJugadoresFichero(v, tam, fOut);
           fOut.close();
      }   
   }

   return 0;
}

// Funciones del Jugador
Jugador LeeInfoJugador(unsigned int n)
{      
  //completar
}   

void LeeJugadoresFichero(Vector vec, unsigned int &t, ifstream & f) 
{
   // completar  
}

bool InsertaJugadorVector(Jugador a, Vector vec, unsigned int & t) 
{
   // completar  
}

void EscribeJugadoresFichero(const Vector vec, unsigned int t, ofstream & f) 
{
    // completar      
}

/****** Funcion Menu ***********/
char Menu() 
{
 	// completar
}

/******* Funciones de Inicializacion *******/
void InicializaDesdeFichero(Tablero tab, ifstream & f) 
{   
    // completar
}

void InicializaAleatoriamente(Tablero tab) 
{
  // completar
}


/************** Mostrar Tablero *****************/
void MuestraTablero(const Tablero tab) 
{
    // completar  
} 

/*********** Funciones del Juego **********/
void LeeCelda(unsigned int & fil, unsigned int & col)
{
   // completar    
}
            
void AbreCelda(Tablero tab, unsigned int i, unsigned int j) 
{
   // completar    
}

bool FinJuego(const Tablero tab) 
{
   // completar
}

unsigned int NumeroMinasVecinas(const Tablero tab, unsigned int i, unsigned int j) 
{
	// completar
}

bool MinaAbierta(const Tablero tab) 
{    
    // completar
}

bool TodasCeldasProcesadas(const Tablero tab) 
{
    // completar
}
