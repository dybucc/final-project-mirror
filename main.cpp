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

/**
 *
 * Asigna datos aleatorios a ciertas posiciones de un array tablero. El resto de
 * posiciones se inicializan con valores por defecto, contrarios a los de las
 * celdas-minas.
 *
 * @param [out] tablero Array de filas y columnas a inicializar.
 *
 */
void InicializaAleatoriamente(Tablero tablero)
{
    // Se inicializa la semilla para generar numeros semideterministicos.
    srand(time(NULL));

    unsigned short num_minas = 0,    // Numero de minas aleatorias.
                   tmp;              // Variable auxiliar.
    Posicion minas_pos;              // Posiciones auxiliares de minas.

    // Se itera hasta conseguir un numero de minas no nulo.
    while (num_minas == 0)
        num_minas = rand() % MAX_MINAS + 1;

    // Se inicializan las posiciones auxiliares con valores falsos (0).
    for (int i = 0; i < FIL; i++)
        minas_pos.fil[i] = 0;

    for (int i = 0; i < COL; i++)
        minas_pos.col[i] = 0;

    // Itera por cantidad de minas, para obtener minas en posiciones diferentes.
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

    /*
     *
     * Logica principal; se itera el tablero para inicializar con valores por
     * defecto todas las celdas cuando no haya posiciones de minas restantes que
     * colocar. Si hay posiciones de minas que colocar, determinadas por el
     * valor 1 en su array, se coloca una mina en la posicion correspondiente
     * del tablero, y se elimina el 1 del array de posiciones de minas.
     *
     */
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

/**
 *
 * Devuelve el numero de celdas vecinas que contienen una mina en un array
 * tablero.
 *
 * @param [in] tablero Array de filas y columnas del que leer datos de celdas.
 * @param [in] fil Fila de la celda desde la que determinar minas vecinas.
 * @param [in] col Columna de la celda desde la que determinar minas vecinas.
 *
 * @return Numero de minas vecinas de la celda proveida.
 *
 */
unsigned short NumeroMinasVecinas(const Tablero tablero, unsigned short fil,
                                  unsigned short col)
{
    unsigned short minas = 0;    // Numero de posibles minas vecinas.
    Posicion pos;                // Rangos posibles de celdas a explorar.

    /*
     *
     * Dependiendo de la posicion de la celda proveida, solo sera posible
     * explorar unos rangos u otros a su alrededor.
     *
     */
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

    /*
     *
     * Logica principal; se recorren los rangos determinados previamente, y se
     * exceptua la iteracion que coincide con la fila y columna proveida.
     *
     */
    for (int i = pos.fil[0]; i <= pos.fil[1]; i++)
        for (int j = pos.col[0]; j <= pos.col[1]; j++)
            if (tablero[i][j].mina && (i != fil || j != col))
                minas++;

    return minas;
}

/**
 *
 * Imprime por pantalla la situacion del tablero de valores preinicializados. La
 * impresion del tablero soporta capacidad para incrementar el numero de filas y
 * columnas del juego, si asi se desease, aunque esto esta fuera de los
 * requerimientos del proyecto.
 *
 * @pre El tablero debe tener sus valores inicializados; tomar especial atencion
 * con valores enteros.
 *
 * @param [in] tablero Array de filas y columnas del que leer datos.
 *
 */
void MuestraTablero(const Tablero tablero)
{
    // Variables auxiliares para la longitud de caracteres de filas y columnas.
    unsigned short esp_fil = to_string(FIL).length(),
                   esp_col = to_string(COL).length();

    /*
     *
     * Logica principal; se recorren los valores de todas las filas del tablero
     * mas una, la inicial, que corresponde a los indices numericos de ayuda
     * visual para las columnas. Para todo el resto de filas, se imprimen los
     * indices visuales de filas, y los caracteres correspondientes al estado de
     * cada celda.
     *
     */
    for (int i = 0; i <= FIL; i++)
    {
        // Si la fila es la primera, se muestran los indices de columnas.
        if (i == 0)
        {
            for (int j = 1; j <= COL; j++)
            {
                /*
                 *
                 * La primera iteracion debe considerar dos tipos de espaciado:
                 * el siempre presente, i.e. 4 espacios que corresponden a los
                 * dos guiones de las celdas inferiores, el espacio entre el
                 * indice visual de filas y la barra vertical del tablero, y el
                 * espacio entre el borde de la pantalla y el indice visual de
                 * filas; y el segundo tipo, dependiente de la longitud de
                 * caracteres del numero de filas que se imprimiran como indices
                 * visuales; si solo tiene 8 filas, como requiere el proyecto,
                 * solo se insertara 1 espacio, pero si el tablero tuviese 10
                 * filas, se insertarian 2 espacios, y asi sucesivamente.
                 *
                 */
                if (j == 1)
                {
                    cout << "    ";

                    for (int k = 0; k < esp_fil; k++)
                        cout << ' ';
                }

                /*
                 *
                 * El espaciado entre indices de columnas es de 3 espacios, i.e.
                 * esto determina el numero de espacios que procesaran los
                 * contenidos de cada celda tras mostrar el caracter que les
                 * corresponde. Siempre habra una garantia de que al menos se
                 * anadiran 3 espacios, y dependiendo de la longitud de
                 * caracteres del numero de columnas, espacios adicionales se
                 * anadiran.
                 *
                 */
                cout << j << "   ";
            }

            cout << endl;
        }
        // Si la fila es cualquier otra, se muestran los indices y el tablero.
        else
        {
            /*
             *
             * Similar al mecanismo de espaciado anterior. Se deben contar con
             * dos tipos de espaciados: el siempre presente, i.e. dos espacios
             * correspondientes al espacio entre el indice visual y la barra
             * vertical del tablero, y un espacio entre el borde de la pantalla
             * y el indice visual; y el tipo de espacio dependiente de la
             * longitud del numero de caracteres del numero de fila, que de ser
             * 1 para un tablero de 8 filas, solo anade 1 espacio, pero de ser
             * un tablero con 10 filas, anade 2 espacios.
             *
             */
            cout << "  ";

            for (int j = 0; j < esp_fil; j++)
                cout << ' ';

            /*
             *
             * Para computar el numero de guiones necesarios, se considera el
             * numero de guiones garantizados a imprimirse por celda, i.e. dos
             * previos a la posicion del indice de columnas, y uno tras el
             * indice de columnas; y el tipo de espacio dependiente del indice
             * de columnas, i.e. una proporcion directa con el numero de
             * caracteres del indice de columnas, por lo que si son 8 columnas,
             * 1 espacio, pero si son 10 columnas, 2 espacios.
             *
             */
            for (int j = 0; j <= FIL * (3 + esp_col); j++)
                cout << '-';

            cout << endl;

            // Se imprime el indice visual correspondiente a las filas.
            cout << ' ' << i;

            /*
             *
             * Se aplica la misma logica de espacios que la anterior; se anaden
             * espacios directamente relacionados con el numero de caracteres
             * del numero de filas del tablero.
             *
             */
            for (int j = 0; j < esp_fil; j++)
                cout << ' ';

            // Se imprimen los caracteres correspondientes a cada celda.
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

                /*
                 *
                 * Se anaden tantos espacios como caracteres tenga el numero
                 * maximo de columnas. Si son 8 columnas, 1 espacio precedara la
                 * barra vertical de la siguiente celda; si son 10 columnas,
                 * habra 2 espacios de separacion entre el caracter que designa
                 * el estado de la celda y la barra vertical de la siguiente
                 * celda. Este comportamiento no es estetico pero si es
                 * uniforme; para cada celda, siempre habra un solo espacio
                 * entre la barra vertical y el estado de la celda, pero un
                 * numero variable de espacios entre el estado de celda y la
                 * siguiente barra vertical.
                 *
                 */
                for (int k = 0; k < esp_col; k++)
                    cout << ' ';
            }

            cout << '|' << endl;
        }
    }

    // Se repite el procedimiento de espaciado de la primera fila de guiones.
    cout << "  ";

    for (int i = 0; i < esp_fil; i++)
        cout << ' ';

    for (int i = 0; i <= FIL * (3 + esp_col); i++)
        cout << '-';

    cout << endl;

    return;
}

/**
 *
 * Procesa celdas introducidas por teclado por el usuario.
 *
 * @param [out] fil Numero de la fila para la celda a leer.
 * @param [out] col Numero de la columna para la celda a leer.
 *
 */
void LeeCelda(unsigned short & fil, unsigned short & col)
{
    /*
     *
     * Logica principal; se preguntan datos de la celda hasta obtener un rango
     * correcto.
     *
     */
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

    /*
     *
     * Para facilitar el procesamiento del programa, y dejar tambien que el
     * usuario introduzca indices de celdas mas naturales, se restan valores.
     * Esto es conforme con los indices visuales iniciados en 1 al mostrar el
     * tablero.
     *
     */
    fil--;
    col--;

    return;
}

/**
 *
 * Modifica el estado de posiciones de un array tablero. Explora recursivamente
 * las celdas vecinas si estas no estan en contacto con posiciones que contengan
 * minas. Utiliza una logica similar al algoritmo flood-fill en 8 direcciones.
 *
 * @pre El tablero debe tener sus valores inicializados; tomar especial atencion
 * con valores enteros.
 *
 * @param [in, out] tablero Array del que leer y modificar estados de celda(s).
 * @param [in] fil Numero de fila para la celda desde la que explorar.
 * @param [in] col Numero de columna para la celda desde la que explorar.
 *
 */
void AbreCelda(Tablero tablero, unsigned short fil, unsigned short col)
{
    // Se destapa la posicion del tablero automaticamente.
    tablero[fil][col].destapada = true;

    // Se consulta el numero de minas vecinas de la celda por iteracion.
    tablero[fil][col].n_minas = NumeroMinasVecinas(tablero, fil, col);

    /*
     *
     * Logica principal; si no hay minas alrededor, se procede a comprobar
     * colisiones en 8 direcciones. Las colisiones se comprueban por un orden
     * que permite cortocircuitar la expresion booleana si un requerimiento
     * basico no se cumple. Si la celda es explorable porque existe, relativa a
     * la celda con la que la iteracion esta trabajando, se pasa a comprobar si
     * tiene minas vecinas, y si ese no es el caso, si no esta tapada. A notar
     * es la comprobacion por minas vecinas previo a la nueva llamada recursiva;
     * parece redundante, pero asegura no revelar celdas que tengan contacto con
     * minas, lo que permite que el juego no sea tan sencillo.
     *
     */
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

/**
 *
 * Comprueba si la partida jugada ha finalizado mediante dos posibles escenarios
 * explorados en otras funciones.
 *
 * @see MinaAbierta()
 * @see TodasCeldasProcesadas()
 *
 * @param [in] tablero Array de filas y columnas del que leer datos de celdas.
 *
 * @retval true Se ha finalizado la partida.
 * @retval false La partida aun puede continuar.
 *
 */
bool FinJuego(const Tablero tablero)
{
    bool mina,           // Condicion de mina destapada.
         proc,           // Condicion de tablero completo procesado.
         fin = false;    // Condicion de fin; modificada si corresponde.

    // Se comprueba si alguno de los posibles finales se ha alcanzado.
    mina = MinaAbierta(tablero);
    proc = TodasCeldasProcesadas(tablero);

    // Si algun final se ha alcanzado, se modifica el valor de retorno absoluto.
    if (mina || proc)
        fin = true;

    return fin;
}

/**
 *
 * Devuelve la condicion de fin por existir una celda con mina destapada en el
 * tablero proveido.
 *
 * @param [in] tablero Array de filas y columnas del que leer datos de celdas.
 *
 * @retval true No hay ninguna celda-mina destapada.
 * @retval false Hay una mina destapada.
 *
 */
bool MinaAbierta(const Tablero tablero)
{
    bool abierta = false;    // Condicion de mina encontrada.

    /*
     *
     * Logica principal; se recorre el array tablero hasta encontrar una celda
     * destapada que contenga una mina. En el momento en que se encuentra, se
     * para el recorrido.
     *
     */
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

/**
 *
 * Devuelve la condicion de fin por haber destapado todas las celdas del tablero
 * sin minas, y haber colocado una bandera en todas las celdas con minas.
 *
 * @param [in] tablero Array de filas y columnas del que leer datos de celdas.
 *
 * @retval true Numero de celdas procesadas es igual a celdas totales.
 * @retval false Numero de celdas procesadas no es igual a celdas totales.
 *
 */
bool TodasCeldasProcesadas(const Tablero tablero)
{
    bool procesadas = false;              // Condicion de fin.
    unsigned short num_procesadas = 0;    // Contador de celdas procesadas.

    /*
     *
     * Logica principal; se recorre el array en busqueda de celdas destapadas o
     * celdas-minas con banderas. Si ese es el caso, se incrementa el contador
     * de celdas procesadas.
     *
     */
    for (int i = 0; i < FIL; i++)
        for (int j = 0; j < COL; j++)
            if (tablero[i][j].destapada ||
                tablero[i][j].mina && tablero[i][j].bandera)
                num_procesadas++;

    // Si el contador de celdas procesadas es igual al total de celdas, fin.
    if (num_procesadas == FIL * COL)
        procesadas = true;

    return procesadas;
}

/**
 *
 * Lee jugadores de un archivo de entrada de formato fijo, y almacena los datos
 * encontrados en un array de jugadores.
 *
 * @pre El archivo recibido debe estar abierto; se asume que no hay ningun
 * badbit, failbit o eofbit.
 *
 * @pre El archivo debe contener un numero de jugadores en el rango 0-100 para
 * evitar que el vector no recoja todos los jugadores. Los datos de los
 * jugadores tambien deben ser conformes con los datos del archivo de ejemplo;
 * no se realizara ninguna comprobacion.
 *
 * @param [out] jugadores Array de jugadores en el que escribir datos.
 * @param [out] tam Tamano del array rellenado con los jugadores del archivo.
 * @param [in] f Archivo de entrada con jugadores existentes.
 *
 */
void LeeJugadoresFichero(VectorJ jugadores, unsigned short & tam, ifstream & f)
{
    string linea;    // Variable para procesar lineas del archivo.

    /*
     *
     * Logica principal; se procesan lineas, que se asumen estar en el formato
     * estandar dado con los documentos de ejemplo. Se aprovechan
     * caracteristicas de cada linea de datos para procesar diferentes tipos. Al
     * final de cada set de 3 lineas, se incrementa el tamano del array.
     *
     */
    while (getline(f, linea))
    {
        /*
         *
         * Si el primer caracter de la linea coincide con un caracter de letras
         * ASCII, se asume que se trata del nombre del jugador. Todo nombre de
         * jugador se espera en minuscula, de acuerdo con el documento de
         * ejemplo.
         *
         */
        if (linea[0] >= 'a' && linea[0] <= 'z')
            jugadores[tam].nombre = linea;
        /*
         *
         * Si la linea contiene algun espacio, debe ser la fecha, puesto que
         * cada elemento esta separado por espacios, de acuerdo al documento de
         * ejemplo. En ese caso, se procesan datos hasta el primer espacio
         * encontrado en la cadena de la linea, y se elimina ese mismo rango de
         * la cadena tras alamacenarlo.
         *
         */
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
        /*
         *
         * El unico otro caso es el de el numero de jugadas, que no contiene
         * letras ni espacios, de acuerdo al documento de ejemplo.
         *
         */
        else
        {
            jugadores[tam].jugadas = stoi(linea);
            tam++;
        }
    }

    return;
}

/**
 *
 * Recoge informacion por teclado del usuario sobre el jugador que ha acabado la
 * partida.
 *
 * @param [in] intentos Numero de jugadas/intentos realizados en la partida.
 *
 * @return Estructura de jugador con datos rellenados.
 *
 */
Jugador LeeInfoJugador(unsigned short intentos)
{
    // Estructura de jugador con intentos inicializados.
    Jugador jug = { .jugadas = intentos };

    // Logica principal; se preguntan los datos del jugador para la estructura.
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
