#ifndef ESCENA_H
#define ESCENA_H

#include "Estilo.h"
#include "Pantalla.h"
#include "Entidad.h"
#include <vector>

const int EF_NADA = 0;
const int EF_APARECER = 2;
const int EF_DESAPARECER = 3;
const int EF_ANIMAR = 4;

struct Actor
{
    QString sprite;
    double x;
    double y;
    bool derecha;
    double escala;
    bool visible;
    bool animado;
};

struct LineaDialogo
{
    QString hablante;
    QString texto;
    int efecto;
    int actor;
};

struct DatosEscena
{
    QString fondo;
    QString musica;
    std::vector<Actor> actores;
    std::vector<LineaDialogo> lineas;
};

// Personaje de las escenas
class Figura : public Entidad
{
public:
    Figura(const Actor& actor);
    void actualizar(double dt);
    bool animando;
};

// Escena de dialogo
class Escena : public Pantalla
{
public:
    Escena(Juego* juego, const DatosEscena& datos);

    void actualizar(double dt);
    void teclaPresionada(int tecla);

private:
    DatosEscena datos;
    std::vector<Figura*> figuras;
    int linea;
    TextoBorde* textoNombre;
    QGraphicsTextItem* textoDialogo;

    void empezarLinea();
};

#endif
