#ifndef PENDULARES_H
#define PENDULARES_H

#include "Entidad.h"
#include "MovimientoPendular.h"
#include <QGraphicsLineItem>

// Algo que cuelga y se balancea
class ObjetoPendular : public Entidad
{
public:
    ObjetoPendular(QPointF pivote, double longitud, double amplitudGrados, double faseGrados);

    void actualizar(double dt);
    MovimientoPendular* getPendulo() { return &pendulo; }

protected:
    MovimientoPendular pendulo;
    QGraphicsLineItem* cuerda;
};

// Liana para colgarse
class Liana : public ObjetoPendular
{
public:
    Liana(QPointF pivote, double longitud, double amplitudGrados);
};

// Tronco o hacha que hace dano
class TrampaColgante : public ObjetoPendular
{
public:
    TrampaColgante(QPointF pivote, double longitud, double amplitudGrados, const QString& imagen, double ancho, double alto, double velocidad);

    void actualizar(double dt);
    void acelerar();

private:
    double velocidad;
    double tiempoRapido;
};

#endif
