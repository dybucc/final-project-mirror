#include <iostream>
#include <fstream>

using namespace std;

struct Fecha
{
    unsigned short mes;
    unsigned short dia;
    unsigned short anyo;
};

struct Jugador
{
    string nombre;
    Fecha nacimiento;
    unsigned int partidas;
};

struct Estado
{
    unsigned short n_minas;
    bool mina;
    bool destapada;
    bool bandera;
};

const unsigned short MAX_MINAS = 8;
const unsigned short MAX_JUGADORES = 100;
const unsigned short FIL = 8;
const unsigned short COL = 8;
typedef Jugador VectorJ[MAX_JUGADORES];
typedef Estado Tablero[FIL][COL];

char Menu();
void InicializarDesdeFichero(Tablero, ifstream &);

int main(void)
{


    return 0;
}

char Menu()
{
    char opcion = 'a';

    do
    {
        if (opcion < 'a' || opcion > 'e')
            cout << endl;

        cout << "a. Lee las posiciones de las minas desde fichero."
            << endl;
        cout << "b. Genera aleatoriamente las posiciones de las minas."
            << endl;
        cout << "c. Descube celda." << endl;
        cout << "d. Marca celda como una mina. Anade una bandera."
            << endl;
        cout << "e. Desmarca celda como una mina." << endl << endl;

        cout << "> ";
        cin >> opcion;
    }
    while (opcion < 'a' || opcion > 'e');

    return opcion;
}

void InicializarDesdeFichero(Tablero tablero, ifstream & f)
{
    string linea;

    for (int i = 0; i < FIL; i++)
        for (int j = 0; j < COL; j++)
        {
            tablero[i][j].mina = false;
            tablero[i][j].n_minas = 0;
            tablero[i][j].bandera = false;
            tablero[i][j].destapada = false;
        }

    while (getline(f, linea))
        tablero[linea[0]][linea[2]].mina = true;

    return;
}
