#ifndef OPERACJE_NA_PLIKACH_H
#define OPERACJE_NA_PLIKACH_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>

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
        if (zadania == 1) {
            cout << "Zaimportowano " << zadania << " zadanie z pliku " << plik << "." << endl;
        } else if (zadania == 2 || zadania == 3 || zadania == 4) {
            cout << "Zaimportowano " << zadania << " zadania z pliku " << plik << "." << endl;
        } else {
            cout << "Zaimportowano " << zadania << " zadań z pliku " << plik << "." << endl;
        }
    }

    void zapis(int zadania, const vector<string> lista) {
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
                    if (c == ' ' || c == ':' || c == '\n') c = '_';
                }
                string filename = "Todo_list_" + plikData + ".csv";
                ofstream outfile(filename);

                for (int i = 0; i < zadania; i++) {
                    outfile << lista[i] << "\n";
                }
                cout << "Plik został zapisany pod nazwą '" << filename << "'." << endl;

            } else if (decyzjaNazwa == "T" || decyzjaNazwa == "t") {
                string NazwaPliku;
                cout << "UWAGA! PAMIĘTAJ O ODPOWIEDNICH ZNAKACH WSPIERANYCH PRZEZ WINDOWS!\nPodaj nazwę pliku: " << endl;
                getline(cin, NazwaPliku);

                string nielegalne = "<>:\"/\\|?*";
                if (NazwaPliku.find_first_of(nielegalne) != string::npos) {
                    cout << "==============BŁĄD==============\nPodana nazwa zawiera znaki niedozwolone w systemie Windows. Spróbuj ponownie." << endl;
                    return;
                } else {
                    string filename = NazwaPliku + ".csv";
                    ofstream outfile(filename);
                    for (int i = 0; i < zadania; i++) {
                        outfile << lista[i] << "\n";
                    }
                    cout << "Plik został zapisany pod nazwą " << filename << "." << endl;
                }
            }
        }
    }
};

#endif // OPERACJE_NA_PLIKACH_H