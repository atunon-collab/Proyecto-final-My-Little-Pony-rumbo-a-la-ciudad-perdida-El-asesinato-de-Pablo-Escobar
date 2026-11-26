#ifndef APPLEJACK_H
#define APPLEJACK_H

#include "Poni.h"
#include "MovimientoParabolico.h"
#include "MovimientoPendular.h"
#include <QGraphicsLineItem>

const double ALTURA_MANOS = 125;

// Applejack, la protagonista de la Ciudad Perdida
class Applejack : public Poni
{
public:
    Applejack(double gravedad);
    ~Applejack();

    void actualizar(double dt);
    bool puedeLanzar() const { return esperaLanzar <= 0 && colgadoDe == nullptr; }
    void lanzar() { esperaLanzar = 0.5; }

    void colgarse(MovimientoPendular* pendulo);
    void lanzarLazo(QPointF gancho, bool desdeLaIzquierda);
    void soltarse();
    bool estaColgada() const { return colgadoDe != nullptr; }
    MovimientoPendular* getPendulo() const { return colgadoDe; }
    bool colgadaDelLazo() const { return colgadoDe == lazo; }

    MovimientoParabolico movimiento;
    bool enSuelo;
    double esperaAgarre;

private:
    MovimientoPendular* colgadoDe;
    MovimientoPendular* lazo;
    QGraphicsLineItem* cuerda;
    double esperaLanzar;
};

#endif
