#include "Escena.h"
#include "Juego.h"
#include "Sonido.h"
#include "Constantes.h"
#include "Estilo.h"

Figura::Figura(const Actor& actor)
    : animando(false)
{
    agregarAnimacion("base", actor.sprite, !actor.animado, actor.escala);
    mirarHacia(actor.derecha);
    setPos(actor.x, actor.y);
    setVisible(actor.visible);
    setZValue(actor.y);
}

void Figura::actualizar(double dt)
{
    if (animando) {
        avanzarAnimacion(dt);
    }
}

Escena::Escena(Juego* juego, const DatosEscena& datos)
    : Pantalla(juego), datos(datos), linea(0)
{
    agregarFondo(datos.fondo);
    for (const Actor& actor : datos.actores) {
        Figura* figura = new Figura(actor);
        escena->addItem(figura);
        figuras.push_back(figura);
    }
    agregarMarco("ui_dialogo", 40, 535);
    textoNombre = agregarTextoBorde("", fuenteTexto(20), DORADO, 82, 593, 3);
    textoDialogo = agregarTexto("", fuenteTexto(22), CREMA, 76, 613, 1110);
    agregarTexto("Enter continuar | Esc saltar", fuenteTexto(15), QColor(170, 170, 200), 950, 591);
    if (!datos.musica.isEmpty()) {
        juego->getSonido()->musica(datos.musica);
    }
    empezarLinea();
}

void Escena::empezarLinea()
{
    const LineaDialogo& actual = datos.lineas[linea];
    textoNombre->setTexto(actual.hablante);
    textoDialogo->setPlainText(actual.texto);
    if (actual.actor >= 0 && actual.actor < int(figuras.size())) {
        Figura* figura = figuras[actual.actor];
        if (actual.efecto == EF_APARECER) {
            figura->setVisible(true);
        } else if (actual.efecto == EF_DESAPARECER) {
            figura->setVisible(false);
        } else if (actual.efecto == EF_ANIMAR) {
            figura->setVisible(true);
            figura->animando = true;
        }
    }
}

void Escena::actualizar(double dt)
{
    for (Figura* figura : figuras) {
        figura->actualizar(dt);
    }
}

void Escena::teclaPresionada(int tecla)
{
    Pantalla::teclaPresionada(tecla);
    if (tecla == Qt::Key_Escape) {
        terminar(RES_GANO);
        return;
    }
    if (tecla != Qt::Key_Return) {
        return;
    }
    if (linea + 1 >= int(datos.lineas.size())) {
        terminar(RES_GANO);
        return;
    }
    linea++;
    empezarLinea();
}
