#ifndef OPERACJE_NA_PLIKACH_H
#define OPERACJE_NA_PLIKACH_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "IListaZadan.h"

int pobierzLiczbe(const std::string& wejscie);

class operacjeNaPlikach : public IListaZadan {

    int                                   zadania      = 0;
    bool                                  czyKategorie = false;
    std::string                           ostatniPlik;
    std::vector<std::string>              kategorie;
    std::vector<std::vector<std::string>> lista;

public:
    const std::vector<std::string>&              pobierzKategorie() const { return kategorie; }
    const std::vector<std::vector<std::string>>& pobierzListe()    const { return lista;      }
    bool czyMaKategorie() const { return czyKategorie; }

    int  liczbaZadan()    const override { return zadania; }

    std::string pobierzZadanie(int index) const override {
        return (index >= 0 && index < (int)lista.size() && !lista[index].empty())
               ? lista[index][0] : "";
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
        std::cout << "\n==============ZADANIA Z PLIKU"
                  << (ostatniPlik.empty() ? "" : ": " + ostatniPlik)
                  << "==============\n";
        bool pusty = std::all_of(lista.begin(), lista.end(),
                                 [](const auto& w){ return w.empty(); });
        if (pusty) { std::cout << "Brak załadowanych zadań z pliku.\n"; return; }

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

    void odczyt() {
        std::cout << "Podaj nazwę pliku: ";
        std::string plik; std::getline(std::cin, plik);

        std::ifstream f(plik);
        if (!f.is_open()) {
            std::cout << "==============BŁĄD==============\nNie można otworzyć pliku '" << plik << "'.\n";
            return;
        }
        ostatniPlik = plik;
        lista.clear(); kategorie.clear(); zadania = 0; czyKategorie = false;

        std::string naglowek; std::getline(f, naglowek);

        if (naglowek == "id,zadanie") {
            for (std::string linia; std::getline(f, linia); ) {
                auto p = linia.find(',');
                if (p == std::string::npos) continue;
                lista.push_back({linia.substr(p + 1)});
                ++zadania;
            }
        } else if (naglowek == "id kategorii,kategoria,id zadania,zadanie") {
            czyKategorie = true;
            int ostatnieIdKat = -1;
            for (std::string linia; std::getline(f, linia); ) {
                auto p1 = linia.find(',');
                if (p1 == std::string::npos) continue;
                auto p2 = linia.find(',', p1 + 1);
                if (p2 == std::string::npos) continue;
                auto p3 = linia.find(',', p2 + 1);
                if (p3 == std::string::npos) continue;

                int idKat = std::stoi(linia.substr(0, p1));
                if (idKat != ostatnieIdKat) {
                    lista.push_back({});
                    kategorie.push_back(linia.substr(p1 + 1, p2 - p1 - 1));
                    ostatnieIdKat = idKat;
                }
                lista.back().push_back(linia.substr(p3 + 1));
                ++zadania;
            }
        } else {
            std::cout << "==============BŁĄD==============\nNieznany format pliku.\n";
            return;
        }

        const char* odmiana = (zadania == 1) ? "zadanie" :
                              (zadania <= 4)  ? "zadania" : "zadań";
        std::cout << "Zaimportowano " << zadania << " " << odmiana
                  << " z pliku " << plik << ".\n";
    }

    void zapis(bool czyKat,
               const std::vector<std::string>&              kat,
               const std::vector<std::vector<std::string>>& listaKat) const {
        std::string dec;
        std::cout << "\nCzy chcesz aby twoje zadania zostały zapisane w osobnym pliku .csv? (T/N)\n";
        std::getline(std::cin, dec);
        if (dec != "T" && dec != "t") return;

        std::cout << "Czy chcesz nazwać swój plik?(T/N)\n";
        std::getline(std::cin, dec);

        std::string filename;
        if (dec == "N" || dec == "n") {
            time_t ts; time(&ts);
            std::string data = ctime(&ts);
            for (char& c : data)
                if (c == ' ' || c == ':' || c == '\n') c = '_';
            filename = "Todo_list_" + data + ".csv";

        } else if (dec == "T" || dec == "t") {
            std::cout << "UWAGA! PAMIĘTAJ O ODPOWIEDNICH ZNAKACH WSPIERANYCH PRZEZ WINDOWS!\n"
                         "Podaj nazwę pliku: \n";
            std::getline(std::cin, filename);
            if (filename.find_first_of("<>:\"/\\|?*") != std::string::npos) {
                std::cout << "==============BŁĄD==============\n"
                             "Podana nazwa zawiera znaki niedozwolone w systemie Windows.\n";
                return;
            }
            filename += ".csv";
        } else {
            return;
        }

        std::ofstream out(filename);
        if (czyKat) {
            out << "id kategorii,kategoria,id zadania,zadanie\n";
            for (int i = 0; i < (int)listaKat.size(); ++i)
                for (int j = 0; j < (int)listaKat[i].size(); ++j)
                    out << i+1 << "," << kat[i] << "," << j+1 << "," << listaKat[i][j] << "\n";
        } else {
            out << "id,zadanie\n";
            for (int i = 0; i < (int)listaKat.size(); ++i)
                if (!listaKat[i].empty())
                    out << i+1 << "," << listaKat[i][0] << "\n";
        }
        std::cout << "Plik został zapisany pod nazwą '" << filename << "'.\n";
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
                            std::cout << "Nieprawidłowe miejsce, element dodany na koniec.\n";
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
                std::cout << "Podaj ile zadań chcesz dodać: ";
                std::getline(std::cin, w);
                int n = pobierzLiczbe(w);
                if (n == 0) { std::cout << "Podano 0 dodań, program wróci do menu.\n"; return; }

                std::cout << "Chcesz dodać element na:\n1. Konkretne miejsce listy,\n2. Koniec listy?\n";
                std::getline(std::cin, w);
                int dec = pobierzLiczbe(w);

                if (dec == 1) {
                    for (int i = 1; i <= n; ++i) {
                        std::cout << "Podaj " << i << " zadanie: ";
                        std::string zad; std::getline(std::cin, zad);
                        std::cout << "Podaj miejsce (1-" << zadania + 1 << "): ";
                        std::getline(std::cin, w);
                        int m = pobierzLiczbe(w);
                        if (m < 1 || m > zadania + 1) {
                            std::cout << "Nieprawidłowe miejsce, element dodany na koniec.\n";
                            dodajZadanie(zad);
                        } else {
                            lista.insert(lista.begin() + m - 1, {zad});
                            ++zadania;
                        }
                    }
                } else if (dec == 2) {
                    for (int i = 0; i < n; ++i) {
                        std::cout << "Podaj " << zadania + 1 << " zadanie: ";
                        std::getline(std::cin, w);
                        dodajZadanie(w);
                    }
                } else {
                    std::cout << "Podano nieprawidłową wartość.\n";
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
                    std::cout << "Wyczyszczono listę.\n";

                } else if (dec == 2) {
                    std::cout << "Usuń wg:\n1. Numeru,\n2. Nazwy zadania?\n";
                    std::getline(std::cin, w);
                    int spos = pobierzLiczbe(w);

                    if (spos == 1) {
                        std::cout << "Podaj numer zadania: ";
                        std::getline(std::cin, w);
                        usunZadanie(pobierzLiczbe(w) - 1);

                    } else if (spos == 2) {
                        std::cout << "Podaj zadanie do usunięcia: ";
                        std::string el; std::getline(std::cin, el);
                        auto it = std::find_if(lista.begin(), lista.end(),
                            [&el](const auto& row){ return !row.empty() && row[0] == el; });
                        if (it != lista.end()) {
                            usunZadanie((int)std::distance(lista.begin(), it));
                            std::cout << "Usunięto zadanie.\n";
                        } else {
                            std::cout << "Nie znaleziono: " << el << "\n";
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