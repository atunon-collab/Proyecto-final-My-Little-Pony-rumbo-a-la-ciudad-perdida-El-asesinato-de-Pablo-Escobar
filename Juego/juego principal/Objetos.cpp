#include "Objetos.h"
#include "Recursos.h"
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <cmath>

Plataforma::Plataforma(double x, double y, double ancho, double alto, const QString& textura, TipoPlataforma tipo, double fase, double velocidad)
    : ancho(ancho), alto(alto), tipo(tipo), tiempo(0), solida(true), pisada(false), tiempoCaida(0), fase(fase),
      velocidad(velocidad), habilitada(true)
{
    setPos(x, y);
    caja->setRect(0, 0, ancho, alto);
    superficie = new QGraphicsRectItem(0, 0, ancho, alto, this);
    superficie->setPen(Qt::NoPen);
    if (textura.isEmpty()) {
        superficie->setVisible(false);
    } else {
        double borde = textura == "plat_piedra" ? 0.2 : 0.05;
        double total = alto / (1 - borde);
        double sobresale = total - alto;
        QBrush brocha(cargarImagenAlto(":/fondos/" + textura + ".png", int(total + 0.5)));
        brocha.setTransform(QTransform::fromTranslate(0, -sobresale));
        superficie->setRect(0, -sobresale, ancho, total);
        superficie->setBrush(brocha);
    }
    setZValue(10);
}

void Plataforma::setHabilitada(bool valor)
{
    habilitada = valor;
    setVisible(valor);
}

void Plataforma::pisar()
{
    if (tipo == PLAT_DESMORONA && !pisada && solida) {
        pisada = true;
        tiempoCaida = 1.5 / velocidad;
    }
}

void Plataforma::actualizar(double dt)
{
    tiempo += dt;
    if (tipo == PLAT_DESMORONA) {
        if (pisada && solida) {
            tiempoCaida -= dt;
            superficie->setPos(int(tiempoCaida * 40) % 2 == 0 ? 2 : -2, 0);
            if (tiempoCaida <= 0) {
                solida = false;
                setOpacity(0.15);
                tiempoCaida = 4.0;
            }
        } else if (!solida) {
            tiempoCaida -= dt;
            if (tiempoCaida <= 0) {
                solida = true;
                pisada = false;
                setOpacity(1.0);
                superficie->setPos(0, 0);
            }
        }
    } else if (tipo == PLAT_ECO) {
        double ciclo = 4.0 / velocidad;
        // fmod da el resto en un solo paso; el bucle anterior daba mas vueltas cuanto mas se jugaba
        double momento = std::fmod(tiempo + fase, ciclo);
        double visible = 2.5 / velocidad;
        solida = momento < visible;
        if (!solida) {
            setOpacity(0.12);
        } else if (momento > visible - 0.6) {
            setOpacity(int(momento * 12) % 2 == 0 ? 0.4 : 1.0);
        } else {
            setOpacity(1.0);
        }
    }
}

Techo::Techo(double x, double y, double ancho, double alto, const QString& imagen)
    : ancho(ancho), alto(alto)
{
    setPos(x, y);
    caja->setRect(0, 0, ancho, alto);
    if (!imagen.isEmpty()) {
        QPixmap tablas = cargarImagen(":/objetos/" + imagen + ".png").scaled(int(ancho), int(alto), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        setPixmap(tablas);
        QGraphicsRectItem* sombraPuente = new QGraphicsRectItem(8, 10, ancho, alto, this);
        sombraPuente->setBrush(QColor(0, 0, 0, 90));
        sombraPuente->setPen(Qt::NoPen);
        sombraPuente->setFlag(QGraphicsItem::ItemStacksBehindParent);
    }
    setZValue(1);
}

Item::Item(TipoItem tipo, QPointF posicion, bool cenital)
    : tipo(tipo), tiempo(0)
{
    QString archivo = tipo == ITEM_CRISTAL ? "cristal" : "manzana";
    ponerImagen(cargarImagen(":/objetos/" + archivo + ".png", 38, 38), true);
    definirCaja(56, 56, false);
    if (cenital) {
        crearSombra(28);
    }
    setPos(posicion);
    setZValue(15);
}

void Item::actualizar(double dt)
{
    tiempo += dt;
    setAltura(5 + 5 * qSin(tiempo * 4));
}

Prueba::Prueba(int indice, const QString& imagen, QPointF posicion)
    : indice(indice), tiempo(0)
{
    ponerImagen(cargarImagen(":/objetos/" + imagen + ".png", 44, 44), true);
    definirCaja(64, 64, false);
    crearSombra(34);
    setPos(posicion);
    setZValue(15);
}

void Prueba::actualizar(double dt)
{
    tiempo += dt;
    setOpacity(0.75 + 0.25 * qSin(tiempo * 5));
    setAltura(6 + 4 * qSin(tiempo * 3));
}

ObjetoFijo::ObjetoFijo(TipoObjeto tipo, const QString& imagen, QPointF posicion, double ancho, double alto, bool desdePies)
    : tipo(tipo), encendido(false), golpes(0), temblor(0), posicionOriginal(posicion)
{
    ponerImagen(cargarImagen(":/objetos/" + imagen + ".png", int(ancho), int(alto)), desdePies);
    if (tipo == OBJ_OBSTACULO) {
        definirCaja(ancho * 0.8, alto * 0.45, desdePies);
    } else if (tipo == OBJ_PINCHOS) {
        definirCaja(ancho * 0.9, alto * 0.6, desdePies);
    } else {
        definirCaja(ancho * 0.85, alto * 0.9, desdePies);
    }
    setPos(posicion);
    setZValue(tipo == OBJ_DECORADO ? 2 : 12);
    if (tipo == OBJ_TOTEM) {
        setOpacity(0.55);
    }
}

void ObjetoFijo::actualizar(double dt)
{
    if (temblor > 0) {
        temblor -= dt;
        setPos(posicionOriginal + QPointF(int(temblor * 50) % 2 == 0 ? 3 : -3, 0));
        if (temblor <= 0) {
            setPos(posicionOriginal);
        }
    }
}

void ObjetoFijo::encender()
{
    encendido = true;
    setOpacity(1.0);
}

bool ObjetoFijo::golpear()
{
    golpes++;
    temblor = 0.3;
    if (golpes >= 2) {
        desactivar();
        setVisible(false);
        return true;
    }
    return false;
}
