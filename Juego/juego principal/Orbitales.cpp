#include "Orbitales.h"
#include "Recursos.h"
#include <QtMath>

AnomaliaTemporal::AnomaliaTemporal(QPointF centro, double radio, double omega, double anguloGrados, double tamano)
    : espera(0), giro(centro, radio, omega, anguloGrados)
{
    ponerImagen(cargarImagen(":/objetos/orbe.png", int(tamano), int(tamano)), false);
    definirCaja(tamano * 0.7, tamano * 0.7, false);
    setZValue(45);
    setPos(giro.posicion());
}

void AnomaliaTemporal::actualizar(double dt)
{
    if (espera > 0) {
        espera -= dt;
    }
    giro.actualizar(dt);
    setPos(giro.posicion());
}

QPointF AnomaliaTemporal::empujeTangente() const
{
    // tangente al circulo: (-sen, cos)
    double sentido = giro.getOmega() > 0 ? 1 : -1;
    return QPointF(-qSin(giro.getAngulo()), qCos(giro.getAngulo())) * sentido;
}

Portal::Portal(QPointF centro, double radio, int cantidadOrbes)
    : listo(false), pulso(0)
{
    ponerImagen(cargarImagen(":/objetos/portal.png", 150, 150), false);
    definirCaja(90, 90, false);
    setPos(centro);
    setZValue(5);
    QPixmap imagenOrbe = cargarImagen(":/objetos/orbe.png", 30, 30);
    for (int i = 0; i < cantidadOrbes; i++) {
        giros.push_back(MovimientoCircular(QPointF(0, 0), radio, 1.8, 360.0 * i / cantidadOrbes));
        QGraphicsPixmapItem* orbe = new QGraphicsPixmapItem(imagenOrbe, this);
        orbe->setOffset(-15, -15);
        orbes.push_back(orbe);
    }
}

void Portal::actualizar(double dt)
{
    pulso += dt;
    for (int i = 0; i < int(giros.size()); i++) {
        giros[i].actualizar(dt);
        orbes[i]->setPos(giros[i].posicion());
    }
    setOpacity(listo ? 1.0 : 0.75);
    setScale(listo ? 1.0 + 0.08 * qSin(pulso * 6) : 1.0);
}
