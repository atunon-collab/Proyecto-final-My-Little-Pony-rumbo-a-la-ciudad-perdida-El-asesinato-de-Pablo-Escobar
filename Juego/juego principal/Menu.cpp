#include "Menu.h"
#include "Juego.h"
#include "Sonido.h"
#include "Constantes.h"
#include "Estilo.h"
#include "Recursos.h"
#include <QApplication>

const int MENU_PRINCIPAL = 0;
const int MENU_DIFICULTAD = 1;
const int MENU_CONTROLES = 2;

static void centrar(TextoBorde* item, double y)
{
    item->setPos(ANCHO_PANTALLA / 2.0 - item->boundingRect().center().x(), y);
}

Menu::Menu(Juego* juego)
    : Pantalla(juego), estado(MENU_PRINCIPAL), opcion(0)
{
    agregarFondo("titulo");

    placaTitulo = escena->addPixmap(cargarImagen(":/objetos/titulo_marco.png", 1000, 240));
    placaTitulo->setPos((ANCHO_PANTALLA - placaTitulo->pixmap().width()) / 2, 20);
    placaTitulo->setZValue(990);
    titulo1 = agregarTextoBorde("RUMBO A LA", fuenteTitulo(34), DORADO, 0, 0, 4);
    titulo2 = agregarTextoBorde("CIUDAD PERDIDA", fuenteTitulo(34), DORADO, 0, 0, 4);
    subtitulo = agregarTextoBorde("El Asesinato de Pablo Escobar", fuenteTexto(30), CREMA, 0, 0, 4);
    centrar(titulo1, 95);
    centrar(titulo2, 145);
    centrar(subtitulo, 262);
    titulo1->moveBy(20, 0);
    titulo2->moveBy(20, 0);

    panel = agregarMarco("ui_menu_chico", 390, 320);
    cursor = escena->addPixmap(cargarImagen(":/objetos/icono_flecha.png", 40, 30));
    cursor->setZValue(1001);
    descripcion = agregarTexto("", fuenteTexto(20), CREMA, 420, 522, 440);
    textoControles = agregarTexto("", fuenteTexto(20), CREMA, 205, 335, 870);
    juego->getSonido()->musica("medellin");
    mostrarPrincipal();
}

void Menu::limpiarOpciones()
{
    for (TextoBorde* item : opciones) {
        delete item;
    }
    opciones.clear();
    descripcion->setPlainText("");
    textoControles->setPlainText("");
}

void Menu::crearOpciones(const std::vector<QString>& textos, double y)
{
    for (int i = 0; i < int(textos.size()); i++) {
        TextoBorde* item = agregarTextoBorde(textos[i], fuenteTitulo(24), Qt::white, 0, 0, 3);
        centrar(item, y + i * 54);
        opciones.push_back(item);
    }
}

void Menu::mostrarPrincipal()
{
    limpiarOpciones();
    estado = MENU_PRINCIPAL;
    opcion = 0;
    panel->setPixmap(cargarImagen(":/fondos/ui_menu_chico.png"));
    panel->setPos(390, 330);
    subtitulo->setVisible(true);
    crearOpciones({ "Nueva partida", "Controles", "Salir" }, 375);
    cursor->setVisible(true);
    resaltar();
}

void Menu::mostrarDificultad()
{
    limpiarOpciones();
    estado = MENU_DIFICULTAD;
    opcion = 1;
    panel->setPixmap(cargarImagen(":/fondos/ui_menu.png"));
    panel->setPos(390, 310);
    subtitulo->setVisible(true);
    crearOpciones({ "Facil", "Medio", "Discordia" }, 356);
    cursor->setVisible(true);
    resaltar();
}

void Menu::mostrarControles()
{
    limpiarOpciones();
    estado = MENU_CONTROLES;
    panel->setPixmap(cargarImagen(":/fondos/ui_controles.png"));
    panel->setPos(140, 262);
    subtitulo->setVisible(false);
    cursor->setVisible(false);
    textoControles->setPlainText(
        "Twilight (Medellín): WASD moverse, Espacio saltar, E teletransporte, Q analizar la última prueba.\n\n"
        "Applejack (Ciudad Perdida): A y D moverse, Espacio saltar, W agarrarse de la liana, Espacio soltarse, "
        "E lanzar manzanas, R lanzar el lazo.\n\n"
        "Menús: W y S para elegir, Enter para aceptar. Escenas: Enter avanza. Esc pausa o vuelve.\n\n"
        "Esc para volver");
}

void Menu::resaltar()
{
    for (int i = 0; i < int(opciones.size()); i++) {
        opciones[i]->setColor(i == opcion ? DORADO : QColor(Qt::white));
    }
    if (!opciones.empty()) {
        TextoBorde* elegido = opciones[opcion];
        cursor->setPos(elegido->x() - 55, elegido->y() - 2);
    }
    if (estado == MENU_DIFICULTAD) {
        const char* descripciones[] = {
            "Más tiempo, más objetos de ayuda y un Relojero que apunta con menos precisión.",
            "La experiencia pensada para el juego: el Relojero aprende y presiona.",
            "Menos tiempo y menos ayuda. El Relojero aprende rápido y lanza ataques dobles."
        };
        descripcion->setPlainText(descripciones[opcion]);
    }
}

void Menu::teclaPresionada(int tecla)
{
    Pantalla::teclaPresionada(tecla);
    if (estado == MENU_CONTROLES) {
        if (tecla == Qt::Key_Escape) {
            mostrarPrincipal();
        }
        return;
    }
    int cantidad = int(opciones.size());
    if (tecla == Qt::Key_W) {
        opcion = (opcion + cantidad - 1) % cantidad;
        resaltar();
    } else if (tecla == Qt::Key_S) {
        opcion = (opcion + 1) % cantidad;
        resaltar();
    } else if (tecla == Qt::Key_Escape && estado == MENU_DIFICULTAD) {
        mostrarPrincipal();
    } else if (tecla == Qt::Key_Return) {
        if (estado == MENU_PRINCIPAL) {
            if (opcion == 0) {
                mostrarDificultad();
            } else if (opcion == 1) {
                mostrarControles();
            } else {
                QApplication::quit();
            }
        } else {
            juego->setDificultad(opcion);
            terminar(RES_GANO);
        }
    }
}
