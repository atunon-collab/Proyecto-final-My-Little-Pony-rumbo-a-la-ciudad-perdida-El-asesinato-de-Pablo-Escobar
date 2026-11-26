#ifndef PROYECTILES_H
#define PROYECTILES_H

#include "Entidad.h"
#include "MovimientoParabolico.h"

// Ataque que va en linea recta
class Proyectil : public Entidad
{
public:
    Proyectil(const QString& imagen, double tamano, QPointF inicio, QPointF objetivo, double rapidez);
    void actualizar(double dt);

private:
    double vx;
    double vy;
    double tiempoVida;
};

// Manzana que lanza Applejack
class Manzana : public Entidad
{
public:
    Manzana(QPointF inicio, double vx, double vy, double gravedad);
    void actualizar(double dt);

private:
    MovimientoParabolico trayectoria;
    double tiempoVida;
};

#endif
