#ifndef MENU_H
#define MENU_H

#include "Estilo.h"
#include "Pantalla.h"
#include <vector>

// Menu principal, seleccion de dificultad y controles
class Menu : public Pantalla
{
public:
    Menu(Juego* juego);

    void actualizar(double) {}
    void teclaPresionada(int tecla);

private:
    int estado;
    int opcion;
    std::vector<TextoBorde*> opciones;
    QGraphicsPixmapItem* placaTitulo;
    TextoBorde* titulo1;
    TextoBorde* titulo2;
    TextoBorde* subtitulo;
    QGraphicsPixmapItem* panel;
    QGraphicsPixmapItem* cursor;
    QGraphicsTextItem* descripcion;
    QGraphicsTextItem* textoControles;

    void mostrarPrincipal();
    void mostrarDificultad();
    void mostrarControles();
    void limpiarOpciones();
    void crearOpciones(const std::vector<QString>& textos, double y, double paso = 54);
    void resaltar();
};

#endif
