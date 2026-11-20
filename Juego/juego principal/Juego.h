#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include "Configuracion.h"
#include "TiposAgente.h"

class Pantalla;
class Menu;
class Escena;
class NivelMedellin;
class NivelCiudadPerdida;
class Sonido;

enum Paso {
    PASO_MENU, PASO_INTRO, PASO_MEDELLIN, PASO_CAPTURA, PASO_VIAJE_SIERRA, PASO_CIUDAD_PERDIDA, PASO_FINAL, PASO_CREDITOS
};

// Ventana del juego: lleva el ciclo de 60 cuadros por segundo y cambia de pantalla
class Juego : public QMainWindow
{
    Q_OBJECT

public:
    Juego();
    ~Juego();

    QGraphicsView* getVista() const { return vista; }
    Sonido* getSonido() const { return sonido; }
    const Configuracion& getConfig() const { return config; }
    void setDificultad(int dificultad) { config = configuracionPara(dificultad); }

    int puntaje;

protected:
    void keyPressEvent(QKeyEvent* evento);
    void keyReleaseEvent(QKeyEvent* evento);

private slots:
    void cicloDelJuego();

private:
    QGraphicsView* vista;
    QTimer* temporizador;
    Sonido* sonido;
    Configuracion config;
    MemoriaRelojero memoria;
    int paso;
    Menu* menu;
    Escena* escena;
    NivelMedellin* medellin;
    NivelCiudadPerdida* ciudad;

    Pantalla* pantallaActual() const;
    void borrarPantalla();
    void irAPaso(int nuevoPaso);
};

#endif
