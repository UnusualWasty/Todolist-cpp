#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <windows.h>
#include <bits/stdc++.h>
using namespace std;

class operacjeNaPlikach {
public:
    void odczyt(int& zadania, vector<string>& lista) {
        string plik;
        cout << "Pamiętaj proszę, iż w celu poprawnego odczytu pliku, musi być on odpowiednio sformatowany. Przykład:\nPrzykładowe zadanie 1\nPrzykładowe zadanie 2\nPrzykładowe zadanie 3" << endl;
        cout << "Podaj nazwę pliku: ";
        getline(cin, plik);
        string linia;
        ifstream otwieranie;
        otwieranie.open(plik);
        if (!otwieranie.is_open()) {
            cout << "==============BŁĄD==============\nNie można otworzyć pliku '" << plik << "'." << endl;
            return;
        }
        while (getline(otwieranie, linia)) {
            lista.push_back(linia);
            zadania++;
        }
        cout << "Zaimportowano " << zadania << " zadania z pliku " << plik << "." << endl;
    }
    void zapis (int zadania, const vector<string> lista) {
        string decyzja;
        cout << "\nCzy chcesz aby twoje zadania zostały zapisane w osobnym pliku .csv? (T/N)" << endl;
        getline(cin, decyzja);
        if (decyzja == "T" || decyzja == "t") {
            string decyzjaNazwa;
            cout << "Czy chcesz nazwać swój plik?(T/N)" << endl;
            getline(cin, decyzjaNazwa);
            if (decyzjaNazwa == "N" || decyzjaNazwa == "n") {
                time_t timestamp;
                time(&timestamp);
                string plikData = ctime(&timestamp);

                for (char& c : plikData) {
                    if (c == ' ' || c == ':' || c == '\n') c = ' ';
                }
                string filename = "Todo list "+plikData+".csv";
                ofstream outfile(filename);

                for (int i = 0; i < zadania; i++) {
                    outfile << lista[i] << "\n";
                }
                cout << "Plik został zapisany pod nazwą '" << filename << "'."<< endl;

            }else if (decyzjaNazwa == "T" || decyzjaNazwa == "t") {
                string NazwaPliku;
                cout << "UWAGA! PAMIĘTAJ O ODPOWIEDNICH ZNAKACH WSPIERANYCH PRZEZ WINDOWS!\nPodaj nazwę pliku: " << endl;
                getline(cin, NazwaPliku);

                string nielegalne = "<>:\"/\\|?*";
                if (NazwaPliku.find_first_of(nielegalne) != string::npos) {
                    cout << "==============BŁĄD==============\nPodana nazwa zawiera znaki niedozwolone w systemie Windows. Spróbuj ponownie."<< endl;
                    return;
                }else {
                    string filename = NazwaPliku + ".csv";
                    ofstream outfile(filename);
                    for (int i = 0; i < zadania; i++) {
                        outfile << lista[i] << "\n";
                    }
                    cout << "Plik został zapisany pod nazwą " << filename << "."<< endl;
                }
            }
        }else {

        }
    }
};

class operacjeNaLiscie {
public:
    void tworzenieZadan(int& zadania, vector<string>& lista){
        int dodajZadania = 0;
        if (empty(lista)) {
            cout << "\nPodaj liczbę zadań, jakie masz do wykonania: ";
            cin >> dodajZadania;
            cin.ignore();
            string task;
            for (int i = 0; i < dodajZadania; i++) {
                cout << "Podaj " << i + 1 << " zadanie: ";
                getline(cin, task);
                lista.push_back(task);
                zadania ++;
            }
        } else {
            string decyzjaUsun;
            cout << "==============UWAGA!==============\nPróbujesz utworzyć zadania, a jedna lista już istnieje\nSpowoduje to usunięcie starej listy\nKontynuować?(T/N)" << endl;
            getline(cin, decyzjaUsun);
            if(decyzjaUsun == "T" || decyzjaUsun == "t") {
                zadania = 0;
                lista.clear();
                tworzenieZadan(zadania, lista);
                return;
            }else{
                cout << "Program powróci do wyboru akcji" << endl;
            }
        }
    }

    void wyswietlanieZadan(int zadania, const vector<string> lista) {
        if (zadania == 0) {
            cout << "\n==============ZADANIA DO WYKONANIA==============\n" ;
            cout << "Twoja lista zadań jest obecnie pusta." << endl;
        }else{
            cout << "\n==============ZADANIA DO WYKONANIA==============\n" ;
            for (int i = 0; i < zadania; i++) {
                cout << i + 1 << ": " << lista[i] << endl;
            }
        }
    }

    void dodawanieZadan(int& zadania, vector<string>& lista) {
        int liczbaDodan = 0;
        cout << "Podaj ile zadań chcesz dodać: " << endl;
        cin >> liczbaDodan;
        cin.ignore();
        if (liczbaDodan == 0) {
            cout << "Podano 0 dodań do listy, program powróci do menu wyboru akcji." << endl;
            return;
        }else{
            int decyzjaDodania;
            cout << "Chcesz dodać element na:\n1.Konkretne miejsce listy,\n2.Koniec listy?" << endl;
            cin >> decyzjaDodania;
            cin.ignore();
            if (decyzjaDodania == 1) {
                for (int i = 1; i < liczbaDodan; i++) {
                    string dodane;
                    int miejsce;

                    cout << "Podaj " << i << " zadanie: " << endl;
                    getline(cin, dodane);

                    cout << "Podaj miejsce w liście dla elementu '" << dodane << "' (1-" << zadania + 1 << "): " << endl;
                    cin >> miejsce;
                    cin.ignore();

                    if (miejsce < 1 || miejsce > zadania + 1) {
                        cout << "Nieprawidłowe miejsce, element zostanie dodany na koniec listy." << endl;
                        lista.push_back(dodane);
                    } else {
                        lista.insert(lista.begin() + miejsce - 1, dodane);
                    }
                    zadania++;
                }
                cout << "Pomyślnie dodano elementy do listy.";
            }else if (decyzjaDodania == 2) {
                for (int i = 0; i < liczbaDodan; i++) {
                    string dodane;

                    cout << "Podaj " << zadania + 1 << " zadanie: " << endl;
                    getline(cin, dodane);

                    lista.push_back(dodane);
                    zadania++;
                }
                cout << "Pomyślnie dodano elementy do listy.\n";
            }else {
                cout << "Podano nieprawidłową wartość, spróbuj ponownie";
                return;
            }
        }
    }

    void usuwanieZadan(int& zadania, vector<string>& lista) {
        int decyzja = 0;
        cout << "Chcesz usunąć:\n1.Całą listę,\n2.Jej konkretny element?" << endl;
        cin >> decyzja;
        cin.ignore();

        switch (decyzja) {
            default:
                cout << "Podano niewłaściwą akcję" << endl;
                break;
            case 1:
                cout << "Wyczyszczono całą listę zadań." << endl;
                lista.clear();
                zadania = 0;
                break;
            case 2:
                int decyzjaUsuwania = 0;
                cout << "Czy chcesz usunąć odwołując się do:\n1. Numeru zadania,\n2. Konkrego zadania?" << endl;
                cin >> decyzjaUsuwania;
                cin.ignore();
                if (decyzjaUsuwania == 1) {
                    int index = 0;
                    cout << "Podaj indeks zadania do usunięcia: " << endl;
                    cin >> index;
                    cin.ignore();
                    int realIndex = index - 1;

                    if (realIndex>=0 && realIndex < lista.size()) {
                        lista.erase(lista.begin() + realIndex);
                        zadania --;
                    }else {
                        cout << "Podany numer zadania nie znajduje się na liście, spróbuj ponownie." << endl;
                    }

                }else if (decyzjaUsuwania == 2) {
                    string element;
                    cout << "Podaj zadanie do usunięcia: " << endl;
                    getline(cin, element);
                    auto sprawdz = find(lista.begin(), lista.end(), element);
                    if (sprawdz != lista.end()) {
                        lista.erase(sprawdz);
                        zadania --;
                    } else {
                        cout << "Nie znaleziono zadania: " << element << endl;
                    }
                }
                break;
        }
    }
};

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
                }else if (akcja2 == 2) {
                    listy.usuwanieZadan(globalZadania, globalLista);
                }else {
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
