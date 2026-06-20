#ifndef OPERACJE_NA_LISCIE_H
#define OPERACJE_NA_LISCIE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class operacjeNaLiscie {
public:
    void tworzenieZadan(int& zadania, vector<string>& lista) {
        int dodajZadania = 0;
        if (lista.empty()) {
            cout << "\nPodaj liczbę zadań, jakie masz do wykonania: ";
            cin >> dodajZadania;
            cin.ignore();
            string task;
            for (int i = 0; i < dodajZadania; i++) {
                cout << "Podaj " << i + 1 << " zadanie: ";
                getline(cin, task);
                lista.push_back(task);
                zadania++;
            }
            cout << "Utworzono listę zadań." << endl;
        } else {
            string decyzjaUsun;
            cout << "==============UWAGA!==============\nPróbujesz utworzyć zadania, a jedna lista już istnieje\nSpowoduje to usunięcie starej listy\nKontynuować?(T/N)" << endl;
            getline(cin, decyzjaUsun);
            if (decyzjaUsun == "T" || decyzjaUsun == "t") {
                zadania = 0;
                lista.clear();
                tworzenieZadan(zadania, lista);
                return;
            } else {
                cout << "Program powróci do wyboru akcji" << endl;
            }
        }
    }

    void wyswietlanieZadan(int zadania, const vector<string> lista) {
        if (zadania == 0) {
            cout << "\n==============ZADANIA DO WYKONANIA==============\n";
            cout << "Twoja lista zadań jest obecnie pusta." << endl;
        } else {
            cout << "\n==============ZADANIA DO WYKONANIA==============\n";
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
        } else {
            int decyzjaDodania;
            cout << "Chcesz dodać element na:\n1.Konkretne miejsce listy,\n2.Koniec listy?" << endl;
            cin >> decyzjaDodania;
            cin.ignore();
            if (decyzjaDodania == 1) {
                for (int i = 1; i <= liczbaDodan; i++) {
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
                cout << "Pomyślnie dodano elementy do listy.\n";
            } else if (decyzjaDodania == 2) {
                for (int i = 0; i < liczbaDodan; i++) {
                    string dodane;

                    cout << "Podaj " << zadania + 1 << " zadanie: " << endl;
                    getline(cin, dodane);

                    lista.push_back(dodane);
                    zadania++;
                }
                cout << "Pomyślnie dodano elementy do listy.\n";
            } else {
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

                    if (realIndex >= 0 && realIndex < (int)lista.size()) {
                        lista.erase(lista.begin() + realIndex);
                        zadania--;
                    } else {
                        cout << "Podany numer zadania nie znajduje się na liście, spróbuj ponownie." << endl;
                    }

                } else if (decyzjaUsuwania == 2) {
                    string element;
                    cout << "Podaj zadanie do usunięcia: " << endl;
                    getline(cin, element);
                    auto sprawdz = find(lista.begin(), lista.end(), element);
                    if (sprawdz != lista.end()) {
                        lista.erase(sprawdz);
                        zadania--;
                    } else {
                        cout << "Nie znaleziono zadania: " << element << endl;
                    }
                }
                break;
        }
    }
};

#endif // OPERACJE_NA_LISCIE_H