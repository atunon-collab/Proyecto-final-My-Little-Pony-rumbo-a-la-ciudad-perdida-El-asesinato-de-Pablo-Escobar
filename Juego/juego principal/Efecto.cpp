#include "Efecto.h"
#include "Recursos.h"

Efecto::Efecto(const QString& imagen, QPointF posicion, double tamano, double duracion, double crecimiento)
    : duracion(duracion), tiempo(0), crecimiento(crecimiento)
{
    ponerImagen(cargarImagen(":/objetos/" + imagen + ".png", int(tamano), int(tamano)), false);
    setPos(posicion);
    setZValue(70);
}

void Efecto::actualizar(double dt)
{
    tiempo += dt;
    double avance = tiempo / duracion;
    if (avance >= 1) {
        desactivar();
        setVisible(false);
        return;
    }
    setScale(1.0 + (crecimiento - 1.0) * avance);
    setOpacity(1.0 - avance);
}
