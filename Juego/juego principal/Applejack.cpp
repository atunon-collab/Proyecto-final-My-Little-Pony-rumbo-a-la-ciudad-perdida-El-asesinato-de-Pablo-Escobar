#include "Applejack.h"
#include <QPen>
#include <QtMath>

Applejack::Applejack(double gravedad)
    : Poni("Applejack", 3, 200), movimiento(gravedad), enSuelo(true), plataformaActual(nullptr), esperaAgarre(0),
      colgadoDe(nullptr), lazo(nullptr), esperaLanzar(0)
{
    agregarAnimacion("quieta", "aj_quieta");
    agregarAnimacion("trote", "aj_trote");
    agregarAnimacion("salto", "aj_salto");
    agregarAnimacion("colgada", "aj_colgada");
    definirCaja(44, 76, true);

    cuerda = new QGraphicsLineItem(this);
    cuerda->setPen(QPen(QColor(150, 100, 40), 3));
    cuerda->setFlag(QGraphicsItem::ItemStacksBehindParent);
    cuerda->setVisible(false);
}

Applejack::~Applejack()
{
    delete lazo;
}

void Applejack::actualizar(double dt)
{
    actualizarParpadeo(dt);
    if (esperaAgarre > 0) {
        esperaAgarre -= dt;
    }
    if (esperaLanzar > 0) {
        esperaLanzar -= dt;
    }
    cuerda->setVisible(colgadoDe != nullptr);
    if (colgadoDe != nullptr) {
        cuerda->setLine(QLineF(QPointF(0, -ALTURA_MANOS), mapFromScene(colgadoDe->getPivote())));
        ponerAnimacion("colgada");
    } else if (!enSuelo) {
        ponerAnimacion("salto");
    } else if (velocidad.x() != 0) {
        ponerAnimacion("trote");
    } else {
        ponerAnimacion("quieta");
    }
    mirarSegunVelocidad();
    avanzarAnimacion(dt);
}

void Applejack::colgarse(MovimientoPendular* pendulo)
{
    colgadoDe = pendulo;
    enSuelo = false;
}

void Applejack::lanzarLazo(QPointF gancho, bool desdeLaIzquierda)
{
    delete lazo;
    QPointF manos = pos() + QPointF(0, -ALTURA_MANOS);
    double dx = manos.x() - gancho.x();
    double dy = manos.y() - gancho.y();
    double longitud = qSqrt(dx * dx + dy * dy);
    lazo = new MovimientoPendular(gancho, longitud, 40, 1800, desdeLaIzquierda ? -90 : 90);
    colgarse(lazo);
}

void Applejack::soltarse()
{
    colgadoDe = nullptr;
    esperaAgarre = 0.4;
}
