#ifndef MOVIMIENTOPARABOLICO_H
#define MOVIMIENTOPARABOLICO_H

#include <QPointF>

// Tiro parabolico (y crece hacia abajo)
class MovimientoParabolico
{
public:
    MovimientoParabolico(double gravedad);

    void lanzar(QPointF inicio, double vx, double vy);
    void actualizar(double dt);

    QPointF posicion() const { return QPointF(x, y); }
    void setPosicion(QPointF p) { x = p.x(); y = p.y(); }
    double getVx() const { return vx; }
    double getVy() const { return vy; }
    void setVx(double valor) { vx = valor; }
    void setVy(double valor) { vy = valor; }

private:
    double x;
    double y;
    double vx;
    double vy;
    double g;
};

#endif
