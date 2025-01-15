/**
 *
 * @file main.cpp
 *
 * @brief
 *
 * Permite jugar al buscaminas en un tablero fijo, pero variable en el programa,
 * de 8x8.
 *
 * Datos de entrada: nombres de archivos: cadenas de caracteres; opcion de menu:
 * caracter; valores de celdas: enteros positivos pequenos.
 *
 * Datos de salida: tablero: caracteres, enteros positivos pequenos; menu,
 * mensajes finales: cadenas de caracteres.
 *
 * @author Adam Martinez
 * @date 01/15/2025
 *
 */

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/**
 *
 * Comprueba el sistema en que se ejecuta el programa, y configura el comando
 * para limpiar la pantalla pero no el buffer.
 *
 */
#ifdef _WIN32
    const string limpiar = "cls";
#else
    const string limpiar = "clear";
#endif

/**
 *
 * Estructura para registrar fechas en cualquier formato, con los datos de dia,
 * mes y anyo.
 *
 */
struct Fecha
{
    unsigned short mes;     ///< Mes de la fecha.
    unsigned short dia;     ///< Dia de la fecha.
    unsigned short anyo;    ///< Anyo de la fecha.
};

/**
 *
 * Estructura para almacenar datos particulares de cada jugador.
 *
 */
struct Jugador
{
    string nombre;             ///< Nombre del jugador.
    Fecha nacimiento;          ///< Fecha de nacimiento del jugador.
    unsigned short jugadas;    ///< Numero de intentos realizados en la partida.
};

/**
 *
 * Estructura para guardar el estado de cada celda del tablero.
 *
 */
struct Estado
{
    unsigned short n_minas;    ///< Numero de minas vecinas a la celda.
    bool mina;                 ///< Propiedad de celda conteniendo mina.
    bool destapada;            ///< Propiedad de celda destapada.
    bool bandera;              ///< Propiedad de celda marcada con bandera.
};

const unsigned short MAX_MINAS = 8;          ///< Maximo numero de minas.
const unsigned short MAX_JUGADORES = 100;    ///< Maximo numero de jugadores.
const unsigned short FIL = 8;                ///< Filas en el tablero.
const unsigned short COL = 8;                ///< Columnas en el tablero.

/**
 *
 * Array en que almacenar los datos de todos los jugadores, siendo 100 el
 * maximo.
 *
 */
typedef Jugador VectorJ[MAX_JUGADORES];
/**
 *
 * Array en que representar el estado del tablero, con datos particulares a cada
 * celda.
 *
 */
typedef Estado Tablero[FIL][COL];

/**
 *
 * Estructura para guardar temporalmente las posiciones de celdas; auxiliar a
 * ciertas funciones de cambio de celdas mayores a 1.
 *
 */
struct Posicion
{
    unsigned short fil[FIL];    ///< Valor auxiliar para cada fila.
    unsigned short col[COL];    ///< Valor auxiliar para cada columna.
};

char Menu(void);
void InicializaDesdeFichero(Tablero, ifstream &);
void InicializaAleatoriamente(Tablero);
unsigned short NumeroMinasVecinas(const Tablero, unsigned short,
                                  unsigned short);
void MuestraTablero(const Tablero);
void LeeCelda(unsigned short &, unsigned short &);
void AbreCelda(Tablero, unsigned short, unsigned short);
bool FinJuego(const Tablero);
bool MinaAbierta(const Tablero);
bool TodasCeldasProcesadas(const Tablero);
void LeeJugadoresFichero(VectorJ, unsigned short &, ifstream &);
Jugador LeeInfoJugador(unsigned short);
bool InsertaJugadorVector(Jugador, VectorJ, unsigned short &);
void EscribeJugadoresFichero(const VectorJ, unsigned short, ofstream &);

int main(void)
{
    char opcion;                    // Opcion elegida en el menu.
    unsigned short fil,             // Fila auxiliar que proveer a funciones.
                   col,             // Columna auxiliar que proveer a funciones.
                   tam = 0,         // Contador de jugadores registrados.
                   intentos = 0;    // Variable para numero de intentos.
    string f_nom;                   // Cadena para nombres de archivos.
    ifstream f_in;                  // Archivos para configuracion y guardado.
    ofstream f_out;                 // Archivo para guardado de nuevos datos.
    Jugador jug;                    // Jugador que ha acabado la partida.
    VectorJ jugadores;              // Informacion de jugadores de archivo.
    Tablero tablero;                // Informacion de celdas de todo el tablero.

    // Se limpia la terminal de comando para ejectuar el programa.
    system(limpiar.c_str());

    /*
     *
     * Logica principal primera; se itera en el menu hasta elegir una opcion
     * correcta. Al elegir una opcion disponible, se ejecuta la logica
     * correspondiente. Por cada iteracion del bucle, se comprobara si se ha
     * llegado a las condiciones de fin mediante la correspondiente funcion.
     *
     */
    do
    {
        opcion = Menu();

        cout << endl;

        switch (opcion)
        {
            case 'a':
                // Se obtiene el dato de entrada para el nombre del archivo.
                cout << "Introduzca el nombre del archivo con las posiciones de"
                    " las minas: ";
                cin >> f_nom;

                // Se sigue el procedimiento estandar para leer archivos.
                f_in.open(f_nom);

                if (!f_in)
                    cout << "Ha habido un error abriendo el archivo." << endl;
                else
                {
                    // Se inicializa el tablero con el archivo proveido.
                    InicializaDesdeFichero(tablero, f_in);

                    f_in.close();
                }

                break;
            case 'b':
                // Se inicializa el tablero de manera aleatoria.
                InicializaAleatoriamente(tablero);

                break;
            case 'c':
                LeeCelda(fil, col);

                // Evita repeticion en funcion llamando solo celdas tapadas.
                if (!tablero[fil][col].destapada)
                {
                    AbreCelda(tablero, fil, col);
                    intentos++;
                }

                break;
            case 'd':
                LeeCelda(fil, col);

                // Evita repeticion en funcion llamando solo celdas no marcadas.
                if (!tablero[fil][col].bandera)
                    tablero[fil][col].bandera = true;

                break;
            case 'e':
                LeeCelda(fil, col);

                // Evita repeticion en funcion llamando solo celdas marcadas.
                if (tablero[fil][col].bandera)
                    tablero[fil][col].bandera = false;
        }

        // Se limpian posibles mensajes impresos y se muestra el tablero.
        system(limpiar.c_str());

        MuestraTablero(tablero);

        cout << endl;
    }
    while (!FinJuego(tablero));

    /*
     *
     * Logica principal segunda; el fin del juego llega con dos posibles
     * condiciones, a las que les corresponden dos posibles mensajes.
     *
     */
    if (MinaAbierta(tablero))
        cout << "Se ha encontrado una mina. Fin del juego" << endl;
    else
        cout << "Se han marcado todas las minas. Se ha ganado el juego."
            << endl;

    // Independientemente del resultado, se muestran los intentos.
    cout << "Se ha(n) realizado " << intentos << " intento(s)." << endl << endl;

    /*
     *
     * Logica principal tercera; si se ha ganado el juego, se procede a la
     * lectura de un archivo con jugadores existentes, y al guardado del jugador
     * que ha terminado la presente partida en el mismo u otro archivo.
     *
     */
    if (TodasCeldasProcesadas(tablero))
    {
        // Se informa de las tareas que se llevaran a cabo.
        cout << "Se han procesado todas las celdas. Se procedera al guardado en"
            " un archivo existente." << endl << endl;

        // Se obtiene dato de entrada para el nombre del archivo de jugadores.
        cout << "Introduzca el nombre del archivo en que hay jugadores "
            "guardados: ";
        cin >> f_nom;

        f_in.open(f_nom);

        // Se sigue el procedimiento estandar para la lectura de archivos.
        if (!f_in)
            cout << "Ha habido un error abriendo el archivo.";
        else
        {
            // Se leen jugadores del fichero proveido en el vector jugadores.
            LeeJugadoresFichero(jugadores, tam, f_in);
            cout << "Se han procesado " << tam << " jugadores del archivo.";

            f_in.close();
        }

        cout << endl << endl;

        // Se obtiene informacion del ganador de la presente partida.
        jug = LeeInfoJugador(intentos);

        // Anade datos del nuevo jugador a aquellos de los jugadores de archivo.
        if (InsertaJugadorVector(jug, jugadores, tam))
            cout << "Jugador insertado correctamente.";
        else
            cout << "Memoria insuficiente para insertar jugador.";

        cout << endl << endl;

        // Se obtiene el dato de entrada para el nombre del archivo de guardado.
        cout << "Introduzca el nombre del archivo en que guardar la informacion"
            " actualizada de los jugadores: ";
        cin >> f_nom;

        // Se sigue el procedimiento estandar para la escritura en archivo.
        f_out.open(f_nom);

        if (!f_out)
            cout << "Ha habido un error abriendo el archivo." << endl;
        else
        {
            // Se escriben los datos de jugadores en el archivo proveido.
            EscribeJugadoresFichero(jugadores, tam, f_out);

            f_out.close();
        }
    }

    return 0;
}

/**
 *
 * Devuelve la opcion elegida de un menu. El menu ofrece la capacidad para
 * cargar minas desde archivo, cargar minas aleatoriamente, descubrir celdas de
 * un tablero inicializado, anadir banderas a celdas, y desmarcar banderas de
 * celdas.
 *
 * @return Caracter introducido por el usuario con la opcion elegida.
 *
 */
char Menu(void)
{
    char opcion = 'a';    // Opcion a elegir; inicializada por el formato.

    /*
     *
     * Logica principal; si una opcion ya esta fuera de rango, se inserta salto
     * de linea (formato) y se itera hasta obtener una opcion correcta.
     *
     */
    do
    {
        if (opcion < 'a' || opcion > 'e')
            cout << endl;

        cout << "a. Lee las posiciones de las minas desde archivo." << endl;
        cout << "b. Genera aleatoriamente las posiciones de las minas." << endl;
        cout << "c. Descubre una celda." << endl;
        cout << "d. Marca una celda como mina. Anade una bandera." << endl;
        cout << "e. Desmarca una celda como mina." << endl << endl;

        cout << "> ";
        cin >> opcion;
    }
    while (opcion < 'a' || opcion > 'e');

    return opcion;
}

/**
 *
 * Lee datos de un archivo abierto con formato fijo y los escribe en un array de
 * tablero. Las posiciones del archivo que no estan afectadas se inicializan con
 * valores por defecto, contrarios a los de las celdas-minas.
 *
 * @pre El archivo recibido debe estar abierto; se asume la ausencia de badbit,
 * failbit y eofbit.
 *
 * @pre El archivo debe contener posiciones validas, y un maximo de valores
 * ajustado al maximo de minas establecido; no se realizara ninguna
 * comprobacion.
 *
 * @param [out] tablero Array de filas y columnas a inicializar.
 * @param [in] f Archivo de entrada del que leer la informacion.
 *
 */
void InicializaDesdeFichero(Tablero tablero, ifstream & f)
{
    string linea;    // Variable para procesar cada linea del archivo.

    // Se inicializan posiciones del tablero con valores por defecto.
    for (int i = 0; i < FIL; i++)
        for (int j = 0; j < COL; j++)
        {
            tablero[i][j].mina = false;
            tablero[i][j].n_minas = 0;
            tablero[i][j].bandera = false;
            tablero[i][j].destapada = false;
        }

    /*
     *
     * Logica principal; se asignan datos del archivo a las posiciones
     * correspondientes del tablero. Se asume un formato fijo de datos en el
     * archivo.
     *
     */
    while (getline(f, linea))
        tablero[linea[0] - '0'][linea[2] - '0'].mina = true;

    return;
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
        minas_pos.fil[i] = 0;

    for (int i = 0; i < COL; i++)
        minas_pos.col[i] = 0;

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

unsigned short NumeroMinasVecinas(const Tablero tablero, unsigned short fil,
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
    {
        for (int j = 0; j < COL; j++)
            if (tablero[i][j].mina && tablero[i][j].destapada)
            {
                abierta = true;
                break;
            }

        if (abierta)
            break;
    }

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

void LeeJugadoresFichero(VectorJ jugadores, unsigned short & tam, ifstream & f)
{
    string linea;

    while (getline(f, linea))
    {
        if (linea[0] >= 'a' && linea[0] <= 'z')
            jugadores[tam].nombre = linea;
        else if (linea.find(' ') != linea.npos)
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
                        jugadores[tam].nacimiento.anyo = stoi(linea);
                }

                if (linea.find(' ') != linea.npos)
                    linea.erase(0, linea.find(' ') + 1);
            }
        else
        {
            jugadores[tam].jugadas = stoi(linea);
            tam++;
        }
    }

    return;
}

Jugador LeeInfoJugador(unsigned short intentos)
{
    Jugador jug = { .jugadas = intentos };

    for (int i = 0; i < 2; i++)
    {
        cout << "Introduzca ";

        switch (i)
        {
            case 0:
                cout << "el nombre del jugador: ";
                cin >> jug.nombre;

                break;
            case 1:
                cout << "la fecha de nacimiento del jugador:" << endl;

                cout << "Mes: ";
                cin >> jug.nacimiento.mes;
                cout << "Dia: ";
                cin >> jug.nacimiento.dia;
                cout << "Anyo: ";
                cin >> jug.nacimiento.anyo;
        }
    }

    return jug;
}

bool InsertaJugadorVector(Jugador jug, VectorJ jugadores, unsigned short & tam)
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

void EscribeJugadoresFichero(const VectorJ jugadores, unsigned short tam,
                             ofstream & f)
{
    for (int i = 0; i < tam; i++)
    {
        f << jugadores[i].nombre << endl;

        f << jugadores[i].nacimiento.dia << " "
          << jugadores[i].nacimiento.mes << " "
          << jugadores[i].nacimiento.anyo << endl;

        f << jugadores[i].jugadas << endl;
    }

    return;
}
