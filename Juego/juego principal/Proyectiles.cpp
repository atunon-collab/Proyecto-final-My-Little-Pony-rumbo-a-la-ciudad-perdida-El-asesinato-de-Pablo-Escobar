#include "Proyectiles.h"
#include "Recursos.h"
#include <QtMath>

Proyectil::Proyectil(const QString& imagen, double tamano, QPointF inicio, QPointF objetivo, double rapidez)
    : tiempoVida(0)
{
    ponerImagen(cargarImagen(":/objetos/" + imagen + ".png", int(tamano), int(tamano)), false);
    definirCaja(tamano * 0.75, tamano * 0.75, false);
    setPos(inicio);
    setZValue(50);
    double dx = objetivo.x() - inicio.x();
    double dy = objetivo.y() - inicio.y();
    double distancia = qSqrt(dx * dx + dy * dy);
    if (distancia < 1) {
        distancia = 1;
    }
    vx = dx / distancia * rapidez;
    vy = dy / distancia * rapidez;
}

void Proyectil::actualizar(double dt)
{
    // x = x + vx*dt, y = y + vy*dt
    setPos(x() + vx * dt, y() + vy * dt);
    setRotation(rotation() + 360 * dt);
    tiempoVida += dt;
    if (tiempoVida > 4) {
        desactivar();
    }
}

Manzana::Manzana(QPointF inicio, double vx, double vy, double gravedad)
    : trayectoria(gravedad), tiempoVida(0)
{
    ponerImagen(cargarImagen(":/objetos/manzana.png", 32, 32), false);
    definirCaja(26, 26, false);
    setZValue(50);
    trayectoria.lanzar(inicio, vx, vy);
    setPos(inicio);
}

void Manzana::actualizar(double dt)
{
    trayectoria.actualizar(dt);
    setPos(trayectoria.posicion());
    tiempoVida += dt;
    if (tiempoVida > 4) {
        desactivar();
    }
}
