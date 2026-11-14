#include "Personaje.h"

Personaje::Personaje(int vidaMaxima, double rapidez)
    : vida(vidaMaxima), vidaMax(vidaMaxima), rapidez(rapidez), velocidad(0, 0), invulnerable(0)
{
}

void Personaje::recibirDanio(int cantidad)
{
    if (invulnerable > 0 || vida <= 0) {
        return;
    }
    vida -= cantidad;
    if (vida < 0) {
        vida = 0;
    }
    invulnerable = 1.3;
}

void Personaje::curar(int cantidad)
{
    vida += cantidad;
    if (vida > vidaMax) {
        vida = vidaMax;
    }
}

void Personaje::actualizarParpadeo(double dt)
{
    if (invulnerable > 0) {
        invulnerable -= dt;
        int paso = int(invulnerable * 12);
        setOpacity(paso % 2 == 0 ? 0.35 : 1.0);
    } else {
        setOpacity(1.0);
    }
}

void Personaje::mirarSegunVelocidad()
{
    if (velocidad.x() > 1) {
        mirarHacia(true);
    } else if (velocidad.x() < -1) {
        mirarHacia(false);
    }
}
