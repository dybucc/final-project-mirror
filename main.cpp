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

struct Posicion
{
    unsigned short fil[2];
    unsigned short col[2];
};

const unsigned short MAX_MINAS = 8;
const unsigned short MAX_JUGADORES = 100;
const unsigned short FIL = 8;
const unsigned short COL = 8;
typedef Jugador VectorJ[MAX_JUGADORES];
typedef Estado Tablero[FIL][COL];

char Menu();
void InicializarDesdeFichero(Tablero, ifstream &);
unsigned short NumeroMinasVecinas(const Tablero, unsigned short,
                                  unsigned short);

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

unsigned short NumeroMinasVecinas(const Tablero tablero,
                                  unsigned short fil,
                                  unsigned short col)
{
    unsigned short minas = 0;
    Posicion pos;

    switch (fil)
    {
        case 0:
            pos.fil[0] = fil;
            pos.fil[1] = fil + 1;
            break;
        case FIL:
            pos.fil[0] = fil - 1;
            pos.fil[1] = fil;
            break;
        default:
            pos.fil[0] = fil - 1;
            pos.fil[1] = fil + 1;
    }

    switch (col)
    {
        case 0:
            pos.col[0] = col;
            pos.col[1] = col + 1;
            break;
        case COL:
            pos.col[0] = col - 1;
            pos.col[1] = col;
            break;
        default:
            pos.col[0] = col - 1;
            pos.col[1] = col + 1;
    }

    for (int i = pos.fil[0]; i <= pos.fil[1]; i++)
        for (int j = pos.col[0]; j <= pos.col[1]; j++)
            if (tablero[i][j].mina == true)
                minas++;

    return minas;
}
