#include "Poni.h"

Poni::Poni(const QString& nombre, int vida, double rapidez)
    : Personaje(vida, rapidez), nombre(nombre)
{
}
