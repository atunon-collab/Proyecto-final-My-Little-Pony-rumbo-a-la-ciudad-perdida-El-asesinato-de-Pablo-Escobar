#include "Entidad.h"
#include "Recursos.h"
#include <QPen>
#include <QBrush>

Entidad::Entidad()
    : activa(true), actual(nullptr), cuadro(0), tiempoCuadro(0), haciaDerecha(true), desdePies(true), altura(0)
{
    caja = new QGraphicsRectItem(this);
    caja->setPen(Qt::NoPen);
    caja->setBrush(Qt::NoBrush);
    // La caja es invisible: asi Qt no la tiene en cuenta al pintar
    caja->setFlag(QGraphicsItem::ItemHasNoContents);
}

bool Entidad::colisionaCon(const Entidad* otra) const
{
    if (otra == nullptr || !otra->activa || !activa) {
        return false;
    }
    // Sin rotacion las dos cajas son rectangulos rectos y basta con ver si se cruzan
    if (rotation() == 0 && otra->rotation() == 0) {
        return getCaja().intersects(otra->getCaja());
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
    if (nombre == animacionActual) {
        return;
    }
    auto encontrada = animaciones.find(nombre);
    if (encontrada == animaciones.end()) {
        return;
    }
    animacionActual = nombre;
    // Se guarda la animacion para no buscarla por nombre en cada cuadro
    actual = &encontrada->second;
    cuadro = 0;
    tiempoCuadro = 0;
    mostrarCuadro();
}

void Entidad::avanzarAnimacion(double dt)
{
    tiempoCuadro += dt;
    if (tiempoCuadro < DURACION_CUADRO || actual == nullptr) {
        return;
    }
    tiempoCuadro = 0;
    int total = int(actual->derecha.size());
    if (cuadro + 1 < total) {
        cuadro++;
    } else if (actual->repetir) {
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
    if (actual == nullptr) {
        return;
    }
    const QPixmap& imagen = haciaDerecha ? actual->derecha[cuadro] : actual->izquierda[cuadro];
    // setPixmap siempre recalcula y repinta; si es la misma imagen (cuadro repetido o solo cambio la altura) se evita
    if (pixmap().cacheKey() != imagen.cacheKey()) {
        setPixmap(imagen);
    }
    if (desdePies) {
        setOffset(-imagen.width() / 2.0, -imagen.height() - altura);
    } else {
        setOffset(-imagen.width() / 2.0, -imagen.height() / 2.0 - altura);
    }
}
