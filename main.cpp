#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

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

struct Posicion
{
    unsigned short fil[FIL];
    unsigned short col[COL];
};

char Menu();
void InicializarDesdeFichero(Tablero, ifstream &);
unsigned short NumeroMinasVecinas(const Tablero, unsigned short,
                                  unsigned short);
void InicializaAleatoriamente(Tablero);
void MuestraTablero(const Tablero);
void LeeCelda(unsigned short &, unsigned short &);

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

void InicializaAleatoriamente(Tablero tablero)
{
    srand(time(NULL));

    unsigned short num_minas = rand() & MAX_MINAS + 1;
    Posicion minas_pos;

    for (int i = 0; i < FIL; i++)
    {
        minas_pos.fil[i] = 0;
        minas_pos.col[i] = 0;
    }

    for (int i = 0; i < num_minas; i++)
    {
        minas_pos.fil[rand() & MAX_MINAS + 1] = 1;
        minas_pos.col[rand() & MAX_MINAS + 1] = 1;
    }

    for (int i = 0; i < FIL; i++)
        for (int j = 0; j < COL; j++)
        {
            if (minas_pos.fil[i] == 1 && minas_pos.col[j] == 1)
                tablero[i][j].mina = true;
            else
                tablero[i][j].mina = false;

            tablero[i][j].n_minas = 0;
            tablero[i][j].bandera = false;
            tablero[i][j].destapada = false;
        }

    return;
}

void MuestraTablero(const Tablero tablero)
{
    for (int i = 0; i <= FIL; i++)
    {
        for (int j = 0; j <= COL; j++)
        {
            if (i == 0 || i == FIL)
                cout << '-';
            else
            {
                if (j == 0)
                    cout << "| ";

                if (tablero[i][j].destapada == true)
                {
                    if (tablero[i][j].n_minas > 0)
                        cout << tablero[i][j].n_minas;
                    else if (tablero[i][j].mina == true)
                        cout << '#';
                    else if (tablero[i][j].n_minas < 1 &&
                             tablero[i][j].mina == false)
                        cout << ' ';
                }
                else
                    cout << '.';

                cout << " |";
            }
        }

        cout << endl;
    }

    return;
}

void LeeCelda(unsigned short & fil, unsigned short & col)
{
    do
    {
        cout << "Introduzca el numero de fila:" << endl;
        cout << "> ";
        cin >> fil;

        cout << "Introduzca el numero de columna:" << endl;
        cout << "> ";
        cin >> col;
    }
    while (fil < 1 || fil > 8 || col < 1 || col > 8);

    return;
}
