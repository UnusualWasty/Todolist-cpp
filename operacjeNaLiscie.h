#ifndef OPERACJE_NA_LISCIE_H
#define OPERACJE_NA_LISCIE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "IListaZadan.h"

int pobierzLiczbe(const std::string& wejscie);

class operacjeNaLiscie : public IListaZadan {

    int                          zadania    = 0;
    bool                         czyKategorie = false;
    std::vector<std::string>     kategorie;
    std::vector<std::vector<std::string>> lista;

public:
    const std::vector<std::string>&              pobierzKategorie() const { return kategorie; }
    const std::vector<std::vector<std::string>>& pobierzListe()    const { return lista;      }
    bool czyMaKategorie() const { return czyKategorie; }
    int  liczbaZadan()    const override { return zadania; }

    int  liczbaKategorii()              const { return (int)lista.size(); }
    int  liczbaZadanWKategorii(int kat) const {
        return (kat >= 0 && kat < (int)lista.size()) ? (int)lista[kat].size() : 0;
    }

    std::string pobierzZadanie(int index) const override {
        return (index >= 0 && index < (int)lista.size() && !lista[index].empty())
               ? lista[index][0] : "";
    }
    std::string pobierzZadanieZKategorii(int kat, int idx) const {
        return (kat >= 0 && kat < (int)lista.size() &&
                idx >= 0 && idx < (int)lista[kat].size())
               ? lista[kat][idx] : "";
    }

    void dodajZadanie(const std::string& zadanie) override {
        lista.push_back({zadanie});
        ++zadania;
    }

    void usunZadanie(int index) override {
        if (index >= 0 && index < (int)lista.size()) {
            lista.erase(lista.begin() + index);
            --zadania;
        } else {
            std::cout << "Podany numer zadania nie znajduje się na liście.\n";
        }
    }

    void wyswietlZadania() const override {
        std::cout << "\n==============ZADANIA DO WYKONANIA==============\n";
        bool pusty = std::all_of(lista.begin(), lista.end(),
                                 [](const auto& w){ return w.empty(); });
        if (pusty) {
            std::cout << "Twoja lista zadań jest obecnie pusta.\n";
            return;
        }
        if (czyKategorie) {
            for (int i = 0; i < (int)lista.size(); ++i) {
                std::cout << kategorie[i] << ":\n";
                for (int j = 0; j < (int)lista[i].size(); ++j)
                    std::cout << "  " << j + 1 << ": " << lista[i][j] << "\n";
            }
        } else {
            for (int i = 0; i < (int)lista.size(); ++i)
                std::cout << i + 1 << ": " << lista[i][0] << "\n";
        }
    }

    void tworzenieZadan() {
        if (!lista.empty()) {
            std::cout << "==============UWAGA!==============\n"
                         "Próbujesz utworzyć zadania, a jedna lista już istnieje\n"
                         "Spowoduje to usunięcie starej listy\nKontynuować?(T/N)\n";
            std::string dec; std::getline(std::cin, dec);
            if (dec != "T" && dec != "t") { std::cout << "Program powróci do wyboru akcji\n"; return; }
            zadania = 0; lista.clear(); kategorie.clear();
        }
        try {
            std::cout << "Zdecyduj, czy chcesz:\n"
                         "1. Utworzyć listę bez podziału zadań\n"
                         "2. Utworzyć listę z podziałem na kategorie zadań\n";
            std::string w; std::getline(std::cin, w);
            int typ = pobierzLiczbe(w);

            if (typ == 1) {
                czyKategorie = false;
                std::cout << "\nPodaj liczbę zadań, jakie masz do wykonania: ";
                std::getline(std::cin, w);
                int n = pobierzLiczbe(w);
                for (int i = 0; i < n; ++i) {
                    std::cout << "Podaj " << i + 1 << " zadanie: ";
                    std::getline(std::cin, w);
                    dodajZadanie(w);
                }
                std::cout << "Utworzono listę zadań.\n";

            } else if (typ == 2) {
                czyKategorie = true;
                std::cout << "Podaj ilość kategorii zadań, jakie masz do wykonania\n";
                std::getline(std::cin, w);
                int rozmiar = pobierzLiczbe(w);
                std::vector<int>         katzad(rozmiar);
                std::vector<std::string> kat(rozmiar);

                for (int i = 0; i < rozmiar; ++i) {
                    std::cout << "Podaj nazwę kategorii numer " << i + 1 << "\n";
                    std::getline(std::cin, kat[i]);
                    std::cout << "Podaj liczbę zadań w kategorii numer " << i + 1 << "\n";
                    std::getline(std::cin, w);
                    katzad[i] = pobierzLiczbe(w);
                }
                lista.resize(rozmiar);
                for (int i = 0; i < rozmiar; ++i) {
                    for (int j = 0; j < katzad[i]; ++j) {
                        std::cout << "Podaj zadanie " << j + 1 << " dla kategorii " << kat[i] << "\n";
                        std::getline(std::cin, w);
                        lista[i].push_back(w);
                    }
                    kategorie.push_back(kat[i]);
                    std::cout << "Dodano zadania do kategorii " << kat[i] << "\n";
                }
                std::cout << "Pomyślnie dodano zadania\n";
            }
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << "\nProgram wróci do menu wyboru akcji.\n";
            lista.clear(); kategorie.clear(); zadania = 0; czyKategorie = false;
        }
    }

    void dodawanieZadan() {
        try {
            std::string w;
            if (czyKategorie) {
                std::cout << "Do której kategorii chcesz dodać zadanie?\n";
                for (int i = 0; i < (int)kategorie.size(); ++i)
                    std::cout << i + 1 << ". " << kategorie[i] << "\n";
                std::getline(std::cin, w);
                int kat = pobierzLiczbe(w) - 1;
                if (kat < 0 || kat >= (int)kategorie.size()) { std::cout << "Nieprawidłowa kategoria.\n"; return; }

                std::cout << "Podaj ile zadań chcesz dodać: ";
                std::getline(std::cin, w);
                int n = pobierzLiczbe(w);

                std::cout << "Chcesz dodać element na:\n1. Konkretne miejsce,\n2. Koniec kategorii?\n";
                std::getline(std::cin, w);
                int dec = pobierzLiczbe(w);

                for (int i = 0; i < n; ++i) {
                    std::cout << "Podaj " << i + 1 << " zadanie: ";
                    std::string zad; std::getline(std::cin, zad);
                    if (dec == 1) {
                        std::cout << "Podaj miejsce (1-" << lista[kat].size() + 1 << "): ";
                        std::getline(std::cin, w);
                        int m = pobierzLiczbe(w);
                        if (m < 1 || m > (int)lista[kat].size() + 1) {
                            std::cout << "Nieprawidłowe miejsce, element zostanie dodany na koniec.\n";
                            lista[kat].push_back(zad);
                        } else {
                            lista[kat].insert(lista[kat].begin() + m - 1, zad);
                        }
                    } else {
                        lista[kat].push_back(zad);
                    }
                    ++zadania;
                }
                std::cout << "Pomyślnie dodano elementy do kategorii " << kategorie[kat] << ".\n";

            } else {
                std::cout << "Podaj ile zadań chcesz dodać: \n";
                std::getline(std::cin, w);
                int n = pobierzLiczbe(w);
                if (n == 0) { std::cout << "Podano 0 dodań do listy, program powróci do menu wyboru akcji.\n"; return; }

                std::cout << "Chcesz dodać element na:\n1. Konkretne miejsce listy,\n2. Koniec listy?\n";
                std::getline(std::cin, w);
                int dec = pobierzLiczbe(w);

                if (dec == 1) {
                    for (int i = 1; i <= n; ++i) {
                        std::cout << "Podaj " << i << " zadanie: \n";
                        std::string zad; std::getline(std::cin, zad);
                        std::cout << "Podaj miejsce w liście dla elementu '" << zad << "' (1-" << zadania + 1 << "): \n";
                        std::getline(std::cin, w);
                        int m = pobierzLiczbe(w);
                        if (m < 1 || m > zadania + 1) {
                            std::cout << "Nieprawidłowe miejsce, element zostanie dodany na koniec listy.\n";
                            dodajZadanie(zad);
                        } else {
                            lista.insert(lista.begin() + m - 1, {zad});
                            ++zadania;
                        }
                    }
                } else if (dec == 2) {
                    for (int i = 0; i < n; ++i) {
                        std::cout << "Podaj " << zadania + 1 << " zadanie: \n";
                        std::getline(std::cin, w);
                        dodajZadanie(w);
                    }
                } else {
                    std::cout << "Podano nieprawidłową wartość, spróbuj ponownie.\n";
                    return;
                }
                std::cout << "Pomyślnie dodano elementy do listy.\n";
            }
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << "\nProgram wróci do menu wyboru akcji.\n";
        }
    }

    void usuwanieZadan() {
        try {
            std::string w;
            if (czyKategorie) {
                std::cout << "Z której kategorii chcesz usunąć zadanie?\n";
                for (int i = 0; i < (int)kategorie.size(); ++i)
                    std::cout << i + 1 << ". " << kategorie[i] << "\n";
                std::getline(std::cin, w);
                int kat = pobierzLiczbe(w) - 1;
                if (kat < 0 || kat >= (int)kategorie.size()) { std::cout << "Nieprawidłowa kategoria.\n"; return; }

                std::cout << "Zadania w kategorii " << kategorie[kat] << ":\n";
                for (int i = 0; i < (int)lista[kat].size(); ++i)
                    std::cout << i + 1 << ". " << lista[kat][i] << "\n";

                std::cout << "Podaj numer zadania do usunięcia: ";
                std::getline(std::cin, w);
                int idx = pobierzLiczbe(w) - 1;
                if (idx < 0 || idx >= (int)lista[kat].size()) { std::cout << "Nieprawidłowe zadanie.\n"; return; }

                lista[kat].erase(lista[kat].begin() + idx);
                --zadania;
                std::cout << "Usunięto zadanie.\n";

            } else {
                std::cout << "Chcesz usunąć:\n1. Całą listę,\n2. Konkretny element?\n";
                std::getline(std::cin, w);
                int dec = pobierzLiczbe(w);

                if (dec == 1) {
                    lista.clear(); zadania = 0;
                    std::cout << "Wyczyszczono całą listę zadań.\n";

                } else if (dec == 2) {
                    std::cout << "Czy chcesz usunąć odwołując się do:\n1. Numeru zadania,\n2. Konkretnego zadania?\n";
                    std::getline(std::cin, w);
                    int spos = pobierzLiczbe(w);

                    if (spos == 1) {
                        std::cout << "Podaj indeks zadania do usunięcia: \n";
                        std::getline(std::cin, w);
                        usunZadanie(pobierzLiczbe(w) - 1);

                    } else if (spos == 2) {
                        std::cout << "Podaj zadanie do usunięcia: \n";
                        std::string el; std::getline(std::cin, el);
                        auto it = std::find_if(lista.begin(), lista.end(),
                            [&el](const auto& row){ return !row.empty() && row[0] == el; });
                        if (it != lista.end()) {
                            usunZadanie((int)std::distance(lista.begin(), it));
                            std::cout << "Usunięto zadanie.\n";
                        } else {
                            std::cout << "Nie znaleziono zadania: " << el << "\n";
                        }
                    } else {
                        std::cout << "Podano niewłaściwą akcję.\n";
                    }
                } else {
                    std::cout << "Podano niewłaściwą akcję.\n";
                }
            }
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << "\nProgram wróci do menu wyboru akcji.\n";
        }
    }
};

#endif