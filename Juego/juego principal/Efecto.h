#ifndef EFECTO_H
#define EFECTO_H

#include "Entidad.h"

// Efecto visual corto (golpe o explosion) que crece, se desvanece y desaparece
class Efecto : public Entidad
{
public:
    Efecto(const QString& imagen, QPointF posicion, double tamano, double duracion, double crecimiento);
    void actualizar(double dt);

private:
    double duracion;
    double tiempo;
    double crecimiento;
};

#endif
