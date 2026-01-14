#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <conio.h>
#include <algorithm>
#include <windows.h>

using namespace std;

struct EntradaRanking {
    string nombre;
    int puntuacion;
};

struct Jugador {
    string nombre;
    int vida;
    vector <int> objetos; // 0: Lupa, 1: Manzana, 2: Muelle, 3: Tapon
    int dinero;
};

int generarAleatorio(int min, int max);
int esperarEnter(); // Retorna 1 al completarse

// --- Pantalla de carga ---
void mostrar_ascii();
void esperar_tecla();
void limpiar_pantalla();

// --- Gestión de Archivos y Ranking ---
bool guardarPuntuacion(string nombre, int dinero); // Retorna true si guardó con éxito
vector<EntradaRanking> leerRanking();
int mostrarRanking(); // Retorna la cantidad de registros mostrados

// --- Lógica del Juego ---
int calcularTotalBalas(int fase, int recargas);
int cargarRecamara(vector<int>& recamara, int fase, bool doubleOrNothing); // Retorna balas cargadas
int darObjetos(vector<int>& objetos, int fase); // Retorna cantidad total de objetos dados

// --- Combate y Turnos ---
int procesarDisparo(int bala, int objetivo, int& danoExtra, bool esJugador, int& vidaAtacante, int& vidaDefensor);
int turnoBot(const vector<int>& recamara, int indiceActual, int vidaBot, int vidaJugador);
int elegirObjetivo();
int mostrarMenuTurno(int fase, const vector<int>& objetos);

// --- Control de Flujo ---
int mostrarEstado(string nombreJugador, int vidaJugador, int vidaBot, int fase, int balasRestantes); // Retorna 1 (OK)
int jugarFase(Jugador& jugador, int fase, bool doubleOrNothing);
int iniciarPartidaNueva(); // Retorna puntuación final o -1
int mostrarMenuPrincipal();
int ejecutarJuego(); // Retorna código de salida (0 = Normal)

// --------------------------------------------------------------------------------
// 4. MAIN
// --------------------------------------------------------------------------------

int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(static_cast<unsigned int>(time(0)));

    mostrar_ascii();
    esperar_tecla();
    limpiar_pantalla();

    return ejecutarJuego();
}


int generarAleatorio(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int esperarEnter() {
    cout << "Presiona Enter para continuar...";
    cin.ignore();
    cin.get();
    return 1; // Código de éxito
}

// -- Pantalla de carga --
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

void esperar_tecla(){
    _getch();
}

void limpiar_pantalla(){
    system("cls");
}


// --- Gestión de Archivos ---

bool guardarPuntuacion(string nombre, int dinero) {
    vector <EntradaRanking> ranking = leerRanking();
    ranking.push_back({nombre, dinero});
    
    sort(ranking.begin(), ranking.end(), [](const EntradaRanking & a, const EntradaRanking & b) {
        return a.puntuacion > b.puntuacion;
    });
    
    if (ranking.size() > 10) ranking.resize(10);
    
    ofstream archivo("ranking.txt");
    if (archivo.is_open()) {
        for (const auto& entrada : ranking) {
            archivo << entrada.nombre << " " << entrada.puntuacion << endl;
        }
        archivo.close();
        return true; // Guardado exitoso
    }
    
    cout << "Error critico: No se pudo guardar el ranking." << endl;
    return false; // Fallo al guardar
}

vector<EntradaRanking> leerRanking() {
    vector<EntradaRanking> ranking;
    ifstream archivo("ranking.txt");
    string nombre;
    int pts;
    
    if (archivo.is_open()) {
        while (archivo >> nombre >> pts) {
            ranking.push_back({nombre, pts});
        }
        archivo.close();
    }
    return ranking;
}

int mostrarRanking() {
    vector<EntradaRanking> ranking = leerRanking();
    
    cout << "\n========================================" << endl;
    cout << "           RANKING TOP 10" << endl;
    cout << "========================================" << endl;
    
    if (ranking.empty()) {
        cout << "No hay registros aun." << endl;
        return 0; // 0 registros mostrados
    } 
    
    for (size_t i = 0; i < ranking.size(); i++) {
        cout << (i + 1) << ". " << ranking[i].nombre << " - $" << ranking[i].puntuacion << endl;
    }
    cout << "========================================" << endl;
    
    return ranking.size(); // Retorna cuántos mostró
}

// --- Lógica del Juego ---

int calcularTotalBalas(int fase, int recargas) {
    return 4 + fase + recargas;}

int cargarRecamara(vector <int> & recamara, int fase, bool doubleOrNothing){
    recamara.clear();
    int tamano = generarAleatorio(3, 8); 
    
    int minimo = (tamano * 20) / 100;
    if(minimo < 1) minimo = 1;
    
    int balasRojas = generarAleatorio(minimo, tamano - minimo);
    int balasAzules = tamano - balasRojas;
    
    if (doubleOrNothing && tamano > 2) balasRojas--; 

    cout << "\n>> INFORME DE INTELIGENCIA <<" << endl;
    cout << ">> " << balasRojas << " VIVAS (Rojas)" << endl;
    cout << ">> " << balasAzules << " FOGUEO (Azules)" << endl;
    if (doubleOrNothing && tamano > 2) cout << ">> 1 ESPECIAL (Dorada)" << endl;
    cout << ">> Insertando balas..." << endl;
    
    for (int i = 0; i < balasRojas; i++) recamara.push_back(1);
    for (int i = 0; i < balasAzules; i++) recamara.push_back(2);
    if (doubleOrNothing && tamano > 2) recamara.push_back(3);
    
    for (size_t i = recamara.size() - 1; i > 0; i--) {
        int j = generarAleatorio(0, i);
        int temp = recamara[i];
        recamara[i] = recamara[j];
        recamara[j] = temp;}
    
    return recamara.size(); // Devuelve cuántas balas se cargaron en total
}

int darObjetos(vector<int>& objetos, int fase){
    int totalDados = 0;
    for(int i = 0; i < 4; i++){
        int cantidad = generarAleatorio(0, fase);
        objetos[i] += cantidad;
        totalDados += cantidad;}

    return totalDados; // Retorna cuántos objetos nuevos recibió el jugador
}

int mostrarMenuTurno(int fase, const vector<int>& objetos){
    int opcion;
    cout << "\n--- TU TURNO ---" << endl;
    cout << "1. Disparar" << endl;
    
    if(fase >= 2){
        cout << "2. Usar Lupa (" << objetos[0] << ")" << endl;
        cout << "3. Usar Manzana (" << objetos[1] << ")" << endl;}

    if (fase >= 3){
        cout << "4. Usar Muelle (" << objetos[2] << ")" << endl;
        cout << "5. Usar Tapon (" << objetos[3] << ")" << endl;}
    
    cout << "Elige accion: ";
    cin >> opcion;
    return opcion;}

int elegirObjetivo(){
    int opcion;
    cout << "\nSelecciona objetivo:" << endl;
    cout << "1. Oponente" << endl;
    cout << "2. Tu mismo" << endl;
    cout << "> ";
    cin >> opcion;
    return opcion;}

int procesarDisparo(int bala, int objetivo, int & danoExtra, bool esJugador, int & vidaAtacante, int & vidaDefensor){
    int resultado = 0; 
    
    if (bala == 1) { 
        int dano = 1 + danoExtra;
        if (objetivo == 1){ 
             cout << "BANG! Fuego real! Dano: " << dano << endl;
             vidaDefensor -= dano;
             resultado = 1;} 
        else{ 
             cout << "BANG! Te disparaste fuego real! Dano: " << dano << endl;
             vidaAtacante -= dano;
             resultado = 1;}} 
        
    else if (bala == 2){ 
        cout << "Click... Bala de fogueo." << endl;
        resultado = 0;} 
        
    else if (bala == 3){ 
        int dano = 2 + danoExtra;
        if (objetivo == 1){ 
            cout << "BOOM! Municion Dorada! Dano masivo: " << dano << endl;
            vidaDefensor -= dano;
            resultado = 2;} 
        else{ 
            cout << "Brillo divino... Te curas 1 vida." << endl;
            vidaAtacante += 1;
            resultado = -1;}}
            
    return resultado;
}

int turnoBot(const vector<int>& recamara, int indiceActual, int vidaBot, int vidaJugador) {
    if (indiceActual >= recamara.size()) return 1;

    int balaActual = recamara[indiceActual];
    int probabilidad = generarAleatorio(1, 100);
    int decision = 2; // Por defecto a sí mismo (estrategia conservadora)

    if (balaActual == 2 && probabilidad > 30) decision = 2;
    else if (vidaJugador <= 1 && balaActual == 1) decision = 1;
    else if (probabilidad > 50) decision = 1;

    return decision;
}

int mostrarEstado(string nombreJugador, int vidaJugador, int vidaBot, int fase, int balasRestantes) {
    cout << "\n+-------------------------+" << endl;
    cout << "| FASE " << fase << " | BALAS: " << balasRestantes << endl;
    cout << "| " << nombreJugador << ": " << vidaJugador << " HP" << endl;
    cout << "| DEALER: " << vidaBot << " HP" << endl;
    cout << "+-------------------------+" << endl;
    return 1; // Estado renderizado correctamente
}

int jugarFase(Jugador& jugador, int fase, bool doubleOrNothing) {
    int vidaBot = 2 + fase;
    jugador.vida = 2 + fase; 
    
    vector<int> recamara;
    int indiceActual = 0;
    bool turnoJugador = true;
    int danoExtra = 0;
    int objetosUsados = 0;
    
    cargarRecamara(recamara, fase, doubleOrNothing);
    
    while (jugador.vida > 0 && vidaBot > 0) {
        
        if (indiceActual >= recamara.size()) {
            cout << "\n--- Recamara vacia, recargando ---" << endl;
            cargarRecamara(recamara, fase, doubleOrNothing);
            indiceActual = 0;
            
            if (fase >= 2) {
                int nuevos = darObjetos(jugador.objetos, fase);
                cout << "Recibiste " << nuevos << " objetos nuevos." << endl;
            }
        }
        
        mostrarEstado(jugador.nombre, jugador.vida, vidaBot, fase, (int)recamara.size() - indiceActual);
        
        if (turnoJugador) {
            int accion = mostrarMenuTurno(fase, jugador.objetos);
            
            if (accion == 1) { 
                int objetivo = elegirObjetivo();
                int vidaCopiaJ = jugador.vida;
                int vidaCopiaB = vidaBot;
                
                procesarDisparo(recamara[indiceActual], objetivo, danoExtra, true, jugador.vida, vidaBot);
                danoExtra = 0;
                
                if (recamara[indiceActual] == 2 && objetivo == 2) {
                    cout << "Turno extra para ti." << endl;
                    turnoJugador = true;
                } else {
                    turnoJugador = false;
                }
                indiceActual++;
                
            } else if (accion >= 2 && accion <= 5) {
                // Lógica simplificada de objetos para ahorrar espacio en este ejemplo
                int idxObj = accion - 2;
                if (fase >= 2 && jugador.objetos[idxObj] > 0) {
                    jugador.objetos[idxObj]--;
                    objetosUsados++;
                    
                    if (idxObj == 0) cout << "Lupa: La siguiente es " << (recamara[indiceActual] == 1 ? "ROJA" : (recamara[indiceActual] == 2 ? "AZUL" : "DORADA")) << endl;
                    if (idxObj == 1) { jugador.vida++; cout << "Manzana: +1 Vida" << endl; }
                    if (idxObj == 2) { danoExtra = 1; cout << "Muelle: +1 Dano proximo tiro" << endl; }
                    if (idxObj == 3) { cout << "Tapon: Bala saltada" << endl; indiceActual++; }
                } else {
                    cout << "No puedes usar eso." << endl;
                }
            } 
            
        } else { // Turno BOT
            cout << "\n[Turno del Dealer]" << endl;
            int decisionBot = turnoBot(recamara, indiceActual, vidaBot, jugador.vida);
            
            cout << (decisionBot == 1 ? "Dealer dispara hacia TI" : "Dealer se dispara a SI MISMO") << endl;
            
            procesarDisparo(recamara[indiceActual], (decisionBot == 1 ? 1 : 2), danoExtra, false, vidaBot, jugador.vida);
            
            if (recamara[indiceActual] == 2 && decisionBot == 2) {
                cout << "Dealer obtiene turno extra." << endl;
                turnoJugador = false;
            } else {
                turnoJugador = true;
            }
            indiceActual++;
        }
    }
    
    if (jugador.vida > 0) return jugador.dinero - (objetosUsados * 100);
    return -1;
}

int iniciarPartidaNueva() {
    Jugador jugador;
    jugador.dinero = 70000;
    jugador.objetos = {0, 0, 0, 0}; 
    
    cout << "\nNombre del jugador: ";
    cin >> jugador.nombre;
    
    int multiplicador = 1;
    
    for (int fase = 1; fase <= 3; fase++) {
        int resultado = jugarFase(jugador, fase, false);
        
        if (resultado == -1) {
            cout << "\nEliminado en fase " << fase << endl;
            return 0; // Retorna 0 dinero
        }
        if (fase < 3) esperarEnter();
    }
    
    cout << "\nVICTORIA PRINCIPAL. Dinero: $" << jugador.dinero << endl;
    
    char resp;
    cout << "Double or Nothing? (s/n): ";
    cin >> resp;
    
    while (resp == 's' || resp == 'S') {
        multiplicador *= 2;
        cout << "APUESTA x" << multiplicador << endl;
        
        if (jugarFase(jugador, 3, true) == -1) {
            cout << "Lo perdiste todo." << endl;
            return 0; 
        }
        
        jugador.dinero *= 2; // Simplificado: dobla lo que tenias
        cout << "Acumulado: $" << jugador.dinero << endl;
        
        if (guardarPuntuacion(jugador.nombre, jugador.dinero)) {
            cout << "(Progreso guardado)" << endl;
        }
        
        cout << "Seguir? (s/n): ";
        cin >> resp;
    }
    
    guardarPuntuacion(jugador.nombre, jugador.dinero);
    return jugador.dinero;
}

int mostrarMenuPrincipal() {
    int opcion;
    cout << "\n1. Jugar\n2. Ranking\n3. Salir\n> ";
    cin >> opcion;
    return opcion;
}

int ejecutarJuego() {
    bool activo = true;
    while (activo) {
        int op = mostrarMenuPrincipal();
        if (op == 1) iniciarPartidaNueva();
        else if (op == 2) mostrarRanking();
        else if (op == 3) activo = false;
    }
    cout << "Saliendo..." << endl;
    return 0; // Todo OK
}