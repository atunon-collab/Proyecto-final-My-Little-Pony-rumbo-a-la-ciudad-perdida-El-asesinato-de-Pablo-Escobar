#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"

// Entidad con vida, velocidad y un momento de invulnerabilidad despues de cada golpe
class Personaje : public Entidad
{
public:
    Personaje(int vidaMaxima, double rapidez);

    void recibirDanio(int cantidad);
    void curar(int cantidad);
    bool estaVivo() const { return vida > 0; }
    int getVida() const { return vida; }
    int getVidaMax() const { return vidaMax; }
    void setVida(int valor) { vida = valor; }

    bool esInvulnerable() const { return invulnerable > 0; }
    void hacerInvulnerable(double segundos) { invulnerable = segundos; }

    double getRapidez() const { return rapidez; }
    QPointF getVelocidad() const { return velocidad; }
    void setVelocidad(QPointF v) { velocidad = v; }

protected:
    int vida;
    int vidaMax;
    double rapidez;
    QPointF velocidad;
    double invulnerable;

    void actualizarParpadeo(double dt);
    void mirarSegunVelocidad();
};

#endif
