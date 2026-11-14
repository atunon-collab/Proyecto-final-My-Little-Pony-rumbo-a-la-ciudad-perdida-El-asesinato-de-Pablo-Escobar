#include "MovimientoCircular.h"
#include <QtMath>

MovimientoCircular::MovimientoCircular(QPointF centro, double radio, double omega, double anguloGrados)
    : centro(centro), radio(radio), omega(omega), angulo(qDegreesToRadians(anguloGrados))
{
}

void MovimientoCircular::actualizar(double dt)
{
    // angulo = angulo + w*dt
    angulo += omega * dt;
}

QPointF MovimientoCircular::posicion() const
{
    // x = xc + r*cos(angulo), y = yc + r*sen(angulo)
    return QPointF(centro.x() + radio * qCos(angulo), centro.y() + radio * qSin(angulo));
}
