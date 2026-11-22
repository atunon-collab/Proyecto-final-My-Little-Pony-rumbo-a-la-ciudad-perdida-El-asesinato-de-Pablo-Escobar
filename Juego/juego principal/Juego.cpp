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
    : puntaje(0), paso(PASO_MENU), menu(nullptr), escena(nullptr), medellin(nullptr), ciudad(nullptr)
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

    for (int s = 0; s < NUM_SITUACIONES; s++) {
        for (int a = 0; a < NUM_ACCIONES; a++) {
            memoria.tablaQ[s][a] = 0;
        }
    }
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
    borrarPantalla();
    delete sonido;
    liberarImagenes();
}

Pantalla* Juego::pantallaActual() const
{
    if (menu != nullptr) {
        return menu;
    }
    if (escena != nullptr) {
        return escena;
    }
    if (medellin != nullptr) {
        return medellin;
    }
    return ciudad;
}

void Juego::borrarPantalla()
{
    delete menu;
    delete escena;
    delete medellin;
    delete ciudad;
    menu = nullptr;
    escena = nullptr;
    medellin = nullptr;
    ciudad = nullptr;
}

void Juego::irAPaso(int nuevoPaso)
{
    borrarPantalla();
    paso = nuevoPaso;
    switch (paso) {
    case PASO_MENU:
        puntaje = 0;
        menu = new Menu(this);
        break;
    case PASO_INTRO:
        escena = new Escena(this, escenaIntro());
        break;
    case PASO_MEDELLIN:
        medellin = new NivelMedellin(this, &memoria);
        break;
    case PASO_CAPTURA:
        escena = new Escena(this, escenaCaptura());
        break;
    case PASO_VIAJE_SIERRA:
        escena = new Escena(this, escenaViajeSierra());
        break;
    case PASO_CIUDAD_PERDIDA:
        ciudad = new NivelCiudadPerdida(this, &memoria);
        break;
    case PASO_FINAL:
        escena = new Escena(this, escenaFinal());
        break;
    default:
        escena = new Escena(this, escenaCreditos(puntaje));
        break;
    }
    vista->setScene(pantallaActual()->getEscena());
    vista->setSceneRect(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA);
}

void Juego::keyPressEvent(QKeyEvent* evento)
{
    if (evento->isAutoRepeat()) {
        return;
    }
    int tecla = evento->key();
    if (menu != nullptr) {
        menu->teclaPresionada(tecla);
    } else if (escena != nullptr) {
        escena->teclaPresionada(tecla);
    } else if (medellin != nullptr) {
        medellin->teclaPresionada(tecla);
    } else if (ciudad != nullptr) {
        ciudad->teclaPresionada(tecla);
    }
}

void Juego::keyReleaseEvent(QKeyEvent* evento)
{
    if (!evento->isAutoRepeat()) {
        pantallaActual()->teclaSoltada(evento->key());
    }
}

void Juego::cicloDelJuego()
{
    if (menu != nullptr) {
        menu->actualizar(DT);
    } else if (escena != nullptr) {
        escena->actualizar(DT);
    } else if (medellin != nullptr) {
        medellin->actualizar(DT);
    } else if (ciudad != nullptr) {
        ciudad->actualizar(DT);
    }

    Pantalla* actual = pantallaActual();
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
