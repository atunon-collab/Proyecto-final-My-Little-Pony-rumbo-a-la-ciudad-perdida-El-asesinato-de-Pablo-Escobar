#include "Pantalla.h"
#include "Constantes.h"
#include "Recursos.h"
#include "Estilo.h"
#include <QFont>

Pantalla::Pantalla(Juego* juego)
    : juego(juego), terminada(false), resultado(RES_NINGUNO)
{
    escena = new QGraphicsScene(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA);
    escena->setBackgroundBrush(Qt::black);
    // Casi todo se mueve en cada cuadro: sin indice Qt no tiene que reordenar un arbol en cada movimiento
    escena->setItemIndexMethod(QGraphicsScene::NoIndex);
}

Pantalla::~Pantalla()
{
    delete escena;
}

void Pantalla::teclaPresionada(int tecla)
{
    teclas[tecla] = true;
}

void Pantalla::teclaSoltada(int tecla)
{
    teclas[tecla] = false;
}

bool Pantalla::estaPresionada(int tecla) const
{
    auto encontrada = teclas.find(tecla);
    return encontrada != teclas.end() && encontrada->second;
}

void Pantalla::terminar(int valor)
{
    terminada = true;
    resultado = valor;
}

QGraphicsTextItem* Pantalla::agregarTexto(const QString& texto, const QFont& fuente, const QColor& color, double x, double y, double ancho)
{
    QGraphicsTextItem* item = escena->addText(texto, fuente);
    item->setDefaultTextColor(color);
    item->setPos(x, y);
    if (ancho > 0) {
        item->setTextWidth(ancho);
    }
    item->setZValue(1000);
    return item;
}

TextoBorde* Pantalla::agregarTextoBorde(const QString& texto, const QFont& fuente, const QColor& color, double x, double y, double grosor)
{
    TextoBorde* item = new TextoBorde(texto, fuente, color, grosor);
    escena->addItem(item);
    item->setPos(x, y);
    item->setZValue(1000);
    return item;
}

QGraphicsPixmapItem* Pantalla::agregarMarco(const QString& archivo, double x, double y)
{
    QGraphicsPixmapItem* marco = escena->addPixmap(cargarImagen(":/fondos/" + archivo + ".png"));
    marco->setPos(x, y);
    marco->setZValue(900);
    return marco;
}

QGraphicsPixmapItem* Pantalla::agregarFondo(const QString& archivo)
{
    QGraphicsPixmapItem* fondo = escena->addPixmap(cargarImagen(":/fondos/" + archivo + ".png", ANCHO_PANTALLA, ALTO_PANTALLA));
    fondo->setZValue(-100);
    return fondo;
}
