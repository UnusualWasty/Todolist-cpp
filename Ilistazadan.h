#ifndef ILISTA_ZADAN_H
#define ILISTA_ZADAN_H

#include <string>
#include <vector>

class IListaZadan {
public:
    virtual void        wyswietlZadania()                    const = 0;
    virtual void        dodajZadanie(const std::string& zadanie)   = 0;
    virtual void        usunZadanie(int index)                     = 0;
    virtual int         liczbaZadan()                        const = 0;
    virtual std::string pobierzZadanie(int index)            const = 0;
    virtual            ~IListaZadan()                              = default;
};

#endif