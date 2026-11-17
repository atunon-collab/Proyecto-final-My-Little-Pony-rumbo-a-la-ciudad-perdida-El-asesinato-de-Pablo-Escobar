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

    void actualizar(double dt);
    void teclaPresionada(int tecla);

private:
    int estado;
    int opcion;
    double tiempo;
    std::vector<TextoBorde*> opciones;
    QGraphicsPixmapItem* placaTitulo;
    TextoBorde* titulo1;
    TextoBorde* titulo2;
    TextoBorde* subtitulo;
    QGraphicsPixmapItem* panel;
    QGraphicsPixmapItem* cursor;
    QGraphicsTextItem* descripcion;
    TextoBorde* advertencia;
    QGraphicsTextItem* textoControles;

    void mostrarPrincipal();
    void mostrarDificultad();
    void mostrarControles();
    void limpiarOpciones();
    void crearOpciones(const std::vector<QString>& textos, double y);
    void resaltar();
};

#endif
