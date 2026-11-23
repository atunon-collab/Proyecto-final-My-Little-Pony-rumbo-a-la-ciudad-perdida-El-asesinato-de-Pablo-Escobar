#ifndef NIVELCIUDADPERDIDA_H
#define NIVELCIUDADPERDIDA_H

#include "NivelLateral.h"
#include "TiposAgente.h"
#include <vector>

class Applejack;
class Relojero;
class Proyectil;
class Manzana;
class Liana;
class TrampaColgante;
class Patrullero;
class ManeIac;
class Item;
class TextoBorde;

// Nivel 2: la Ciudad Perdida
class NivelCiudadPerdida : public NivelLateral
{
public:
    NivelCiudadPerdida(Juego* juego, MemoriaRelojero* memoria);

    void actualizar(double dt);
    void teclaPresionada(int tecla);

private:
    Applejack* aj;
    Relojero* relojero;
    ManeIac* maneiac;
    Plataforma* escalera;
    QGraphicsPixmapItem* fondo;
    std::vector<ObjetoFijo*> muros;
    std::vector<ObjetoFijo*> pinchos;
    std::vector<ObjetoFijo*> ganchos;
    std::vector<ObjetoFijo*> totems;
    std::vector<Item*> manzanas;
    std::vector<Liana*> lianas;
    std::vector<TrampaColgante*> trampas;
    std::vector<Patrullero*> secuaces;
    std::vector<Proyectil*> rocas;
    std::vector<Proyectil*> bolas;
    std::vector<Manzana*> tiros;
    std::vector<QPointF> perchas;
    int perchaActual;
    bool faseJefe;
    QPointF puntoControl;
    QPointF ultimoSeguro;
    double alturaMaxima;
    int valorHud;
    TextoBorde* textoAltura;

    void cargar(MemoriaRelojero* memoria);
    void moverApplejack(double dt);
    void lanzarManzana();
    void actualizarTiros(double dt);
    void actualizarAtaques(double dt);
    void revisarColisiones();
    void actualizarRelojero(double dt);
    void ejecutarAccion(Accion accion);
    QRectF zonaRelojero() const;
    void empezarJefe();
    void danarApplejack(bool causadoPorRelojero);
    void reaparecer(QPointF lugar);
    void actualizarHud();
};

#endif
