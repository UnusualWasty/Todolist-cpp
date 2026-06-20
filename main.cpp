#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include "operacjeNaPlikach.h"
#include "operacjeNaLiscie.h"

/*dodac polimorfizm, dziedziczenie, kolory interfejsu, podzial zadan na typy (rodzina, praca itp),
prosta obsługa wyjatków, pola danych do klasy, obsluge wyjatkow w tworzeniu zadan i w innych miejsach jako przyklady*/

using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    int globalZadania = 0;
    vector<string> globalLista;
    char akcja;

    operacjeNaPlikach pliki;
    operacjeNaLiscie listy;
    bool dziala = true;

    cout << "Witaj w programie do zarządzania twoimi zadaniami." << endl;

    while (dziala) {
        cout << "\nLista akcji:\nI - import zadań z zew. pliku\nU - utworzenie listy zadań\nP - wyświetlenie listy zadań\nE - edycja listy zadań\nZ - zapisanie zadań do pliku .csv\nW - wyjście z programu" << endl;
        cin >> akcja;
        cin.ignore();

        switch (akcja) {
            case 'I':
            case 'i': {
                pliki.odczyt(globalZadania, globalLista);
                break;
            }
            case 'U':
            case 'u': {
                listy.tworzenieZadan(globalZadania, globalLista);
                break;
            }
            case 'P':
            case 'p': {
                listy.wyswietlanieZadan(globalZadania, globalLista);
                break;
            }
            case 'E':
            case 'e': {
                int akcja2 = 0;
                cout << "Co chcesz zrobić?\n1. Dodać element do listy\n2. Usunąć element z listy" << endl;
                cin >> akcja2;
                cin.ignore();
                if (akcja2 == 1) {
                    listy.dodawanieZadan(globalZadania, globalLista);
                } else if (akcja2 == 2) {
                    listy.usuwanieZadan(globalZadania, globalLista);
                } else {
                    cout << "Podano niewłaściwą wartość, program wróci do menu wyboru akcji." << endl;
                }
                break;
            }
            case 'Z':
            case 'z': {
                pliki.zapis(globalZadania, globalLista);
                break;
            }
            case 'W':
            case 'w': {
                cout << "Wybrano wyjście z programu. Do następnego!" << endl;
                dziala = false;
                break;
            }
            default: {
                cout << "Podano znak, który nie odpowiada żadnej akcji. Proszę spróbować ponownie." << endl;
                break;
            }
        }
    }
    return 0;
}