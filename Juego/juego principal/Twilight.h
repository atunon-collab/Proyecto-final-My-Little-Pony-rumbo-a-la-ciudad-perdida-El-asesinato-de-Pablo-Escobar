#ifndef TWILIGHT_H
#define TWILIGHT_H

#include "Poni.h"
#include "Configuracion.h"

// Protagonista del nivel de Medellin (vista desde arriba)
class Twilight : public Poni
{
public:
    Twilight(const Configuracion& config);

    void actualizar(double dt);
    void saltar();
    bool estaEnElAire() const { return altura > 0; }
    bool puedeTeletransportarse() const;
    void terminarTeletransporte();
    bool congelar(double segundos);

    double magia;
    double esperaTeleport;
    double analizando;
    double congelada;

private:
    double esperaTeleportMax;
    double efectoTeleport;
    double inmuneCongelar;
    double vz;
};

#endif
