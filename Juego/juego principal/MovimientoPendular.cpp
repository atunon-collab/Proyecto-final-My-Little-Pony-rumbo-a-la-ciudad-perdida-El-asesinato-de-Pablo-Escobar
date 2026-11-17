#include "MovimientoPendular.h"
#include <QtMath>

MovimientoPendular::MovimientoPendular(QPointF pivote, double longitud, double amplitudGrados, double gravedad, double faseGrados)
    : pivote(pivote), longitud(longitud), amplitud(qDegreesToRadians(amplitudGrados)), fase(qDegreesToRadians(faseGrados)), rapidez(1)
{
    // w = raiz(g/L)
    omega = qSqrt(gravedad / longitud);
}

void MovimientoPendular::actualizar(double dt)
{
    fase += omega * rapidez * dt;
}

double MovimientoPendular::getAngulo() const
{
    // angulo = A*sen(fase)
    return amplitud * qSin(fase);
}

QPointF MovimientoPendular::posicion() const
{
    // x = xp + L*sen(angulo), y = yp + L*cos(angulo)
    double angulo = getAngulo();
    return QPointF(pivote.x() + longitud * qSin(angulo), pivote.y() + longitud * qCos(angulo));
}
