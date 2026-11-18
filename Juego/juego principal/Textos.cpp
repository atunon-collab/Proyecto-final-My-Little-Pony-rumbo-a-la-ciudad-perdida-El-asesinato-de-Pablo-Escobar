#include "Textos.h"

static Actor actor(const QString& sprite, double x, double y, bool derecha, bool visible = true, bool animado = false)
{
    Actor a;
    a.sprite = sprite;
    a.x = x;
    a.y = y;
    a.derecha = derecha;
    a.escala = 1.5;
    a.visible = visible;
    a.animado = animado;
    return a;
}

static LineaDialogo linea(const QString& hablante, const QString& texto, int efecto = EF_NADA, int actorIndice = -1, const QString& voz = "")
{
    LineaDialogo l;
    l.hablante = hablante;
    l.texto = texto;
    l.efecto = efecto;
    l.actor = actorIndice;
    l.voz = voz;
    return l;
}

const double SUELO_MEDELLIN = 482;
const double SUELO_SELVA = 548;
const double SUELO_TEMPLO = 522;

static void agregarManeSix(std::vector<Actor>& actores, double inicio, double suelo)
{
    actores.push_back(actor("twilight_quieta", inicio + 220, suelo, true));
    actores.push_back(actor("aj_quieta", inicio + 330, suelo, true));
    actores.push_back(actor("rarity_quieta", inicio, suelo, true));
    actores.push_back(actor("pinkie_quieta", inicio + 440, suelo, true));
    actores.push_back(actor("rainbow_quieta", inicio + 550, suelo, true));
    actores.push_back(actor("fluttershy_quieta", inicio + 110, suelo, true));
}

DatosEscena escenaIntro()
{
    DatosEscena d;
    d.fondo = "escena_medellin";
    d.musica = "intro";
    agregarManeSix(d.actores, 60, SUELO_MEDELLIN);
    d.actores.push_back(actor("pablo_silueta", 960, SUELO_MEDELLIN, false, false, true));
    d.actores.push_back(actor("relojero_hechizo", 1150, SUELO_MEDELLIN, false, false));
    d.lineas.push_back(linea("Narrador", "Equestria. Un hechizo de Twilight para estudiar \"otros mundos\" sale terriblemente mal...", EF_DESTELLO));
    d.lineas.push_back(linea("Narrador", "Medellín, Colombia. 2 de diciembre de 1993."));
    d.lineas.push_back(linea("Pinkie Pie", "¡Uuuh! ¿Dónde estamos? ¡Huele a arepa recién hecha!"));
    d.lineas.push_back(linea("Twilight Sparkle", "Según ese periódico... estamos en otro mundo. Y en otra época."));
    d.lineas.push_back(linea("Applejack", "Silencio, chicas. Algo está pasando en ese tejado."));
    d.lineas.push_back(linea("Narrador", "En un tejado cercano, rodeado por la policía, un hombre intentaba escapar.", EF_APARECER, 6));
    d.lineas.push_back(linea("Narrador", "Pero el disparo definitivo no salió de la policía.", EF_ANIMAR, 6));
    d.lineas.push_back(linea("Narrador", "Salió de una pistola que no debería existir en 1993.", EF_APARECER, 7));
    d.lineas.push_back(linea("Rainbow Dash", "¡¿Qué fue ese rayo azul?! ¡Eso no era una pistola normal!"));
    d.lineas.push_back(linea("Twilight Sparkle", "Energía cuántica... ¡Alguien vino de otra época y cambió la historia!", EF_NADA, -1, "twilight_oh_no"));
    d.lineas.push_back(linea("???", "Llegan tarde, ponis. Esta historia ahora me pertenece."));
    d.lineas.push_back(linea("Fluttershy", "Lleva... ¿un reloj de arena gigante?"));
    d.lineas.push_back(linea("Twilight Sparkle", "¡Es un viajero del tiempo! Abrió un portal en el último tejado y se está cerrando."));
    d.lineas.push_back(linea("Twilight Sparkle", "Si reúno las pruebas del crimen podré sincronizar mi magia con su portal y alcanzarlo."));
    d.lineas.push_back(linea("Applejack", "Y nosotras atrapamos a sus cómplices. ¡Ya oyeron, a trabajar!", EF_DESAPARECER, 7));
    return d;
}

DatosEscena escenaCaptura()
{
    DatosEscena d;
    d.fondo = "escena_medellin";
    d.musica = "intro";
    agregarManeSix(d.actores, 20, SUELO_MEDELLIN);
    d.actores.push_back(actor("relojero_quieto", 760, SUELO_MEDELLIN, false));
    d.actores.push_back(actor("sunburst_quieto", 880, SUELO_MEDELLIN, false, false));
    d.actores.push_back(actor("lightning_quieta", 990, SUELO_MEDELLIN, false, false));
    d.actores.push_back(actor("caballeron_quieto", 1100, SUELO_MEDELLIN, false, false));
    d.actores.push_back(actor("trixie_quieta", 1210, SUELO_MEDELLIN, false, false));
    d.lineas.push_back(linea("Narrador", "Twilight llega al portal justo a tiempo y se teletransporta encima del Relojero.", EF_DESTELLO));
    d.lineas.push_back(linea("El Relojero", "¡Suéltame! ¡Mi pistola!", EF_NADA, -1, "relojero_dejame"));
    d.lineas.push_back(linea("Narrador", "La pistola temporal cae al tejado. Sin ella, el portal se vuelve inestable... y colapsa.", EF_DESTELLO));
    d.lineas.push_back(linea("El Relojero", "¡No! ¡NO! ...Esto no ha terminado, Twilight Sparkle. Todavía tengo mi reloj."));
    d.lineas.push_back(linea("Narrador", "Con su reloj de muñeca, el Relojero salta de tejado en tejado y desaparece en la noche.", EF_DESAPARECER, 6));
    d.lineas.push_back(linea("Twilight Sparkle", "Se escapó... pero sin la pistola ya no puede salir de esta época."));
    d.lineas.push_back(linea("Rainbow Dash", "¡Y nosotras atrapamos a cuatro de sus cómplices!", EF_APARECER, 7));
    d.lineas.push_back(linea("Rarity", "Sunburst, Lightning Dust, el Doctor Caballeron y... ¿Trixie? Qué decepción, querida.", EF_APARECER, 8));
    d.lineas.push_back(linea("Pinkie Pie", "¡Parecen los invitados más raros de una fiesta!", EF_APARECER, 9));
    d.lineas.push_back(linea("Fluttershy", "Trixie... ¿podrías decirnos, por favor, adónde fue el Relojero?", EF_APARECER, 10));
    d.lineas.push_back(linea("Trixie", "¡Está bien! La Gran y Poderosa Trixie hablará: va a la Ciudad Perdida, en la Sierra Nevada."));
    d.lineas.push_back(linea("Trixie", "Arriba, en el templo, hay un cristal. Con él piensa construir otra máquina del tiempo."));
    d.lineas.push_back(linea("Twilight Sparkle", "Entonces no hay tiempo que perder. ¡A la Sierra Nevada!"));
    return d;
}

DatosEscena escenaViajeSierra()
{
    DatosEscena d;
    d.fondo = "escena_selva";
    d.musica = "intro";
    agregarManeSix(d.actores, 120, SUELO_SELVA);
    d.lineas.push_back(linea("Narrador", "Siguiendo la confesión de Trixie, las ponis viajan a la Sierra Nevada."));
    d.lineas.push_back(linea("Twilight Sparkle", "Trixie dijo que el cristal está en el templo, en lo alto de la Ciudad Perdida."));
    d.lineas.push_back(linea("Twilight Sparkle", "Pero gasté toda mi magia en Medellín. No puedo subir esas terrazas."));
    d.lineas.push_back(linea("Applejack", "Entonces subo yo. Ningún reloj parlante va a romper mi mundo."));
    d.lineas.push_back(linea("Fluttershy", "Los animales dicen que hay ponis extraños patrullando las terrazas... y una poni con mucho, mucho cabello."));
    d.lineas.push_back(linea("Rainbow Dash", "¡Mane-iac! El Relojero contrató a la peor villana de todas."));
    d.lineas.push_back(linea("Applejack", "Que se prepare. ¡Yeehaw!", EF_NADA, -1, "aj_yeehaw"));
    return d;
}

DatosEscena escenaFinal()
{
    DatosEscena d;
    d.fondo = "escena_templo";
    d.musica = "victoria";
    agregarManeSix(d.actores, 40, SUELO_TEMPLO);
    d.actores.push_back(actor("relojero_quieto", 1000, SUELO_TEMPLO, false));
    d.lineas.push_back(linea("Narrador", "La última roca atraviesa el escudo y el Relojero cae de rodillas.", EF_DESTELLO));
    d.lineas.push_back(linea("El Relojero", "Imposible... aprendí cada uno de tus movimientos."));
    d.lineas.push_back(linea("Applejack", "Aprendiste mis movimientos, pero no aprendiste lo terca que soy."));
    d.lineas.push_back(linea("Twilight Sparkle", "Con el cristal del templo puedo reparar la pistola temporal.", EF_DESTELLO));
    d.lineas.push_back(linea("Twilight Sparkle", "El disparo cuántico se borra de la historia. La línea del tiempo vuelve a su lugar."));
    d.lineas.push_back(linea("Rarity", "¿Y qué hacemos con él? ¿Lo dejamos aquí, entre las ruinas?"));
    d.lineas.push_back(linea("Twilight Sparkle", "No. Viene con nosotras a Equestria. La princesa Celestia sabrá qué hacer con él."));
    d.lineas.push_back(linea("Pinkie Pie", "¡Fiesta de regreso! ¿Alguien guardó arepas para el viaje?"));
    d.lineas.push_back(linea("Narrador", "Y así, las ponis regresaron a casa con un prisionero, un cristal... y muchas historias que contar.", EF_DESTELLO, 6));
    return d;
}

DatosEscena escenaCreditos(int puntaje)
{
    DatosEscena d;
    d.fondo = "titulo";
    d.musica = "victoria";
    d.lineas.push_back(linea("FIN", "Puntaje final: " + QString::number(puntaje) + " puntos. ¡Gracias por jugar!"));
    d.lineas.push_back(linea("Créditos", "Rumbo a la Ciudad Perdida · Proyecto final de Informática II · Universidad de Antioquia."));
    d.lineas.push_back(linea("Créditos", "Sprites: colección Desktop Ponies. Fondos y objetos generados con IA. Música: Red Velvet y pistas CC0. Efectos CC0."));
    return d;
}

QString nombrePrueba(int indice)
{
    const char* nombres[] = { "Residuo cuántico", "Casquillo de energía", "Chip del dispositivo", "Fragmento de visor" };
    return nombres[indice];
}

QString imagenPrueba(int indice)
{
    const char* imagenes[] = { "prueba_residuo", "prueba_casquillo", "prueba_chip", "prueba_visor" };
    return imagenes[indice];
}

QString analisisPrueba(int indice)
{
    const char* textos[] = {
        "Residuo cuántico: tiene tinta de pergamino, como la de un erudito.",
        "Casquillo de energía: tiene polvo de nubes; lo transportó un pegaso.",
        "Chip del dispositivo: tiene arena de selva y coordenadas: 11,04 N - 73,93 O.",
        "Fragmento de visor: tiene brillantina mágica de escenario."
    };
    return textos[indice];
}
