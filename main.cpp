#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <conio.h>
#include <windows.h>
using namespace std;

struct EntradaRanking {
    string nombre;
    int puntuacion;
};

struct Jugador {
    string nombre;
    int vida;
    vector <int> objetos;
    int dinero;
};

int generarAleatorio(int min, int max);
void esperar(int milisegundos);
int leerEntrada(int min, int max);
void lineaSeparadora();

//Estetica
void mostrar_ascii();
void esperar_tecla();
void limpiar_pantalla();

//Gestion de almacenamiento y ranking, no tocar.
bool guardarPuntuacion(string nombre, int dinero); 
vector<EntradaRanking> leerRanking();
int mostrarRanking(); 

//Todas las funciones para jugar el juego, avisar de cambio.
int calcularTotalBalas(int fase, int recargas);
int cargarRecamara(vector<int> & recamara, int fase, bool doubleOrNothing); 
int darObjetos(vector<int> & objetos, int fase); 
void contarBalasRestantes(const vector<int> & recamara, int indiceActual, int & rojas, int & azules, int & doradas);

// Dentro de la partida, turnos , disparos....
int procesarDisparo(int bala, int objetivo, int & danoExtra, bool esJugador, int & vidaAtacante, int & vidaDefensor);
int turnoBot(const vector<int> & recamara, int indiceActual, int vidaBot, int vidaJugador);
int elegirObjetivo();
int mostrarMenuTurno(int fase, const vector<int> & objetos, int rojas, int azules);

//Control de entrada y de flujo, intentar no modificar nada.
int mostrarEstado(string nombreJugador, int vidaJugador, int vidaBot, int fase, int rojas, int azules); 
int jugarFase(Jugador & jugador, int fase, bool doubleOrNothing);
int iniciarPartidaNueva(); 
int mostrarMenuPrincipal();
int ejecutarJuego(); 

int main(){
    SetConsoleOutputCP(CP_UTF8);
    srand(static_cast<unsigned int>(time(0)));

    mostrar_ascii();
    esperar_tecla();
    limpiar_pantalla();

    return ejecutarJuego();
}

int generarAleatorio(int min, int max){
    return rand() % (max - min + 1) + min;
}

void esperar(int milisegundos){
    Sleep(milisegundos);
}

int leerEntrada(int min, int max){
    int valor;
    while(true){
        if(cin >> valor){
          if(valor >= min && valor <= max){
            return valor;} 
            else{
            cout << " >> Opcion fuera de rango (" << min << "-" << max << "). Intenta de nuevo: ";}} 
        else{
            cout << " >> Entrada invalida. Introduce un numero: ";
            cin.clear();
            cin.ignore(10000, '\n');}}
}

void lineaSeparadora(){
    cout << "+--------------------------------------------------+" << "\n";
}

void esperar_tecla(){
    cout << "Presiona cualquier tecla para continuar";
    _getch();
}

void limpiar_pantalla(){
    system("cls");
}

void mostrar_ascii(){
      cout<<R"(
     ______   ______     __     __   __     ______   ______     __  __     ______     ______       
    /\  == \ /\  __ \   /\ \   /\ "-.\ \   /\__  _\ /\  ___\   /\ \_\ \   /\  __ \   /\__  _\      
    \ \  _-/ \ \  __ \  \ \ \  \ \ \-.  \  \/_/\ \/ \ \___  \  \ \  __ \  \ \ \/\ \  \/_/\ \/      
    \ \_\    \ \_\ \_\  \ \_\  \ \_\\"\_\    \ \_\  \/\_____\  \ \_\ \_\  \ \_____\    \ \_\      
    \/_/     \/_/\/_/   \/_/   \/_/ \/_/     \/_/   \/_____/   \/_/\/_/   \/_____/     \/_/ 
        ______      ______     __  __     __         ______     ______   ______   ______   
        /\  == \   /\  __  \   /\ \/\ \   /\ \       /\  ___\   /\__  _\ /\__  _\ /\  ___\  
        \ \  __<   \ \ \/\ \  \ \ \_\ \  \ \ \____  \ \  __\   \/_/\ \/ \/_/\ \/ \ \  __\  
        \ \_\ \_\  \ \_____\  \ \_____\  \ \_____\  \ \_____\    \ \_\    \ \_\  \ \_____\
        \/_/ /_/   \/_____/   \/_____/   \/_____/   \/_____/     \/_/     \/_/   \/_____/  

    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣿⠟⠉⠉⠻⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣾⠿⠉⠀⠀⠀⠀⠀⠹⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣾
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣾⡿⠛⠉
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣾⡿⠟⠁⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣴⣿⡿⠃⠀⠀       ⠀⠀⠀⠀⠀⢸⣿⣷⣶⣶⣦⣤⣤⣄⡀⠀⢀⣠⣾⣿⠿⠋⠀⠀⠀⠀⠀⣠
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⠶⠞⣿⠟⠋⠉⠉⠙⣻⠿⢿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⢀⡼⠁
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⠁⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠟⠁⢠⡞⠁⠀⠀⠀⢀⡴⠋⠀⢀⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⡞⠁⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣼⣿⡄⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⣼⠃⠀⢠⠏⠀⠀⠀⠀⣰⠏⠀⠀⣠⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡥⠤⡀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⣇⠀⣠⡏⠀⠀⠀⠀⣼⠁⠀⠀⣰⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⠁
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⣄⣄⣬⣿⠉⠀⠀⠀⠀⠀⠀⠀⠀⢸⡅⠀⠀⠀⠀⠀⢻⡶⠋⠀⠀⠀⠀⢸⣧⠀⠀⣴⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢧⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠀⠀⠀⠀⡴⡾⠟⣹⡟⠁⠀⠀⠀⠀⠀⠀⠀⣀⣾⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠛⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳
    ⠀⠀⠀⠀⠀⠀⢀⣘⠿⠙⠿⢟⡳⣀⡀⡶⠃⠀⠈⠀⠀⠀⠀⠀⠒⠾⠿⣿⣿⣿⣿⣯⠿⠷⠖⠒⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⡠⣤⣦⣼⣿⠀⠀⠀⣼⢻⢿⢻⣿⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣷⠏⠀⠀⠀⢀⣀⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⣾⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀
    ⠀⣠⣾⡿⠋⠉⠉⠁⠀⠀⠀⠀⠈⢮⡘⠻⣿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⢹⣿⠀⠀⢀⡤⠖⠋⠉⠉⠉⠉⠉⠉⠒⠦⣄⠀⠀⠀ ⢠⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀
    ⡦⣿⠋⠀⠀⠀⠀⡀⣀⠀⠀⠀⠀⠀⠙⢦⣄⠉⠻⢿⣿⣷⣦⡀⠀⠀⠀⠀⠀⡿⠀⢰⠟⡇⠀⠀⠀⢲⡆⠀⠀⠀⠀⠀⠈⠳⣄⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
    ⡿⡆⠀⠀⠀⣴⠏⢫⣳⣽⣀⠀⠀⠀⠀⡶⢌⠙⠶⢤⡈⠛⠿⣿⣷⣦⣀⠀⠇⠀⠀⡇⠀⢫⣄⠀⠀⣹⢷⠀⠀⠀⠀⠀⡶⠀⠘⡆⠀⠀⠻⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀
    ⣿⡇⠀⠀⢸⣟⢨⣟⣾⣿⡟⠀⠀⠀⠀⡇⠀⠈⠛⠦⣝⡳⢤⣈⠛⠻⣯⣷⣦⣀⠀⠀⠀⠀⠘⠛⠛⠃⠀⠛⠦⠤⠤⠚⠁⠀⠀⢳⠀⠀⠀⠈⠛⠿⠿⠿⠟⠋⠀⠀⠀⠀⠀⠀⠀
    ⣿⡇⠀⠀⠈⢯⣞⣿⣿⢺⠏⠀⠀⠀⠀⡁⠀⠀⠀⠀⠀⠙⠱⢬⣓⠦⠀⠙⢻⣿⣷⣢⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⣿⠆⠀⠀⠀⠀⠀⠚⠋⠁⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⠀⠀⠈⣿⠉⢻⣿⣷⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⣿⡃⠀⠀⠀⠀⠀⣠⣄⠀⠀⢰⠶⠒⠒⢧⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⢸⡄⢸⡟⣿⣷⣦⣴⣶⣶⣶⣶⣤⣔⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⣿⡂⠀⠀⣠⠀⠀⠿⠷⠁⢀⡽⠀⠀⠀⡄⠈⠙⡷⢦⣄⡀⠀⠀⠀⠀⠀⠀⠀⣿⠀⢸⡆⢸⡗⠀⣿⠙⣟⣿⣉⠉⠙⠿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⣿⡇⠀⠀⠙⠶⢠⣀⣠⠴⠘⠁⠀⠀⠀⠇⠀⠀⡇⢸⡏⢹⡷⢦⣄⡀⠀⠀⠀⣿⡀⢸⡇⢿⡆⠀⡟⠀⢸⠀⢹⡳⢦⣄⢘⣿⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⣿⣿⠢⣤⡀⠀⠀⠀⠀⠀⠀⣠⠾⣿⣿⡷⣤⣀⡇⠸⡇⢸⡇⢸⠉⠙⠳⢦⣄⡻⢿⣾⣧⢸⣧⠀⡇⠀⢸⠀⢸⡆⢠⣈⠙⠺⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⢹⣿⣷⣌⡉⠚⠲⢶⣖⠖⠛⠛⢶⣄⡉⠓⠿⣽⣿⣶⣧⣸⠇⢸⠀⠀⠀⠀⠈⠙⠲⢮⣝⠻⣿⣷⣷⣄⣸⠀⢸⡇⠀⠈⠁⠀⢨⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠈⠙⠻⢿⣷⣶⣤⣉⡻⢶⣄⣀⠈⠙⠳⠦⢈⡉⠺⢿⣿⣷⣺⣦⡀⠀⠀⠀⠀⠀⠀⠈⠐⠲⢭⢛⠿⣿⣷⣼⡇⠀⠀⠀⠀⠈⣿⡃⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⣀
    ⠀⠀⠀⠀⠀⠈⠙⠻⢿⣿⣳⣶⣽⣻⡦⠀⠐⠌⠉⠷⣦⣌⡙⠻⢿⣟⣷⣤⣀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠳⢫⣻⡇⠀⠀⠀⠀⠀⢸⣿⠀⣀⠀⠀⠀⠀⠈⠳⣄⠀⠀⠀⢀⡏⠙⠛
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠚⠻⢾⣿⣿⣿⣶⣤⣤⡤⣀⣈⠛⠷⣤⣈⠛⠶⢽⡻⢲⣄⣀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠳⢤⣀⠀⠀⢸⣿⡀⠈⠳⢤⣀⣀⣰⠃⠈⠛⠶⠶⠿⠃⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢈⣿⡿⠛⠉⠙⠚⠚⠻⢷⣦⣄⣩⣿⠶⠖⠛⠛⠛⠛⠛⠛⠿⢷⣶⣦⣄⠀⠀⠀⠀⠉⢻⣶⣿⣿⠇⠀⠀⠀⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⠁⠀⠀⠀⠀⠀⠀⠀⣿⣿⠋⠀⠀⠀⠀⠀⣠⠖⠂⠀⠀⠀⠈⠙⠿⣿⣦⡄⠀⠀⣸⣿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⣰⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⢿⣶⣄⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⢸⣧⠀⠀⢀⠞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡠⠙⢿⣿⣇⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡿⠿⠀⠀⠀⠂⠀⠀⠀⠀⠀⠘⡿⠦⠠⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠧⠤⠄⠙⡿⠿⠦⠤⠤⠤⠤⠄⠀
    )";
}

bool guardarPuntuacion(string nombre, int dinero){
vector <EntradaRanking> ranking = leerRanking();
ranking.push_back({nombre, dinero});

int n = ranking.size();
   for(int i = 0; i < n - 1; i++){
    for(int j = 0; j < n - i - 1; j++){
       if(ranking[j].puntuacion < ranking[j + 1].puntuacion){
        EntradaRanking auxiliar = ranking[j];
        ranking[j] = ranking[j + 1];
        ranking[j + 1] = auxiliar;}}}
    
    if(ranking.size() > 10) ranking.resize(10);
      ofstream archivo("ranking.txt");
      if(archivo.is_open()){
        for(int i = 0; i < ranking.size(); i++){
        archivo << ranking[i].nombre << " " << ranking[i].puntuacion << "\n";}
        archivo.close();
        return true;}

cout << "Error: No se pudo guardar el ranking." << "\n";
return false; 
}

vector<EntradaRanking> leerRanking(){

vector<EntradaRanking> ranking;
ifstream archivo("ranking.txt");
string nombre;
int pts;
    
    if(archivo.is_open()){
      while(archivo >> nombre >> pts){
        ranking.push_back({nombre, pts});}
        archivo.close();}
    return ranking;
}

int mostrarRanking(){
    vector<EntradaRanking> ranking = leerRanking();
    limpiar_pantalla();
    lineaSeparadora();
    cout << "               RANKING TOP 10" << "\n";
    lineaSeparadora();
    
    if(ranking.empty()){
      cout << " No hay nadie que ha podido vencer" << "\n";}
    else{
        for(int i = 0; i < ranking.size(); i++){
        cout << " " << (i + 1) << ". " << ranking[i].nombre << "\t\t $" << ranking[i].puntuacion << "\n";}}

    lineaSeparadora();
    esperar_tecla();
    return ranking.size(); 
}

int cargarRecamara(vector <int> & recamara, int fase, bool doubleOrNothing){
    recamara.clear();
    int tamano = generarAleatorio(3, 8); 
    
    int minimo = (tamano * 20) / 100;
    if(minimo < 1) minimo = 1;
    
    int balasRojas = generarAleatorio(minimo, tamano - minimo);
    int balasAzules = tamano - balasRojas;
    
    if(doubleOrNothing && tamano > 2) balasRojas--; 

    cout << "\n>> CARGANDO ESCOPETA..." << "\n";
    esperar(500);
    cout << ">> " << balasRojas << " PERDIGONES [ROJAS]" << "\n";
    esperar(300);
    cout << ">> " << balasAzules << " FOGUEO [AZULES]" << "\n";
    esperar(300);
    if(doubleOrNothing && tamano > 2) cout << ">> 1 ESPECIAL [DORADA]" << "\n";
    
    cout << ">> Insertando y barajando..." << "\n";
    esperar(1000);
    
    for(int i = 0; i < balasRojas; i++) recamara.push_back(1);
    for(int i = 0; i < balasAzules; i++) recamara.push_back(2);
    if(doubleOrNothing && tamano > 2) recamara.push_back(3);
    
    for(int i = recamara.size() - 1; i > 0; i--){
        int j = generarAleatorio(0, i);
        int auxiliar = recamara[i];
        recamara[i] = recamara[j];
        recamara[j] = auxiliar;
    }
    
    return recamara.size(); 
}

void contarBalasRestantes(const vector <int> & recamara, int indiceActual, int & rojas, int & azules, int & doradas) {
    rojas = 0;
    azules = 0;
    doradas = 0;
    for(int i = indiceActual; i < recamara.size(); i++){
        if (recamara[i] == 1) rojas++;
        else if (recamara[i] == 2) azules++;
        else if (recamara[i] == 3) doradas++;}
}

int darObjetos(vector<int> & objetos, int fase){
    int totalDados = 0;
    cout << "Repartiendo objetos..." << "\n";
    esperar(500);
    
    for(int i = 0; i < 2; i++){ 
        int tipoObjeto = generarAleatorio(0, 3); 
        objetos[tipoObjeto]++;
        totalDados++;
        
        string nombreObj;
        if(tipoObjeto == 0) nombreObj = "Lupa";
        else if(tipoObjeto == 1)
        nombreObj = "Manzana";
        else if(tipoObjeto == 2)
        nombreObj = "Muelle";
        else nombreObj = "Tapon";
        
        cout << " -> Has recibido: " << nombreObj << "\n";
        esperar(200);}

    return totalDados; 
}

int mostrarMenuTurno(int fase, const vector <int> & objetos, int rojas, int azules){
int maxOpcion;
    cout << "\n===================================" << "\n";
    cout << "       TU TURNO" << "\n";
    cout << " Balas restantes: " << rojas << " ROJAS | " << azules << " AZULES" << "\n";
    cout << "===================================" << "\n";
    cout << " [1] DISPARAR" << "\n";
    
    if(fase >= 2){
        cout << " --- OBJETOS ---" << "\n";
        cout << " [2] Lupa    (Tienes " << objetos[0] << ")" << "\n";
        cout << " [3] Manzana (Tienes " << objetos[1] << ")" << "\n";
        cout << " [4] Muelle  (Tienes " << objetos[2] << ")" << "\n";
        cout << " [5] Tapon   (Tienes " << objetos[3] << ")" << "\n";
    }

    cout << "===================================" << "\n";
    cout << " Elige accion > ";
    
    if(fase >= 2){
        maxOpcion = 5;} 
    else{
        maxOpcion = 1;}

    return leerEntrada(1, maxOpcion);
}

int elegirObjetivo(){
    cout << "\n A quien apuntas?" << "\n";
    cout << " [1] Al Bot" << "\n";
    cout << " [2] A ti mismo (Si es de fogueo, repites turno)" << "\n";
    cout << " > ";
    return leerEntrada(1, 2);
}

int procesarDisparo(int bala, int objetivo, int & danoExtra, bool esJugador, int & vidaAtacante, int & vidaDefensor){
    int resultado = 0; 
    
    cout << "\n ...apretando el gatillo..." << "\n";
    esperar(1000);
    
    if(bala == 1){ 
        int dano = 1 + danoExtra;
        cout << "PUUUUM PERDIGONES." << "\n";
        if (objetivo == 1){ 
             cout << " -> Impacto de " << dano << " de daño al objetivo." << "\n";
             vidaDefensor -= dano;
             resultado = 1;} 
        else{ 
             cout << " -> Te has disparado. Sufres " << dano << " de daño." << "\n";
             vidaAtacante -= dano;
             resultado = 1;}} 
        
    else if(bala == 2){ 
        cout << " *Click*... Bala de fogueo." << "\n";
        resultado = 0;} 
        
    else if(bala == 3){ 
        int dano = 3 + danoExtra;
        if(objetivo == 1){ 
             cout << " ¡BOOM! MUNICION DORADA QUE LOCURA." << "\n";
            vidaDefensor -= dano;
            resultado = 2;} 
        else{ 
            cout << " *Vaya suerte*... La bala dorada te cura 1 vida." << "\n";
            vidaAtacante += 1;
            resultado = -1;}}
            
    esperar(1000); 
    return resultado;
}

int turnoBot(const vector<int> & recamara, int indiceActual, int vidaBot, int vidaJugador) {
    cout << "Preparando para ganarte...";
    esperar(1500); 
    cout << "\n"; 

    if(indiceActual >= recamara.size()) return 1;

    int balaActual = recamara[indiceActual];
    int probabilidad = generarAleatorio(1, 100);
    int decision = 2; 

    if(balaActual == 2 && probabilidad > 30) decision = 2; 
    else if(vidaJugador <= 1 && balaActual == 1) decision = 1; 
    else if(probabilidad > 50) decision = 1; 

    return decision;
}

int mostrarEstado(string nombreJugador, int vidaJugador, int vidaBot, int fase, int rojas, int azules) {
    limpiar_pantalla();
    lineaSeparadora();
    cout << "       FASE " << fase << "  |  PENDIENTES: " << rojas << " R / " << azules << " A" << "\n";
    lineaSeparadora();
    cout << "  " << nombreJugador << "\t\tVS\t\tBOT" << "\n";
    cout << "  VIDA: " << vidaJugador << "\t\t\t\tVIDA: " << vidaBot << "\n";
    lineaSeparadora();
    return 1; 
}

int jugarFase(Jugador & jugador, int fase, bool doubleOrNothing) {
int vidaBot = 2 + fase;
jugador.vida = 2 + fase; 
    
vector<int> recamara;
int indiceActual = 0; 
bool turnoJugador = true;
int danoExtra = 0;
int objetosUsados = 0;
    
cargarRecamara(recamara, fase, doubleOrNothing);

while(jugador.vida > 0 && vidaBot > 0){
    if(indiceActual >= recamara.size()){
      cout << "\n--- Recamara vacia, recargando ---" << "\n";
        esperar(1000);
        cargarRecamara(recamara, fase, doubleOrNothing);
        indiceActual = 0;
            
    if(fase >= 2){
        darObjetos(jugador.objetos, fase);}}
        
    int rojas = 0, azules = 0, doradas = 0;
    contarBalasRestantes(recamara, indiceActual, rojas, azules, doradas);
    mostrarEstado(jugador.nombre, jugador.vida, vidaBot, fase, rojas, azules);
        
    if(turnoJugador){
        int accion = mostrarMenuTurno(fase, jugador.objetos, rojas, azules);
            
    if(accion == 1){ 
      int objetivo = elegirObjetivo();
        procesarDisparo(recamara[indiceActual], objetivo, danoExtra, true, jugador.vida, vidaBot);
        danoExtra = 0;
                
    if(recamara[indiceActual] == 2 && objetivo == 2){
        cout << " >> Turno extra para ti." << "\n";
        esperar(1000);
        turnoJugador = true;}
    else{
        turnoJugador = false;}
        indiceActual++;}
        
    else if(accion >= 2 && accion <= 5){ 
        int indiceObjeto = accion - 2;
            if(fase >= 2 && jugador.objetos[indiceObjeto] > 0){
            jugador.objetos[indiceObjeto]--;
            objetosUsados++;
                    
    if(indiceObjeto == 0){
        cout << " [LUPA] Miras dentro de la recamara..." << "\n";
        esperar(1000);
        cout << " La siguiente bala es: ";
        if(recamara[indiceActual] == 1) cout << "ROJA (PERDIGONES)";
          else if (recamara[indiceActual] == 2) cout << "AZUL (FOGUEO)";
                else cout << "DORADA (ESPECIAL)";
                cout << "\n";
                esperar(2000);}

    if(indiceObjeto == 1){ 
      jugador.vida++; 
      cout << " [MANZANA] Comes y recuperas 1 vida." << "\n"; 
      esperar(1000);}
                
    if(indiceObjeto == 2){ 
        danoExtra = 1;
        cout << " [MUELLE] Cortas la escopeta. Proximo tiro +1 daño." << "\n";
        esperar(1000);}

    if(indiceObjeto == 3){ 
      cout << " [TAPON] Extraes la bala actual sin disparar." << "\n"; 
        indiceActual++; 
        esperar(1000);}}
    else{
        cout << " ¡No tienes ese objeto!" << "\n";
        esperar(1000);}}} 
                
    else{
        cout << "\n   [TURNO DEL BOT]" << "\n";
        int decisionBot = turnoBot(recamara, indiceActual, vidaBot, jugador.vida);
            
        if (decisionBot == 1) cout << " >> BOT te apunta a TI." << "\n";
        else cout << " >> BOT se apunta a SI MISMO." << "\n";
        esperar(1000);

        int objetivoReal;
        if(decisionBot == 1) objetivoReal = 1;
        else objetivoReal = 2;
        procesarDisparo(recamara[indiceActual], objetivoReal, danoExtra, false, vidaBot, jugador.vida);
            
        if(recamara[indiceActual] == 2 && decisionBot == 2) {
        cout << " >> BOT obtiene turno extra." << "\n";
        esperar(1000);
        turnoJugador = false;}
    
    else{
        turnoJugador = true;}
        indiceActual++;}}
    
    if (jugador.vida > 0) return jugador.dinero - (objetosUsados * 100);
    return -1;
}

int iniciarPartidaNueva(){
int multiplicador = 1;
Jugador jugador;
jugador.dinero = 70000;
jugador.objetos = {0, 0, 0, 0};
    
limpiar_pantalla();
lineaSeparadora();
cout << " FIRMA TU CONTRATO DE RENUNCIA DE ALMA (UNA VEZ FIRMADO NADA SERA IGUAL)" << "\n";
lineaSeparadora();
cout << " Introduce tu usuario: ";
cin >> jugador.nombre;

for(int fase = 1; fase <= 3; fase++){
    limpiar_pantalla();
    cout << "\n\n --- COMIENZA LA FASE " << fase << " --- \n\n";
    esperar(2000);
        
int resultado = jugarFase(jugador, fase, false);
    if (resultado == -1) {
    cout << "Te ha eliminado un bot jajajaja, te ha matado en la fase " << fase << "\n";
    esperar_tecla();
    return 0;}
    jugador.dinero = resultado;

if(fase < 3){
    cout << "\n >> Has superado la fase, pero no te confies, se esta preparando siguiente ronda..." << "\n";
    esperar(2000);}}
    
cout << "\n VICTORIAAAAAA. Dinero: $" << jugador.dinero << "\n";
    
bool seguirApostando = true;

while(seguirApostando){
    cout << " ¿Doble o nada? (s/n) ----- (No te cagues hombre): ";
    char respuesta;
    cin >> respuesta;
        
if(respuesta == 'n' || respuesta == 'N'){
    seguirApostando = false;}
    else if (respuesta == 's' || respuesta == 'S') {
    multiplicador *= 2;
    cout << " ¡APUESTA x" << multiplicador << "! Preparando mesa..." << "\n";
    esperar(1500);
            
int resultadoDoble = jugarFase(jugador, 3, true);
    if(resultadoDoble == -1) {
        cout << " Lo perdiste todo por ansia." << "\n";
        esperar_tecla();
        return 0;}
            
    jugador.dinero *= 2; 
    cout << " Acumulado actual: $" << jugador.dinero << "\n";}
    else{
        cout << " Entrada invalida." << "\n";}}
    
if(guardarPuntuacion(jugador.nombre, jugador.dinero)){
    cout << "Has acabado la partida, ya esta todo en el ranking (;" << "\n";}
    esperar_tecla();
    return jugador.dinero;
}

int mostrarMenuPrincipal(){
    limpiar_pantalla();
    mostrar_ascii();
    lineaSeparadora();
    cout << " 1. JUGAR" << "\n";
    cout << " 2. RANKING" << "\n";
    cout << " 3. SALIR" << "\n";
    lineaSeparadora();
    cout << " > ";
    return leerEntrada(1, 3);
}

int ejecutarJuego() {
    bool activo = true;
    while(activo){
        int op = mostrarMenuPrincipal();
        if(op == 1){
        iniciarPartidaNueva();}
        else if
        (op == 2)mostrarRanking();
        else if
        (op == 3) activo = false;
    }
    cout << " Cerrando juego..." << "\n";
    return 0; 
}