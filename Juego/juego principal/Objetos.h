#ifndef OBJETOS_H
#define OBJETOS_H

#include "Entidad.h"

enum TipoPlataforma { PLAT_FIJA, PLAT_DESMORONA, PLAT_ECO };

// Plataforma de la Ciudad Perdida
class Plataforma : public Entidad
{
public:
    Plataforma(double x, double y, double ancho, double alto, const QString& textura, TipoPlataforma tipo, double fase, double velocidad);

    void actualizar(double dt);
    bool esSolida() const { return solida && habilitada; }
    void pisar();
    QRectF getRect() const { return QRectF(x(), y(), ancho, alto); }
    void setHabilitada(bool valor);
    TipoPlataforma getTipo() const { return tipo; }

private:
    QGraphicsRectItem* superficie;
    double ancho;
    double alto;
    TipoPlataforma tipo;
    double tiempo;
    bool solida;
    bool pisada;
    double tiempoCaida;
    double fase;
    double velocidad;
    bool habilitada;
};

// Tejado o puente de Medellin
class Techo : public Entidad
{
public:
    Techo(double x, double y, double ancho, double alto, const QString& imagen);
    QRectF getRect() const { return QRectF(x(), y(), ancho, alto); }

private:
    double ancho;
    double alto;
};

enum TipoItem { ITEM_CRISTAL, ITEM_MANZANA };

// Cristal o manzana para recoger
class Item : public Entidad
{
public:
    Item(TipoItem tipo, QPointF posicion, bool cenital);
};

// Prueba del crimen
class Prueba : public Entidad
{
public:
    Prueba(int indice, const QString& imagen, QPointF posicion);
    int getIndice() const { return indice; }

private:
    int indice;
};

enum TipoObjeto { OBJ_OBSTACULO, OBJ_TOTEM, OBJ_PINCHOS, OBJ_GANCHO, OBJ_MURO, OBJ_DECORADO };

// Objeto quieto del escenario
class ObjetoFijo : public Entidad
{
public:
    ObjetoFijo(TipoObjeto tipo, const QString& imagen, QPointF posicion, double ancho, double alto, bool desdePies = true);
    void actualizar(double dt);
    void encender();
    bool estaEncendido() const { return encendido; }
    bool golpear();

private:
    bool encendido;
    int golpes;
    double temblor;
    QPointF posicionOriginal;
};

#endif
