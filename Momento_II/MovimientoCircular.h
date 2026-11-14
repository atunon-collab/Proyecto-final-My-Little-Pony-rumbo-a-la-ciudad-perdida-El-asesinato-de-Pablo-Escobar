#ifndef MOVIMIENTOCIRCULAR_H
#define MOVIMIENTOCIRCULAR_H

#include <QPointF>

// Movimiento circular con radio fijo
class MovimientoCircular
{
public:
    MovimientoCircular(QPointF centro, double radio, double omega, double anguloGrados);

    void actualizar(double dt);
    QPointF posicion() const;

    void setCentro(QPointF c) { centro = c; }
    double getAngulo() const { return angulo; }
    double getOmega() const { return omega; }

private:
    QPointF centro;
    double radio;
    double omega;
    double angulo;
};

#endif
