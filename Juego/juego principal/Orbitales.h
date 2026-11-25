#ifndef ORBITALES_H
#define ORBITALES_H

#include "Entidad.h"
#include "MovimientoCircular.h"

// Orbe que gira alrededor de una prueba
class AnomaliaTemporal : public Entidad
{
public:
    AnomaliaTemporal(QPointF centro, double radio, double omega, double anguloGrados, double tamano);

    void actualizar(double dt);
    QPointF empujeTangente() const;
    double espera;

private:
    MovimientoCircular giro;
};

// Portal del Relojero
class Portal : public Entidad
{
public:
    Portal(QPointF centro);
    void setListo(bool listo) { setOpacity(listo ? 1.0 : 0.75); }
};

#endif
