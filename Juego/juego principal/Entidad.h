#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QString>
#include <map>
#include <vector>

const double DURACION_CUADRO = 0.05;

struct Animacion
{
    std::vector<QPixmap> derecha;
    std::vector<QPixmap> izquierda;
    bool repetir;
};

// Todo lo que existe dentro de un nivel: se dibuja con una animacion y choca usando una caja invisible
class Entidad : public QGraphicsPixmapItem
{
public:
    Entidad();

    bool estaActiva() const { return activa; }
    void desactivar() { activa = false; }

    bool colisionaCon(const Entidad* otra) const;
    QRectF getCaja() const;
    void definirCaja(double ancho, double alto, bool desdePies);

    void agregarAnimacion(const QString& nombre, const QString& archivo, bool repetir = true, double escala = 1.0);
    void ponerImagen(const QPixmap& imagen, bool desdePies);
    void ponerAnimacion(const QString& nombre);
    void avanzarAnimacion(double dt);

    void mirarHacia(bool derecha);
    bool miraDerecha() const { return haciaDerecha; }

    void setAltura(double z);
    double getAltura() const { return altura; }
    void crearSombra(double ancho);

protected:
    bool activa;
    QGraphicsRectItem* caja;
    std::map<QString, Animacion> animaciones;
    QString animacionActual;
    int cuadro;
    double tiempoCuadro;
    bool haciaDerecha;
    bool desdePies;
    double altura;

    void mostrarCuadro();
};

#endif
