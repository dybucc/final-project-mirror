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
