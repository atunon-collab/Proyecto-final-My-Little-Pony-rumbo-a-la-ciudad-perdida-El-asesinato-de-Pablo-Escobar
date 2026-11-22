#include "Entidad.h"
#include "Recursos.h"
#include <QPen>
#include <QBrush>

Entidad::Entidad()
    : activa(true), cuadro(0), tiempoCuadro(0), haciaDerecha(true), desdePies(true), altura(0)
{
    caja = new QGraphicsRectItem(this);
    caja->setPen(Qt::NoPen);
    caja->setBrush(Qt::NoBrush);
}

bool Entidad::colisionaCon(const Entidad* otra) const
{
    if (otra == nullptr || !otra->activa || !activa) {
        return false;
    }
    return caja->collidesWithItem(otra->caja);
}

QRectF Entidad::getCaja() const
{
    return caja->sceneBoundingRect();
}

void Entidad::definirCaja(double ancho, double alto, bool desdePies)
{
    if (desdePies) {
        caja->setRect(-ancho / 2, -alto, ancho, alto);
    } else {
        caja->setRect(-ancho / 2, -alto / 2, ancho, alto);
    }
}

void Entidad::agregarAnimacion(const QString& nombre, const QString& archivo, bool repetir, double escala)
{
    Animacion animacion;
    animacion.repetir = repetir;
    cargarTira(archivo, escala, animacion.derecha, animacion.izquierda);
    animaciones[nombre] = animacion;
    if (animacionActual.isEmpty()) {
        ponerAnimacion(nombre);
    }
}

void Entidad::ponerImagen(const QPixmap& imagen, bool desdePies)
{
    Animacion animacion;
    animacion.repetir = true;
    animacion.derecha.push_back(imagen);
    animacion.izquierda.push_back(imagen);
    animaciones["fija"] = animacion;
    this->desdePies = desdePies;
    animacionActual = "";
    ponerAnimacion("fija");
}

void Entidad::ponerAnimacion(const QString& nombre)
{
    if (nombre == animacionActual || animaciones.find(nombre) == animaciones.end()) {
        return;
    }
    animacionActual = nombre;
    cuadro = 0;
    tiempoCuadro = 0;
    mostrarCuadro();
}

void Entidad::avanzarAnimacion(double dt)
{
    tiempoCuadro += dt;
    if (tiempoCuadro < DURACION_CUADRO) {
        return;
    }
    tiempoCuadro = 0;
    Animacion& animacion = animaciones[animacionActual];
    int total = int(animacion.derecha.size());
    if (cuadro + 1 < total) {
        cuadro++;
    } else if (animacion.repetir) {
        cuadro = 0;
    }
    mostrarCuadro();
}

void Entidad::mirarHacia(bool derecha)
{
    if (derecha != haciaDerecha) {
        haciaDerecha = derecha;
        mostrarCuadro();
    }
}

void Entidad::setAltura(double z)
{
    altura = z;
    mostrarCuadro();
}

void Entidad::crearSombra(double ancho)
{
    QGraphicsEllipseItem* sombra = new QGraphicsEllipseItem(-ancho / 2, -ancho / 6, ancho, ancho / 3, this);
    sombra->setBrush(QColor(0, 0, 0, 90));
    sombra->setPen(Qt::NoPen);
    sombra->setFlag(QGraphicsItem::ItemStacksBehindParent);
}

void Entidad::mostrarCuadro()
{
    Animacion& animacion = animaciones[animacionActual];
    const QPixmap& imagen = haciaDerecha ? animacion.derecha[cuadro] : animacion.izquierda[cuadro];
    setPixmap(imagen);
    if (desdePies) {
        setOffset(-imagen.width() / 2.0, -imagen.height() - altura);
    } else {
        setOffset(-imagen.width() / 2.0, -imagen.height() / 2.0 - altura);
    }
}
