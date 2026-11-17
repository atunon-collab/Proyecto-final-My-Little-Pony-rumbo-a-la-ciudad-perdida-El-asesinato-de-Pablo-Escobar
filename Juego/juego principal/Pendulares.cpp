#include "Pendulares.h"
#include "Recursos.h"
#include <QPen>

const double GRAVEDAD_PENDULOS = 1800;

ObjetoPendular::ObjetoPendular(QPointF pivote, double longitud, double amplitudGrados, double faseGrados)
    : pendulo(pivote, longitud, amplitudGrados, GRAVEDAD_PENDULOS, faseGrados)
{
    cuerda = new QGraphicsLineItem(this);
    cuerda->setFlag(QGraphicsItem::ItemStacksBehindParent);
    setPos(pendulo.posicion());
}

void ObjetoPendular::actualizar(double dt)
{
    pendulo.actualizar(dt);
    setPos(pendulo.posicion());
    cuerda->setLine(QLineF(QPointF(0, 0), mapFromScene(pendulo.getPivote())));
}

Liana::Liana(QPointF pivote, double longitud, double amplitudGrados)
    : ObjetoPendular(pivote, longitud, amplitudGrados, 0)
{
    ponerImagen(cargarImagen(":/objetos/liana.png", 36, 60), false);
    definirCaja(40, 40, false);
    cuerda->setPen(QPen(QColor(60, 130, 40), 6));
    setZValue(5);
}

TrampaColgante::TrampaColgante(QPointF pivote, double longitud, double amplitudGrados, const QString& imagen,
                               double ancho, double alto, double velocidad)
    : ObjetoPendular(pivote, longitud, amplitudGrados, 90), velocidad(velocidad), tiempoRapido(0)
{
    ponerImagen(cargarImagen(":/objetos/" + imagen + ".png", int(ancho), int(alto)), false);
    definirCaja(ancho * 0.7, alto * 0.7, false);
    cuerda->setPen(QPen(QColor(70, 60, 50), 4));
    pendulo.setRapidez(velocidad);
    setZValue(30);
}

void TrampaColgante::actualizar(double dt)
{
    if (tiempoRapido > 0) {
        tiempoRapido -= dt;
        if (tiempoRapido <= 0) {
            pendulo.setRapidez(velocidad);
            setOpacity(1.0);
        }
    }
    ObjetoPendular::actualizar(dt);
}

void TrampaColgante::acelerar()
{
    tiempoRapido = 2.5;
    pendulo.setRapidez(velocidad * 1.6);
    setOpacity(0.7);
}
