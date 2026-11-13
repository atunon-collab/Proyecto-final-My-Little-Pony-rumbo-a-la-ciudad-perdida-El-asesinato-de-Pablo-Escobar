#include "MovimientoParabolico.h"

MovimientoParabolico::MovimientoParabolico(double gravedad)
    : x(0), y(0), vx(0), vy(0), g(gravedad)
{
}

void MovimientoParabolico::lanzar(QPointF inicio, double vx, double vy)
{
    x = inicio.x();
    y = inicio.y();
    this->vx = vx;
    this->vy = vy;
}

void MovimientoParabolico::actualizar(double dt)
{
    // vy = vy + g*dt ; x = x + vx*dt ; y = y + vy*dt
    vy += g * dt;
    x += vx * dt;
    y += vy * dt;
}
