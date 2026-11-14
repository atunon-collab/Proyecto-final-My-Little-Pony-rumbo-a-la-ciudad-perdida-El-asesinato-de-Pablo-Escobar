#ifndef PONI_H
#define PONI_H

#include "Personaje.h"

// Personaje que controla el jugador (Twilight o Applejack)
class Poni : public Personaje
{
public:
    Poni(const QString& nombre, int vida, double rapidez);
    QString getNombre() const { return nombre; }

protected:
    QString nombre;
};

#endif
