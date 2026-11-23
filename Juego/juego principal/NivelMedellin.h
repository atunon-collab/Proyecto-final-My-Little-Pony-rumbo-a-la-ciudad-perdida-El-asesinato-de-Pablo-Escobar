#ifndef NIVELMEDELLIN_H
#define NIVELMEDELLIN_H

#include "Nivel.h"
#include "TiposAgente.h"
#include <vector>

class Twilight;
class Relojero;
class Techo;
class ObjetoFijo;
class Prueba;
class Item;
class Portal;
class AnomaliaTemporal;
class Proyectil;
class TextoBorde;

struct AnomaliaDePrueba
{
    AnomaliaTemporal* anomalia;
    int prueba;
};

// Nivel 1: los tejados de Medellin
class NivelMedellin : public Nivel
{
public:
    NivelMedellin(Juego* juego, MemoriaRelojero* memoria);

    void actualizar(double dt);
    void teclaPresionada(int tecla);

private:
    Twilight* twilight;
    Relojero* relojero;
    Portal* portal;
    std::vector<Techo*> techos;
    std::vector<ObjetoFijo*> obstaculos;
    std::vector<Prueba*> pruebas;
    std::vector<Item*> cristales;
    std::vector<Proyectil*> granadas;
    std::vector<AnomaliaDePrueba> anomalias;
    std::vector<QRectF> zonasResbalosas;
    std::vector<bool> recogida;
    std::vector<int> porAnalizar;
    int recogidas;
    int analizadas;
    double tiempoRestante;
    QPointF ultimoSeguro;
    bool entroPortal;
    double granadaPendiente;
    QPointF objetivoPendiente;
    double tiempoFlecha;
    double tiempoAyuda;
    int hudSegundos;
    int hudRecogidas;
    int hudPorAnalizar;

    TextoBorde* textoTiempo;
    TextoBorde* textoPruebas;
    TextoBorde* textoAyuda;
    TextoBorde* textoAnalizar;
    QGraphicsRectItem* barraMagia;
    QGraphicsPixmapItem* flecha;

    void cargar(MemoriaRelojero* memoria);
    void moverTwilight(double dt);
    void teletransportar(QPointF direccion);
    void revisarColisiones(double dt);
    void terminarAnalisis();
    bool sobreTecho(Entidad* entidad) const;
    bool chocaObstaculo(Entidad* entidad) const;
    bool posicionValidaRelojero(QPointF posicion) const;
    void crearAnomalia(int indicePrueba);
    void lanzarGranada(QPointF objetivo);
    void actualizarRelojero(double dt);
    void ejecutarAccion(Accion accion);
    void apuntarFlecha(double dt);
    void actualizarHud();
};

#endif
