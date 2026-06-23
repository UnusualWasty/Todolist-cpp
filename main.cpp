#include <iostream>
#include <string>
#include <stdexcept>
#include <windows.h>
#include "IListaZadan.h"
#include "operacjeNaPlikach.h"
#include "operacjeNaLiscie.h"

using namespace std;

static const string ZIELONY   = "\033[32m";
static const string FIOLETOWY = "\033[35m";
static const string RESET     = "\033[0m";

static void grajBGM() {
    PlaySound("bgm.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

static void zatrzymajBGM() {
    PlaySound(nullptr, nullptr, 0);
}

int pobierzLiczbe(const string& wejscie) {
    if (wejscie.empty())
        throw invalid_argument("BLAD: PODANA WARTOSC NIE JEST CYFRA!");
    for (int i = 0; i < (int)wejscie.size(); ++i)
        if (!isdigit((unsigned char)wejscie[i]) && !(i == 0 && wejscie[i] == '-'))
            throw invalid_argument("BLAD: PODANA WARTOSC NIE JEST CYFRA!");
    try { return stoi(wejscie); }
    catch (...) { throw invalid_argument("BLAD: PODANA WARTOSC NIE JEST CYFRA!"); }
}

static void wyswietlMenu() {
    cout << "\n" << ZIELONY << "Lista akcji:" << RESET << "\n"
         << FIOLETOWY << "I" << RESET << ZIELONY << " - import zadan z zew. pliku"    << RESET << "\n"
         << FIOLETOWY << "U" << RESET << ZIELONY << " - utworzenie listy zadan"        << RESET << "\n"
         << FIOLETOWY << "P" << RESET << ZIELONY << " - wyswietlenie listy zadan"      << RESET << "\n"
         << FIOLETOWY << "E" << RESET << ZIELONY << " - edycja listy zadan"            << RESET << "\n"
         << FIOLETOWY << "Z" << RESET << ZIELONY << " - zapisanie zadan do pliku .csv" << RESET << "\n"
         << FIOLETOWY << "W" << RESET << ZIELONY << " - wyjscie z programu"            << RESET << "\n";
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    grajBGM();

    operacjeNaPlikach pliki;
    operacjeNaLiscie  listy;
    IListaZadan*      aktywna = &listy;
    char              akcja;

    cout << ZIELONY << "Witaj w programie do zarzadzania twoimi zadaniami." << RESET << "\n";

    for (bool dziala = true; dziala; ) {
        wyswietlMenu();
        cin >> akcja;
        cin.ignore();

        switch (akcja) {

            case 'I': case 'i':
                pliki.odczyt();
                aktywna = &pliki;
                break;

            case 'U': case 'u':
                if (aktywna == &pliki) {
                    string dec;
                    cout << "==============UWAGA!==============\n"
                            "Masz zaimportowana liste z pliku.\n"
                            "Utworzenie nowej listy spowoduje jej utrate.\n"
                            "Kontynuowac?(T/N)\n";
                    getline(cin, dec);
                    if (dec != "T" && dec != "t") { cout << "Program powroci do wyboru akcji.\n"; break; }
                }
                listy.tworzenieZadan();
                aktywna = &listy;
                break;

            case 'P': case 'p':
                aktywna->wyswietlZadania();
                break;

            case 'E': case 'e':
                try {
                    cout << "Co chcesz zrobic?\n1. Dodac element do listy\n2. Usunac element z listy\n";
                    string w; getline(cin, w);
                    int sub = pobierzLiczbe(w);
                    if (sub == 1) {
                        if (aktywna == &listy) listy.dodawanieZadan();
                        else pliki.dodawanieZadan();
                    } else if (sub == 2) {
                        if (aktywna == &listy) listy.usuwanieZadan();
                        else pliki.usuwanieZadan();
                    } else {
                        cout << "Podano niewlasciwa wartosc, program wroci do menu wyboru akcji.\n";
                    }
                } catch (const invalid_argument& e) {
                    cout << e.what() << "\nProgram wroci do menu wyboru akcji.\n";
                }
                break;

            case 'Z': case 'z':
                if (aktywna == &listy)
                    pliki.zapis(listy.czyMaKategorie(), listy.pobierzKategorie(), listy.pobierzListe());
                else
                    pliki.zapis(pliki.czyMaKategorie(), pliki.pobierzKategorie(), pliki.pobierzListe());
                break;

            case 'W': case 'w':
                cout << "Wybrano wyjscie z programu. Do nastepnego!\n";
                dziala = false;
                break;

            default:
                cout << "Podano znak, ktory nie odpowiada zadnej akcji. Prosze sprobowac ponownie.\n";
                break;
        }
    }

    zatrzymajBGM();
    return 0;
}