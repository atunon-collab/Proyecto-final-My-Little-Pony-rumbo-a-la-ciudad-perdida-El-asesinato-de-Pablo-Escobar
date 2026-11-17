#include "Enemigos.h"

Patrullero::Patrullero(const QString& sprite, double xMinimo, double xMaximo, double y, double rapidez)
    : Personaje(1, rapidez), xMinimo(xMinimo), xMaximo(xMaximo)
{
    agregarAnimacion("trote", sprite + "_trote");
    definirCaja(50, 70, true);
    setPos((xMinimo + xMaximo) / 2, y);
    velocidad = QPointF(rapidez, 0);
    setZValue(25);
}

void Patrullero::actualizar(double dt)
{
    setX(x() + velocidad.x() * dt);
    if (x() > xMaximo) {
        setX(xMaximo);
        velocidad.setX(-rapidez);
    } else if (x() < xMinimo) {
        setX(xMinimo);
        velocidad.setX(rapidez);
    }
    mirarHacia(velocidad.x() > 0);
    avanzarAnimacion(dt);
}

ManeIac::ManeIac(QPointF posicion)
    : Personaje(2, 0), esperaLanzar(2.0)
{
    agregarAnimacion("quieta", "maneiac_quieta");
    agregarAnimacion("vencida", "maneiac_vencida", false);
    definirCaja(90, 150, true);
    setPos(posicion);
    mirarHacia(false);
    setZValue(25);
}

void ManeIac::actualizar(double dt)
{
    actualizarParpadeo(dt);
    if (esperaLanzar > 0) {
        esperaLanzar -= dt;
    }
    avanzarAnimacion(dt);
}

void ManeIac::recibirDanio(int cantidad)
{
    Personaje::recibirDanio(cantidad);
    if (vida <= 0) {
        ponerAnimacion("vencida");
        setOpacity(1.0);
    }
}

bool ManeIac::quiereLanzar()
{
    if (vida > 0 && esperaLanzar <= 0) {
        esperaLanzar = 2.2;
        return true;
    }
    return false;
}
