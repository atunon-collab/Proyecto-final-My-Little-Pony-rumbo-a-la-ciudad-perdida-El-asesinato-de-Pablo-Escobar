#include "NivelLateral.h"

const double VELOCIDAD_MAXIMA_CAIDA = 1100;

NivelLateral::NivelLateral(Juego* juego, double anchoMundo, double altoMundo, double gravedad)
    : Nivel(juego, anchoMundo, altoMundo), gravedad(gravedad)
{
}

Plataforma* NivelLateral::agregarPlataforma(double x, double y, double ancho, double alto, const QString& textura,
                                            TipoPlataforma tipo, double fase)
{
    Plataforma* plataforma = new Plataforma(x, y, ancho, alto, textura, tipo, fase, config.velocidadPlataformas);
    agregarEntidad(plataforma);
    plataformas.push_back(plataforma);
    return plataforma;
}

bool NivelLateral::chocaConAlgo(QRectF caja) const
{
    if (caja.left() < 0 || caja.right() > anchoMundo) {
        return true;
    }
    for (Plataforma* p : plataformas) {
        if (p->esSolida() && caja.intersects(p->getRect())) {
            return true;
        }
    }
    for (ObjetoFijo* solido : solidos) {
        if (solido->estaActiva() && caja.intersects(solido->getCaja())) {
            return true;
        }
    }
    return false;
}

bool NivelLateral::moverConGravedad(Entidad* entidad, MovimientoParabolico& movimiento, double dt, Plataforma*& debajo)
{
    QPointF antes = entidad->pos();
    movimiento.setPosicion(antes);
    movimiento.actualizar(dt);
    if (movimiento.getVy() > VELOCIDAD_MAXIMA_CAIDA) {
        movimiento.setVy(VELOCIDAD_MAXIMA_CAIDA);
    }
    QPointF nueva = movimiento.posicion();

    entidad->setX(nueva.x());
    if (chocaConAlgo(entidad->getCaja().adjusted(0, 0, 0, -1))) {
        entidad->setX(antes.x());
    }

    bool enSuelo = false;
    debajo = nullptr;
    entidad->setY(nueva.y());
    // La caja se calcula una vez y solo se recalcula cuando un choque mueve a la entidad
    QRectF caja = entidad->getCaja();
    for (Plataforma* p : plataformas) {
        if (p->esSolida() && apoyarOChocar(entidad, movimiento, p->getRect(), caja)) {
            enSuelo = true;
            debajo = p;
        }
    }
    for (ObjetoFijo* solido : solidos) {
        if (solido->estaActiva() && apoyarOChocar(entidad, movimiento, solido->getCaja(), caja)) {
            enSuelo = true;
        }
    }
    movimiento.setPosicion(entidad->pos());
    return enSuelo;
}

bool NivelLateral::apoyarOChocar(Entidad* entidad, MovimientoParabolico& movimiento, QRectF obstaculo, QRectF& caja)
{
    if (!caja.intersects(obstaculo)) {
        return false;
    }
    bool bajando = movimiento.getVy() >= 0;
    if (bajando) {
        entidad->setY(obstaculo.top());
    } else {
        entidad->setY(entidad->y() + obstaculo.bottom() - caja.top());
    }
    movimiento.setVy(0);
    caja = entidad->getCaja();
    return bajando;
}
