#ifndef MOVIMIENTOPENDULAR_H
#define MOVIMIENTOPENDULAR_H

#include <QPointF>

// Pendulo simple
class MovimientoPendular
{
public:
    MovimientoPendular(QPointF pivote, double longitud, double amplitudGrados, double gravedad, double faseGrados);

    void actualizar(double dt);
    QPointF posicion() const;
    double getAngulo() const;

    QPointF getPivote() const { return pivote; }
    void setRapidez(double multiplicador) { rapidez = multiplicador; }

private:
    QPointF pivote;
    double longitud;
    double amplitud;
    double omega;
    double fase;
    double rapidez;
};

#endif
