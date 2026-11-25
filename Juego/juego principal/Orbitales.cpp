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

Portal::Portal(QPointF centro)
{
    ponerImagen(cargarImagen(":/objetos/portal.png", 150, 150), false);
    definirCaja(90, 90, false);
    setPos(centro);
    setZValue(5);
    setListo(false);
}
