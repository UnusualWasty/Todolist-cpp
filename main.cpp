#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <windows.h>
using namespace std;

class odczyt {
    public:
    int zadania;
    vector<string> lista;
        void odczytFunkcja() {
            string odpowiedz;
            cout << "Czy chciałbyś/chciałabyś zaimportować swoją listę zadań?(T/N)" << endl;
            cin >> odpowiedz;
            cin.ignore();

            if(odpowiedz == "T" || odpowiedz == "t") {
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
                cout << "Zaimportowano " << zadania << " z pliku " << plik << "." << endl;
            }else {
                cout << "\nPodaj liczbę zadań, jakie masz do wykonania: ";
                cin >> zadania;
                cin.ignore();

                string task;
                for (int i = 0; i < zadania; i++) {
                    cout << "Podaj " << i + 1 << " zadanie: ";
                    getline(cin, task);
                    lista.push_back(task);
                }
            }
        }
};
void zapis (int zadania, vector<string> lista) {
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
                zapis(zadania, lista);
            }else {
                string filename = NazwaPliku + ".csv";
                ofstream outfile(filename);
                for (int i = 0; i < zadania; i++) {
                    int a = i+1;
                    outfile << lista[i] << "\n";
                }
                cout << "Plik został zapisany pod nazwą " << filename << "."<< endl;
            }
        }
    }else {

    }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    odczyt objektOdczyt;
    objektOdczyt.odczytFunkcja();

    int zadania;
    cout << "Witaj w programie todolist." << endl;
    odczyt();
    zapis(objektOdczyt.zadania, objektOdczyt.lista);
    cout << "\n==============ZADANIA DO WYKONANIA==============\n" ;
    for (int i = 0; i < zadania; i++) {
        cout << i + 1 << ": " << objektOdczyt.lista[i] << endl;
    }

    return 0;
}