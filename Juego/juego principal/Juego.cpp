#include "Juego.h"
#include "Constantes.h"
#include "Pantalla.h"
#include "Menu.h"
#include "Escena.h"
#include "Textos.h"
#include "NivelMedellin.h"
#include "NivelCiudadPerdida.h"
#include "Sonido.h"
#include "Recursos.h"

Juego::Juego()
    : puntaje(0), memoria(), paso(PASO_MENU), actual(nullptr)
{
    setWindowTitle("Rumbo a la Ciudad Perdida: El Asesinato de Pablo Escobar");
    vista = new QGraphicsView(this);
    vista->setFixedSize(ANCHO_PANTALLA, ALTO_PANTALLA);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFrameShape(QFrame::NoFrame);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->setBackgroundBrush(Qt::black);
    // En los niveles cambia toda la pantalla en cada cuadro, asi que se repinta entera sin calcular regiones
    vista->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    vista->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | QGraphicsView::DontSavePainterState);
    setCentralWidget(vista);
    setFixedSize(ANCHO_PANTALLA, ALTO_PANTALLA);

    // memoria() deja la tabla Q en ceros
    memoria.epsilon = 0.3;

    sonido = new Sonido();
    setDificultad(MEDIO);
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &Juego::cicloDelJuego);
    temporizador->start(16);
    irAPaso(PASO_MENU);
}

Juego::~Juego()
{
    delete actual;
    delete sonido;
    liberarImagenes();
}

void Juego::irAPaso(int nuevoPaso)
{
    delete actual;
    paso = nuevoPaso;
    switch (paso) {
    case PASO_MENU:
        puntaje = 0;
        actual = new Menu(this);
        break;
    case PASO_INTRO:
        actual = new Escena(this, escenaIntro());
        break;
    case PASO_MEDELLIN:
        actual = new NivelMedellin(this, &memoria);
        break;
    case PASO_CAPTURA:
        actual = new Escena(this, escenaCaptura());
        break;
    case PASO_VIAJE_SIERRA:
        actual = new Escena(this, escenaViajeSierra());
        break;
    case PASO_CIUDAD_PERDIDA:
        actual = new NivelCiudadPerdida(this, &memoria);
        break;
    case PASO_FINAL:
        actual = new Escena(this, escenaFinal());
        break;
    default:
        actual = new Escena(this, escenaCreditos(puntaje));
        break;
    }
    vista->setScene(actual->getEscena());
    vista->setSceneRect(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA);
}

void Juego::keyPressEvent(QKeyEvent* evento)
{
    if (!evento->isAutoRepeat()) {
        actual->teclaPresionada(evento->key());
    }
}

void Juego::keyReleaseEvent(QKeyEvent* evento)
{
    if (!evento->isAutoRepeat()) {
        actual->teclaSoltada(evento->key());
    }
}

void Juego::cicloDelJuego()
{
    actual->actualizar(DT);
    if (!actual->estaTerminada()) {
        return;
    }
    int resultado = actual->getResultado();
    if (resultado == RES_MENU || paso == PASO_CREDITOS) {
        irAPaso(PASO_MENU);
    } else if (resultado == RES_REINTENTAR) {
        irAPaso(paso);
    } else {
        irAPaso(paso + 1);
    }
}
