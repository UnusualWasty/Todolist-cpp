#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <windows.h>
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
    }
    void wyswietlanieZadan(int zadania, const vector<string> lista) {
        if (zadania == 0) {
            cout << "\n==============ZADANIA DO WYKONANIA==============\n" ;
            cout << "Twoja lista zadań jest obecnie pusta." << endl;
        }else{
            for (int i = 0; i < zadania; i++) {
                cout << i + 1 << ": " << lista[i] << endl;
            }
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
                //to be done
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