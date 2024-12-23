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
void AbreCelda(Tablero, unsigned short, unsigned short);
bool FinJuego(const Tablero);
bool MinaAbierta(const Tablero);
bool TodasCeldasProcesadas(const Tablero);
void LeeJugadoresFichero(VectorJ, unsigned short &, ifstream &);

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

void AbreCelda(Tablero tablero, unsigned short fil, unsigned short col)
{
    if (tablero[fil][col].destapada == false &&
        tablero[fil][col].mina == false)
        tablero[fil][col].destapada = true;

    if (tablero[fil][col].n_minas == 0)
    {
        if (fil != 0 && col != 0 && fil != FIL && col != COL ||
            fil == FIL && col <= COL && col != 0 ||
            fil != 0 && fil != FIL && col == COL)
            AbreCelda(tablero, fil - 1, col - 1);
        if (fil != 0 && col != 0 && fil != FIL && col != COL ||
            fil == FIL ||
            fil != 0 && fil != FIL && col >= 0)
            AbreCelda(tablero, fil - 1, col);
        if (fil != 0 && col != 0 && fil != FIL && col != COL ||
            fil == FIL && col >= 0 && col != COL ||
            fil != 0 && fil != FIL && col == 0)
            AbreCelda(tablero, fil - 1, col + 1);
        if (fil != 0 && col != 0 && fil != FIL && col != COL ||
            fil == 0 && col <= COL && col != 0 ||
            fil != 0 && fil != FIL && col == COL)
            AbreCelda(tablero, fil, col - 1);
        if (fil != 0 && col != 0 && fil != FIL && col != COL ||
            fil == 0 && col >= 0 && col != COL ||
            fil != 0 && fil != FIL && col == 0)
            AbreCelda(tablero, fil, fil + 1);
        if (fil != 0 && col != 0 && fil != FIL && col != COL ||
            fil == 0 && col <= COL && col != 0 ||
            fil != 0 && fil != FIL && col == COL)
            AbreCelda(tablero, fil + 1, col - 1);
        if (fil != 0 && col != 0 && fil != FIL && col != COL ||
            fil == 0 ||
            fil != 0 && fil != FIL && col >= 0)
            AbreCelda(tablero, fil + 1, col);
        if (fil != 0 && col != 0 && fil != FIL && col != COL ||
            fil == 0 && col != COL ||
            fil != 0 && fil != FIL && col == 0)
            AbreCelda(tablero, fil + 1, col + 1);
    }
    
    return;
}

bool FinJuego(const Tablero tablero)
{
    bool mina = false,
         proc = false,
         fin = false;

    mina = MinaAbierta(tablero);
    proc = TodasCeldasProcesadas(tablero);

    if (mina || proc)
        fin = true;

    return fin;
}

bool MinaAbierta(const Tablero tablero)
{
    bool abierta = false;

    for (int i = 0; i < FIL; i++)
        for (int j = 0; j < COL; j++)
            if (tablero[i][j].mina == true &&
                tablero[i][j].destapada == true)
                abierta = true;

    return abierta;
}

bool TodasCeldasProcesadas(const Tablero tablero)
{
    bool procesadas = false;
    unsigned short num_procesadas;

    for (int i = 0; i < FIL; i++)
        for (int j = 0; j < COL; j++)
            if (tablero[i][j].destapada == true ||
                tablero[i][j].bandera == true)
                num_procesadas++;

    if (num_procesadas == FIL * COL)
        procesadas = true;

    return procesadas;
}

void LeeJugadoresFichero(VectorJ jugadores, unsigned short & tam,
                         ifstream & f)
{
    string linea;

    while (getline(f, linea))
    {
        if (linea[0] > 'a' && linea[0] < 'z')
            jugadores[tam].nombre = linea.erase(linea.find('\n'));
        else if (linea.find(' ') != linea.npos)
        {
            for (int i = 0; i < 3; i++)
            {
                switch (i)
                {
                    case 0:
                        jugadores[tam].nacimiento.dia =
                            stoi(linea.substr(0, linea.find(' ')));
                        break;
                    case 1:
                        jugadores[tam].nacimiento.mes =
                            stoi(linea.substr(0, linea.find(' ')));
                        break;
                    case 2:
                        jugadores[tam].nacimiento.anyo =
                            stoi(linea.substr(0, linea.find('\n')));
                }

                if (linea.find(' ') != linea.npos)
                    linea.erase(0, linea.find(' '));
            }
        }
        else
        {
            jugadores[tam].partidas = stoi(linea.erase('\n'));
            tam++;
        }
    }

    return;
}
