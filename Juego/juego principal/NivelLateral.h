#ifndef NIVELLATERAL_H
#define NIVELLATERAL_H

#include "Nivel.h"
#include "Objetos.h"
#include "MovimientoParabolico.h"
#include <vector>

// Nivel visto de lado, con gravedad y plataformas
class NivelLateral : public Nivel
{
public:
    NivelLateral(Juego* juego, double anchoMundo, double altoMundo, double gravedad);

protected:
    std::vector<Plataforma*> plataformas;
    std::vector<ObjetoFijo*> solidos;
    double gravedad;

    Plataforma* agregarPlataforma(double x, double y, double ancho, double alto, const QString& textura,
                                  TipoPlataforma tipo = PLAT_FIJA, double fase = 0);
    bool chocaConAlgo(QRectF caja) const;
    bool moverConGravedad(Entidad* entidad, MovimientoParabolico& movimiento, double dt, Plataforma*& debajo);
    bool apoyarOChocar(Entidad* entidad, MovimientoParabolico& movimiento, QRectF obstaculo);
};

#endif
