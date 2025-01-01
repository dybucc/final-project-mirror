#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

#ifdef _WIN32
    string limpiar = "cls";
#else
    string limpiar = "clear";
#endif

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
    unsigned int jugadas;
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
Jugador LeeInfoJugador(unsigned int);
bool InsertaJugadorVector(Jugador, VectorJ, unsigned short &);
void EscribeJugadoresFichero(const VectorJ, unsigned short, ofstream &);

int main(void)
{
    char opcion = 'a';
    unsigned short fil,
                   col,
                   tam = 0;
    unsigned int intentos = 0;
    string f_nom;
    ifstream f_in;
    ofstream f_out;
    Jugador jug;
    VectorJ jugadores;
    Tablero tablero;

    system(limpiar.c_str());

    do
    {
        opcion = Menu();

        cout << endl;

        switch (opcion)
        {
            case 'a':
                cout << "Introduzca el nombre del archivo con las "
                    "posiciones de las minas: ";
                cin >> f_nom;

                f_in.open(f_nom);

                if (!f_in)
                    cout << "Ha habido un error abriendo el archivo."
                        << endl;
                else
                {
                    InicializarDesdeFichero(tablero, f_in);

                    f_in.close();
                }

                break;
            case 'b':
                InicializaAleatoriamente(tablero);

                break;
            case 'c':
                LeeCelda(fil, col);
                if (!tablero[fil][col].destapada)
                    AbreCelda(tablero, fil, col);

                intentos++;

                break;
            case 'd':
                LeeCelda(fil, col);
                tablero[fil][col].bandera = true;

                break;
            case 'e':
                LeeCelda(fil, col);
                tablero[fil][col].bandera = false;
        }

        system(limpiar.c_str());

        MuestraTablero(tablero);

        cout << endl;
    }
    while (!FinJuego(tablero));

    if (MinaAbierta(tablero))
        cout << "Se ha encontrado una mina. Fin del juego" << endl;
    else
        cout << "Se han marcado todas las minas. Se ha ganado el juego."
            << endl;

    cout << "Se ha(n) realizado " << intentos << " intento(s)." << endl;

    if (TodasCeldasProcesadas(tablero))
    {
        cout << endl;
        cout << "Se han procesado todas las celdas. Se procedera al "
            "guardado en un archivo existente." << endl << endl;

        cout << "Introduzca el nombre del archivo en que hay jugadores "
            "guardados: ";
        cin >> f_nom;

        f_in.open(f_nom);

        if (!f_in)
            cout << "Ha habido un error abriendo el archivo." << endl;
        else
        {
            LeeJugadoresFichero(jugadores, tam, f_in);
            cout << "Se han procesado " << tam << " jugadores del "
                "archivo." << endl << endl;

            f_in.close();
        }

        jug = LeeInfoJugador(intentos);

        if (InsertaJugadorVector(jug, jugadores, tam))
            cout << "Jugador insertado correctamente." << endl;
        else
            cout << "Memoria insuficiente para insertar jugador."
                << endl;

        cout << endl;
        cout << "Introduzca el nombre del archivo en que guardar la "
            "informacion actualizada de los jugadores: ";
        cin >> f_nom;

        f_out.open(f_nom);

        if (!f_out)
            cout << "Ha habido un error abriendo el archivo." << endl;
        else
        {
            EscribeJugadoresFichero(jugadores, tam, f_out);

            f_out.close();
        }

        system(limpiar.c_str());
    }

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
        cout << "c. Descubre celda." << endl;
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
        tablero[linea[0] - '0'][linea[2] - '0'].mina = true;

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
        case FIL - 1:
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
        case COL - 1:
            pos.col[0] = col - 1;
            pos.col[1] = col;
            break;
        default:
            pos.col[0] = col - 1;
            pos.col[1] = col + 1;
    }

    for (int i = pos.fil[0]; i <= pos.fil[1]; i++)
        for (int j = pos.col[0]; j <= pos.col[1]; j++)
            if (tablero[i][j].mina && (i != fil || j != col))
                minas++;

    return minas;
}

void InicializaAleatoriamente(Tablero tablero)
{
    srand(time(NULL));

    unsigned short num_minas = 0,
                   tmp;
    Posicion minas_pos;

    while (num_minas == 0)
        num_minas = rand() % MAX_MINAS + 1;

    for (int i = 0; i < FIL; i++)
    {
        minas_pos.fil[i] = 0;
        minas_pos.col[i] = 0;
    }

    for (int i = 0; i < num_minas; i++)
    {
        do
            tmp = rand() % MAX_MINAS;
        while (minas_pos.fil[tmp] == 1);
        minas_pos.fil[tmp] = 1;

        do
            tmp = rand() % MAX_MINAS;
        while (minas_pos.col[tmp] == 1);
        minas_pos.col[tmp] = 1;
    }

    for (int i = 0; i < FIL; i++)
        for (int j = 0; j < COL; j++)
        {
            if (minas_pos.fil[i] == 1 && minas_pos.col[j] == 1)
            {
                tablero[i][j].mina = true;
                minas_pos.fil[i] = 0;
                minas_pos.col[j] = 0;
            }
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
    unsigned short esp_fil = to_string(FIL).length(),
                   esp_col = to_string(COL).length();

    for (int i = 0; i <= FIL; i++)
    {
        if (i == 0)
        {
            for (int j = 1; j <= COL; j++)
            {
                if (j == 1)
                {
                    cout << "    ";

                    for (int k = 0; k < esp_fil; k++)
                        cout << ' ';
                }

                cout << j << "   ";
            }

            cout << endl;
        }
        else
        {
            cout << "  ";

            for (int j = 0; j < esp_fil; j++)
                cout << ' ';

            for (int j = 0; j <= FIL * (3 + esp_col); j++)
                cout << '-';

            cout << endl;

            cout << ' ' << i;

            for (int j = 0; j < esp_fil; j++)
                cout << ' ';

            for (int j = 0; j < COL; j++)
            {
                cout << "| ";

                if (tablero[i - 1][j].destapada)
                {
                    if (tablero[i - 1][j].mina)
                        cout << '#';
                    else if (tablero[i - 1][j].n_minas > 0)
                        cout << tablero[i - 1][j].n_minas;
                    else
                        cout << ' ';
                }
                else if (tablero[i - 1][j].bandera)
                    cout << '^';
                else
                    cout << '.';

                for (int k = 0; k < esp_col; k++)
                    cout << ' ';
            }

            cout << '|' << endl;
        }
    }

    cout << "  ";

    for (int i = 0; i < esp_fil; i++)
        cout << ' ';

    for (int i = 0; i <= FIL * (3 + esp_col); i++)
        cout << '-';

    cout << endl;

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

    fil--;
    col--;

    return;
}

void AbreCelda(Tablero tablero, unsigned short fil, unsigned short col)
{
    tablero[fil][col].destapada = true;

    tablero[fil][col].n_minas = NumeroMinasVecinas(tablero, fil, col);

    if (tablero[fil][col].n_minas == 0)
    {
        if (fil - 1 >= 0 && col - 1 >= 0 &&
            NumeroMinasVecinas(tablero, fil - 1, col - 1) == 0 &&
            !tablero[fil - 1][col - 1].destapada)
            AbreCelda(tablero, fil - 1, col - 1);
        if (fil - 1 >= 0 &&
            NumeroMinasVecinas(tablero, fil - 1, col) == 0 &&
            !tablero[fil - 1][col].destapada)
            AbreCelda(tablero, fil - 1, col);
        if (fil - 1 >= 0 && col + 1 < COL &&
            NumeroMinasVecinas(tablero, fil - 1, col + 1) == 0 &&
            !tablero[fil - 1][col + 1].destapada)
            AbreCelda(tablero, fil - 1, col + 1);
        if (col - 1 >= 0 &&
            NumeroMinasVecinas(tablero, fil, col - 1) == 0 &&
            !tablero[fil][col - 1].destapada)
            AbreCelda(tablero, fil, col - 1);
        if (col + 1 < COL &&
            NumeroMinasVecinas(tablero, fil, col + 1) == 0 &&
            !tablero[fil][col + 1].destapada)
            AbreCelda(tablero, fil, col + 1);
        if (fil + 1 < FIL && col - 1 >= 0 &&
            NumeroMinasVecinas(tablero, fil + 1, col - 1) == 0 &&
            !tablero[fil + 1][col - 1].destapada)
            AbreCelda(tablero, fil + 1, col - 1);
        if (fil + 1 < FIL &&
            NumeroMinasVecinas(tablero, fil + 1, col) == 0 &&
            !tablero[fil + 1][col].destapada)
            AbreCelda(tablero, fil + 1, col);
        if (fil + 1 < FIL && col + 1 < COL &&
            NumeroMinasVecinas(tablero, fil + 1, col + 1) == 0 &&
            !tablero[fil + 1][col + 1].destapada)
            AbreCelda(tablero, fil + 1, col + 1);
    }
    
    return;
}

bool FinJuego(const Tablero tablero)
{
    bool mina,
         proc,
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
    unsigned short num_procesadas = 0;

    for (int i = 0; i < FIL; i++)
        for (int j = 0; j < COL; j++)
            if (tablero[i][j].destapada ||
                tablero[i][j].mina && tablero[i][j].bandera)
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
            jugadores[tam].nombre = linea.erase(linea.find('\n'));
        if (linea[0] >= 'a' && linea[0] <= 'z')
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

Jugador LeeInfoJugador(unsigned int intentos)
{
    Jugador jug = { "", 0, 0, 0, intentos };

    cin.ignore();

    for (int i = 0; i < 2; i++)
    {
        cout << "Introduzca ";

        switch (i)
        {
            case 0:
                cout << "el nombre del jugador: ";
                getline(cin, jug.nombre);
                break;
            case 1:
                cout << "la fecha de nacimiento del jugador:" << endl;

                for (int j = 1; j < 3; j++)
                    switch (j)
                    {
                        case 0:
                            cout << "Mes: ";
                            cin >> jug.nacimiento.mes;
                            break;
                        case 1:
                            cout << "Dia: ";
                            cin >> jug.nacimiento.dia;
                            break;
                        case 2:
                            cout << "Anyo: ";
                            cin >> jug.nacimiento.anyo;
                    }
        }
    }

    return jug;
}

bool InsertaJugadorVector(Jugador jug, VectorJ jugadores,
                          unsigned short & tam)
{
    bool mem = false;

    if (tam < MAX_JUGADORES)
    {
        mem = true;

        jugadores[tam].nombre = jug.nombre;
        jugadores[tam].jugadas = jug.jugadas;
        jugadores[tam].nacimiento = jug.nacimiento;

        tam++;
    }

    return mem;
}

void EscribeJugadoresFichero(const VectorJ jugadores,
                             unsigned short tam, ofstream & f)
{
    for (int i = 0; i < tam; i++)
    {
        f << jugadores[i].nombre << endl;

        f << jugadores[i].nacimiento.dia << endl;
        f << jugadores[i].nacimiento.mes << endl;
        f << jugadores[i].nacimiento.anyo << endl;

        f << jugadores[i].jugadas << endl;
    }

    return;
}
