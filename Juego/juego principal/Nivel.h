#ifndef NIVEL_H
#define NIVEL_H

#include "Pantalla.h"
#include "Configuracion.h"
#include <vector>
#include <algorithm>

class Entidad;
class Efecto;
class TextoBorde;

// Lo que tienen en comun los dos niveles
class Nivel : public Pantalla
{
public:
    Nivel(Juego* juego, double anchoMundo, double altoMundo);

    void teclaPresionada(int tecla);
    bool enCurso() const;

    template <typename T>
    void borrarInactivas(std::vector<T*>& lista)
    {
        // Una sola pasada: se borran las inactivas y las activas se corren hacia adelante
        auto fin = std::remove_if(lista.begin(), lista.end(), [](T* entidad) {
            if (entidad->estaActiva()) {
                return false;
            }
            delete entidad;
            return true;
        });
        lista.erase(fin, lista.end());
    }

protected:
    Configuracion config;
    double anchoMundo;
    double altoMundo;
    QRectF camara;
    double tiempoJuego;
    int golpesAlJugador;
    int golpesAlRelojero;

    bool empezarCuadro(double dt);
    void terminarCuadro(double dt);
    void agregarEntidad(Entidad* entidad);
    void moverCamara(QPointF centro);
    void agregarHud(QGraphicsItem* item, double x, double y);
    TextoBorde* textoHud(const QString& texto, const QFont& fuente, const QColor& color, double x, double y);
    QGraphicsRectItem* barraHud(double x, double y, double ancho, double alto, const QColor& color);
    QGraphicsPixmapItem* iconoHud(const QString& icono, double x, double y, double tamano);
    QGraphicsRectItem* barraConIcono(const QString& icono, double y, const QColor& color);
    TextoBorde* crearPlacaDerecha(const QString& icono);
    void crearEfecto(const QString& imagen, QPointF posicion, double tamano, double duracion, double crecimiento);
    void crearCorazones(int cantidad, bool placaChica);
    void mostrarCorazones(int vida);
    void mostrarMensaje(const QString& texto, double segundos);
    void ganar(int puntos);
    static double azar(double minimo, double maximo);
    void perder(const QString& motivo);

private:
    int estado;
    double tiempoEstado;
    int opcionPausa;
    double tiempoMensaje;
    QGraphicsRectItem* capaHud;
    std::vector<Efecto*> efectos;
    std::vector<QGraphicsPixmapItem*> corazones;
    TextoBorde* textoMensaje;
    QGraphicsPixmapItem* bannerMensaje;
    QGraphicsPixmapItem* panelPausa;
    std::vector<TextoBorde*> lineasPanel;

    void mostrarPanel(const QString& titulo, const QString& linea1, const QString& linea2, const QString& linea3, int resaltada);
    void mostrarPausa();
    void ocultarMensaje();
};

#endif
