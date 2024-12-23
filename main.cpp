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

int main(void)
{


    return 0;
}
