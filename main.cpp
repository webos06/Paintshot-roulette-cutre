#include <iostream>
#include <vector>
#include <string>
#include <ctime>   
#include <cstdlib>  
#include <fstream>
#include <algorithm>
using namespace std;

struct EntradaRanking {
    string nombre;
    int puntuacion;
};

struct Jugador {
    string nombre;
    int vidas;
    vector<int> objetos;
};

//Funcion principal
int jugar_juego();

int NumRandom(int minimo, int maximo);
int enterusuario();

// Ranking
bool Puntuacion(string nombre, int monedas);
vector<EntradaRanking> Rankingtexto();
int listaranking();

// Logica del juego
int municion_total(int fasejuego, int recargaescopeta);
int municion_cargador(vector <int> & recamara, int fasejuego, bool dobleonada);
int objetos_usuario(vector <int> & objetos, int fasejuego);

// Disparos y turnos
int disparo(int bala, int enemigo, int & dobleimpacto, bool quejugador, int & vidadisparador, int & vidavictima);
int bot_turno(const vector <int> & recamara, int indiceactual, int vidabot, int vidajugador);
int elegir_disparo();
int mostrar_menu_turno(int fase, const vector <int> & objetos);

// Control del flujo
int mostrar_estado(string nombrejugador, int vidajugador, int vidabot, int fase, int balasquefaltan);
int jugarfase(Jugador & jugador, int fase, bool dobleonada);
int iniciar_partida_nueva();
int mostrar_menu_principal();

int main() {
    srand( static_cast <unsigned> ( time(0)) );
    
    jugar_juego();
    
    return 0;
}

