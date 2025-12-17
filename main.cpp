#include <iostream>
#include <clocale>
#include <locale.h>
using namespace std;

void mostrar_menu();
void procesar_opcio_menu(int opcio);
void triar_opcio_menu();

int main(){
    triar_opcio_menu();
    return 0;
}

void mostrar_menu(){
    cout << "  ____       _       _   ____  _           _   " << endl;
    cout << " |  _ \\ __ _(_)_ __ | |_/ ___|| |__   ___ | |_ " << endl;
    cout << " | |_) / _` | | '_ \\| __\\___ \\| '_ \\ / _ \\| __|" << endl;
    cout << " |  __/ (_| | | | | | |_ ___) | | | | (_) | |_ " << endl;
    cout << " |_|   \\__,_|_|_| |_|\\__|____/|_| |_|\\___/ \\__|" << endl;
    cout << "          P A I N T S H O T  R O U L E T T E    " << endl;
    cout << " ---------------------------------------------- " << endl;
    cout << "  1. Jugar contra CPU                           " << endl;
    cout << "  2. Jugar 1 Vs. 1                              " << endl;
    cout << "  3. Mostrar normas                             " << endl;
    cout << "  4. Sortir                                     " << endl;
    cout << " ---------------------------------------------- " << endl;
    cout << " >> Tria una opcio del menu: ";
}

void procesar_opcio_menu(int opcio){
    switch (opcio) {
        case 1: cout<<"Has triat jugar contra la CPU"<<endl; break;
        case 2: cout<<"Has triat jugar 1 Vs. 1"<<endl; break;
        case 3: cout<<"Has triat mostrar les normes"<<endl; break;
        case 4: cout<<"Sortint del programa..."<<endl; break;
        default: cout<<"Opcio no valida"<<endl; break;
    }
}

void triar_opcio_menu(){
    int opcio = 0;
    do{
        mostrar_menu();
        cout<<"Tria una opcio del menu: ";
        cin>>opcio;
        procesar_opcio_menu(opcio);
    }while(opcio != 4);
}
