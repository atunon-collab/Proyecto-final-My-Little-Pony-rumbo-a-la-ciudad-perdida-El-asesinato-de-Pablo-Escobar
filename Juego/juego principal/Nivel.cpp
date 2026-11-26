#include "Nivel.h"
#include "Juego.h"
#include "Sonido.h"
#include "Constantes.h"
#include "Entidad.h"
#include "Efecto.h"
#include "Recursos.h"
#include "Estilo.h"
#include <QPen>
#include <QBrush>

const int NIVEL_JUGANDO = 0;
const int NIVEL_PAUSA = 1;
const int NIVEL_GANANDO = 2;
const int NIVEL_PERDIDO = 3;

Nivel::Nivel(Juego* juego, double anchoMundo, double altoMundo)
    : Pantalla(juego), config(juego->getConfig()), anchoMundo(anchoMundo), altoMundo(altoMundo),
      camara(0, 0, ANCHO_PANTALLA, ALTO_PANTALLA), tiempoJuego(0), golpesAlJugador(0), golpesAlRelojero(0),
      estado(NIVEL_JUGANDO), tiempoEstado(0), opcionPausa(0), tiempoMensaje(0)
{
    escena->setSceneRect(0, 0, anchoMundo, altoMundo);

    // Todo el HUD cuelga de esta capa: cada cuadro se mueve solo ella con la camara
    capaHud = new QGraphicsRectItem();
    capaHud->setFlag(QGraphicsItem::ItemHasNoContents);
    capaHud->setZValue(1000);
    escena->addItem(capaHud);

    bannerMensaje = escena->addPixmap(cargarImagen(":/fondos/ui_mensaje.png"));
    agregarHud(bannerMensaje, 90, 128);
    bannerMensaje->setZValue(1300);
    bannerMensaje->setVisible(false);
    textoMensaje = agregarTextoBorde("", fuenteTexto(26), CREMA, 0, 0, 3);
    agregarHud(textoMensaje, 0, 0);
    textoMensaje->setZValue(1301);

    panelPausa = escena->addPixmap(cargarImagen(":/fondos/ui_pausa.png"));
    agregarHud(panelPausa, 340, 195);
    panelPausa->setZValue(1500);
    panelPausa->setVisible(false);
    for (int i = 0; i < 4; i++) {
        TextoBorde* linea = agregarTextoBorde("", fuenteTexto(i == 0 ? 40 : 28), CREMA, 0, 0, 3);
        agregarHud(linea, 0, 0);
        linea->setZValue(1501);
        lineasPanel.push_back(linea);
    }
}

bool Nivel::enCurso() const
{
    return estado == NIVEL_JUGANDO;
}

void Nivel::agregarEntidad(Entidad* entidad)
{
    escena->addItem(entidad);
}

bool Nivel::empezarCuadro(double dt)
{
    if (estado == NIVEL_GANANDO) {
        tiempoEstado -= dt;
        if (tiempoEstado <= 0) {
            terminar(RES_GANO);
        }
        return false;
    }
    if (estado != NIVEL_JUGANDO) {
        return false;
    }
    tiempoJuego += dt;
    return true;
}

void Nivel::terminarCuadro(double dt)
{
    if (estado == NIVEL_JUGANDO) {
        for (Efecto* efecto : efectos) {
            efecto->actualizar(dt);
        }
        borrarInactivas(efectos);
        if (tiempoMensaje > 0) {
            tiempoMensaje -= dt;
            if (tiempoMensaje <= 0) {
                ocultarMensaje();
            }
        }
    }
    capaHud->setPos(camara.topLeft());
}

void Nivel::moverCamara(QPointF centro)
{
    double x = centro.x() - ANCHO_PANTALLA / 2.0;
    double y = centro.y() - ALTO_PANTALLA / 2.0;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > anchoMundo - ANCHO_PANTALLA) x = anchoMundo - ANCHO_PANTALLA;
    if (y > altoMundo - ALTO_PANTALLA) y = altoMundo - ALTO_PANTALLA;
    camara = QRectF(x, y, ANCHO_PANTALLA, ALTO_PANTALLA);
    // Se compara con la vista y no con la camara anterior: Juego reinicia la vista al crear el nivel
    if (juego->getVista()->sceneRect() != camara) {
        juego->getVista()->setSceneRect(camara);
    }
}

void Nivel::agregarHud(QGraphicsItem* item, double x, double y)
{
    // Las posiciones del HUD quedan relativas a la esquina de la pantalla
    item->setParentItem(capaHud);
    item->setPos(x, y);
    if (item->zValue() < 1000) {
        item->setZValue(1000);
    }
}

TextoBorde* Nivel::textoHud(const QString& texto, const QFont& fuente, const QColor& color, double x, double y)
{
    TextoBorde* item = agregarTextoBorde(texto, fuente, color, 0, 0, 3);
    agregarHud(item, x, y);
    item->setZValue(1010);
    return item;
}

QGraphicsRectItem* Nivel::barraHud(double x, double y, double ancho, double alto, const QColor& color)
{
    QGraphicsRectItem* barra = escena->addRect(0, 0, ancho, alto, Qt::NoPen, QBrush(color));
    agregarHud(barra, x, y);
    return barra;
}

QGraphicsPixmapItem* Nivel::iconoHud(const QString& icono, double x, double y, double tamano)
{
    QGraphicsPixmapItem* item = escena->addPixmap(cargarImagen(":/objetos/" + icono + ".png", int(tamano), int(tamano)));
    agregarHud(item, x, y);
    item->setZValue(1010);
    return item;
}

QGraphicsRectItem* Nivel::barraConIcono(const QString& icono, double y, const QColor& color)
{
    iconoHud(icono, 36, y - 6, 24);
    QGraphicsRectItem* fondo = barraHud(66, y, 232, 14, QColor(10, 10, 30));
    fondo->setPen(QPen(DORADO, 2));
    fondo->setZValue(1005);
    QGraphicsRectItem* barra = barraHud(67, y + 1, 230, 12, color);
    barra->setZValue(1006);
    return barra;
}

TextoBorde* Nivel::crearPlacaDerecha(const QString& icono)
{
    QGraphicsPixmapItem* placa = escena->addPixmap(cargarImagen(":/fondos/ui_hud_chico.png"));
    agregarHud(placa, 908, 8);
    iconoHud(icono, 934, 18, 38);
    return textoHud("", fuenteTexto(25), CREMA, 982, 20);
}

void Nivel::crearEfecto(const QString& imagen, QPointF posicion, double tamano, double duracion, double crecimiento)
{
    Efecto* efecto = new Efecto(imagen, posicion, tamano, duracion, crecimiento);
    agregarEntidad(efecto);
    efectos.push_back(efecto);
}

void Nivel::crearCorazones(int cantidad, bool placaChica)
{
    QGraphicsPixmapItem* placa = escena->addPixmap(cargarImagen(placaChica ? ":/fondos/ui_hud_chico.png" : ":/fondos/ui_hud.png"));
    agregarHud(placa, 8, placaChica ? 8 : 6);
    for (int i = 0; i < cantidad; i++) {
        QGraphicsPixmapItem* corazon = escena->addPixmap(cargarImagen(":/objetos/corazon.png", 34, 34));
        agregarHud(corazon, 34 + i * 40, placaChica ? 21 : 18);
        corazon->setZValue(1010);
        corazones.push_back(corazon);
    }
}

void Nivel::mostrarCorazones(int vida)
{
    for (int i = 0; i < int(corazones.size()); i++) {
        corazones[i]->setOpacity(i < vida ? 1.0 : 0.2);
    }
}

void Nivel::mostrarMensaje(const QString& texto, double segundos)
{
    textoMensaje->setTexto(texto);
    QPointF centro = textoMensaje->boundingRect().center();
    textoMensaje->setPos(ANCHO_PANTALLA / 2.0 - centro.x(), 168 - centro.y());
    bool dosLineas = texto.contains("\n");
    bannerMensaje->setPixmap(cargarImagen(dosLineas ? ":/fondos/ui_mensaje_alto.png" : ":/fondos/ui_mensaje.png"));
    bannerMensaje->setOffset(0, dosLineas ? -6 : 0);
    bannerMensaje->setVisible(true);
    tiempoMensaje = segundos;
}

void Nivel::ocultarMensaje()
{
    textoMensaje->setTexto("");
    bannerMensaje->setVisible(false);
    tiempoMensaje = 0;
}

void Nivel::ganar(int puntos)
{
    juego->puntaje += puntos;
    juego->getSonido()->efecto("victoria");
    estado = NIVEL_GANANDO;
    tiempoEstado = 2.0;
    mostrarMensaje("¡Nivel completado!", 5);
}

void Nivel::perder(const QString& motivo)
{
    estado = NIVEL_PERDIDO;
    juego->getSonido()->efecto("golpe");
    // El aviso de arriba tapaba el borde del panel
    ocultarMensaje();
    panelPausa->setVisible(true);
    mostrarPanel(motivo, "El Relojero recuerda lo que aprendió de ti.", "Enter: reintentar", "Esc: menú", -1);
    lineasPanel[0]->setColor(QColor(255, 107, 107));
}

void Nivel::mostrarPanel(const QString& titulo, const QString& linea1, const QString& linea2, const QString& linea3, int resaltada)
{
    QString textos[4] = { titulo, linea1, linea2, linea3 };
    for (int i = 0; i < 4; i++) {
        lineasPanel[i]->setTexto(textos[i]);
        lineasPanel[i]->setColor(i == 0 || i == resaltada ? DORADO : CREMA);
        double centroX = lineasPanel[i]->boundingRect().center().x();
        lineasPanel[i]->setPos(ANCHO_PANTALLA / 2.0 - centroX, 225 + i * 60 + (i > 0 ? 20 : 0));
    }
}

void Nivel::mostrarPausa()
{
    mostrarPanel("PAUSA", "Continuar", "Reiniciar nivel", "Volver al menú", opcionPausa + 1);
}

void Nivel::teclaPresionada(int tecla)
{
    Pantalla::teclaPresionada(tecla);
    if (estado == NIVEL_PERDIDO) {
        if (tecla == Qt::Key_Return) {
            terminar(RES_REINTENTAR);
        } else if (tecla == Qt::Key_Escape) {
            terminar(RES_MENU);
        }
        return;
    }
    if (estado == NIVEL_PAUSA) {
        if (tecla == Qt::Key_Escape) {
            estado = NIVEL_JUGANDO;
        } else if (tecla == Qt::Key_W) {
            opcionPausa = (opcionPausa + 2) % 3;
        } else if (tecla == Qt::Key_S) {
            opcionPausa = (opcionPausa + 1) % 3;
        } else if (tecla == Qt::Key_Return) {
            if (opcionPausa == 0) {
                estado = NIVEL_JUGANDO;
            } else if (opcionPausa == 1) {
                terminar(RES_REINTENTAR);
            } else {
                terminar(RES_MENU);
            }
        }
        if (estado == NIVEL_PAUSA) {
            mostrarPausa();
        } else {
            panelPausa->setVisible(false);
            mostrarPanel("", "", "", "", -1);
        }
        teclas.clear();
        return;
    }
    if (tecla == Qt::Key_Escape && estado == NIVEL_JUGANDO) {
        estado = NIVEL_PAUSA;
        opcionPausa = 0;
        ocultarMensaje();
        panelPausa->setVisible(true);
        mostrarPausa();
    }
}
