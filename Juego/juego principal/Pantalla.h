#ifndef PANTALLA_H
#define PANTALLA_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <map>

class Juego;
class TextoBorde;

// Lo que se muestra en la ventana (menu, escena o nivel); cada pantalla tiene su propia escena de Qt
class Pantalla
{
public:
    Pantalla(Juego* juego);
    ~Pantalla();

    QGraphicsScene* getEscena() const { return escena; }
    void teclaPresionada(int tecla);
    void teclaSoltada(int tecla);
    bool estaTerminada() const { return terminada; }
    int getResultado() const { return resultado; }

protected:
    Juego* juego;
    QGraphicsScene* escena;
    std::map<int, bool> teclas;
    bool terminada;
    int resultado;

    bool estaPresionada(int tecla) const;
    void terminar(int valor);
    QGraphicsTextItem* agregarTexto(const QString& texto, const QFont& fuente, const QColor& color, double x, double y, double ancho = 0);
    TextoBorde* agregarTextoBorde(const QString& texto, const QFont& fuente, const QColor& color, double x, double y, double grosor = 3);
    QGraphicsPixmapItem* agregarMarco(const QString& archivo, double x, double y);
    QGraphicsPixmapItem* agregarFondo(const QString& archivo);
};

#endif
