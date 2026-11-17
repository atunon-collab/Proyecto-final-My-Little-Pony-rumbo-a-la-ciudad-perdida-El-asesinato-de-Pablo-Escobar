#include "Juego.h"
#include "Constantes.h"
#include "Pantalla.h"
#include "Menu.h"
#include "Sonido.h"
#include "Recursos.h"

Juego::Juego()
    : puntaje(0), paso(PASO_MENU), menu(nullptr)
{
    setWindowTitle("Rumbo a la Ciudad Perdida: El Asesinato de Pablo Escobar");
    vista = new QGraphicsView(this);
    vista->setFixedSize(ANCHO_PANTALLA, ALTO_PANTALLA);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setFrameShape(QFrame::NoFrame);
    vista->setFocusPolicy(Qt::NoFocus);
    vista->setBackgroundBrush(Qt::black);
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
    return menu;
}

void Juego::borrarPantalla()
{
    delete menu;
    menu = nullptr;
}

void Juego::irAPaso(int nuevoPaso)
{
    borrarPantalla();
    paso = nuevoPaso;
    puntaje = 0;
    menu = new Menu(this);
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
