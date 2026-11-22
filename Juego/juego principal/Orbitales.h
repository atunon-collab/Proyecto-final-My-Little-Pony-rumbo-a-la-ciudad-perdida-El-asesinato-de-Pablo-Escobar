#ifndef ORBITALES_H
#define ORBITALES_H

#include "Entidad.h"
#include "MovimientoCircular.h"
#include <vector>

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
    Portal(QPointF centro, double radio, int cantidadOrbes);

    void actualizar(double dt);
    void setListo(bool valor) { listo = valor; }

private:
    std::vector<MovimientoCircular> giros;
    std::vector<QGraphicsPixmapItem*> orbes;
    bool listo;
    double pulso;
};

#endif
