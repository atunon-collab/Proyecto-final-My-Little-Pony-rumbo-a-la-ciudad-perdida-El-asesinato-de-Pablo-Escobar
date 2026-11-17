#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include "Personaje.h"

// Secuaz que camina de lado a lado en una terraza
class Patrullero : public Personaje
{
public:
    Patrullero(const QString& sprite, double xMinimo, double xMaximo, double y, double rapidez);
    void actualizar(double dt);

private:
    double xMinimo;
    double xMaximo;
};

// Mane-iac, la mini jefa
class ManeIac : public Personaje
{
public:
    ManeIac(QPointF posicion);

    void actualizar(double dt);
    void recibirDanio(int cantidad);
    bool quiereLanzar();
    bool estaVencida() const { return vida <= 0; }

private:
    double esperaLanzar;
};

#endif
