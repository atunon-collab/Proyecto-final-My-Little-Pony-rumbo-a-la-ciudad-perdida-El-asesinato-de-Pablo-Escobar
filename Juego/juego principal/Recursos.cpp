#include "Recursos.h"
#include "ErrorJuego.h"
#include <QImage>
#include <QTransform>
#include <map>

static std::map<QString, QPixmap> imagenesCargadas;

// Cada tira se corta una sola vez; las siguientes entidades con el mismo sprite reusan los cuadros
struct TiraCargada
{
    std::vector<QPixmap> derecha;
    std::vector<QPixmap> izquierda;
};
static std::map<QString, TiraCargada> tirasCargadas;

static std::map<QString, int> cuadrosPorTira = {
    { "aj_colgada", 90 }, { "aj_quieta", 131 }, { "aj_salto", 26 }, { "aj_trote", 16 },
    { "caballeron_quieto", 107 }, { "fluttershy_quieta", 163 }, { "hench1_trote", 16 }, { "hench2_trote", 16 },
    { "hench3_trote", 16 }, { "hench4_trote", 16 }, { "lightning_quieta", 154 }, { "maneiac_quieta", 128 },
    { "maneiac_vencida", 126 }, { "pablo_silueta", 66 }, { "pinkie_quieta", 131 }, { "rainbow_quieta", 189 },
    { "rarity_quieta", 133 }, { "relojero_hechizo", 88 }, { "relojero_quieto", 74 }, { "relojero_trote", 16 },
    { "sunburst_quieto", 96 }, { "trixie_quieta", 76 }, { "twilight_galope", 12 }, { "twilight_leer", 60 },
    { "twilight_magia", 130 }, { "twilight_quieta", 131 }, { "twilight_teleport", 60 }, { "twilight_trote", 16 }
};

QPixmap cargarImagen(const QString& ruta)
{
    auto encontrada = imagenesCargadas.find(ruta);
    if (encontrada != imagenesCargadas.end()) {
        return encontrada->second;
    }
    QPixmap imagen(ruta);
    if (imagen.isNull()) {
        throw ErrorJuego("No se pudo cargar la imagen: " + ruta);
    }
    imagenesCargadas[ruta] = imagen;
    return imagen;
}

QPixmap cargarImagen(const QString& ruta, int ancho, int alto)
{
    // Se guarda ya escalada: los efectos y proyectiles piden la misma imagen muchas veces durante el juego
    QString clave = ruta + "@" + QString::number(ancho) + "x" + QString::number(alto);
    auto encontrada = imagenesCargadas.find(clave);
    if (encontrada != imagenesCargadas.end()) {
        return encontrada->second;
    }
    QPixmap imagen = cargarImagen(ruta).scaled(ancho, alto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imagenesCargadas[clave] = imagen;
    return imagen;
}

void cargarTira(const QString& archivo, double escala, std::vector<QPixmap>& derecha, std::vector<QPixmap>& izquierda)
{
    QString clave = archivo + "@" + QString::number(escala);
    auto encontrada = tirasCargadas.find(clave);
    if (encontrada != tirasCargadas.end()) {
        derecha = encontrada->second.derecha;
        izquierda = encontrada->second.izquierda;
        return;
    }
    QImage imagen(":/sprites/" + archivo + ".png");
    auto cantidad = cuadrosPorTira.find(archivo);
    if (imagen.isNull() || cantidad == cuadrosPorTira.end()) {
        throw ErrorJuego("No se pudo cargar la animacion: " + archivo);
    }
    int cuadros = cantidad->second;
    int ancho = imagen.width() / cuadros;
    QImage anterior;
    for (int i = 0; i < cuadros; i++) {
        QImage cuadro = imagen.copy(i * ancho, 0, ancho, imagen.height());
        if (i > 0 && cuadro == anterior) {
            derecha.push_back(derecha.back());
            izquierda.push_back(izquierda.back());
            continue;
        }
        anterior = cuadro;
        if (escala != 1.0) {
            cuadro = cuadro.scaled(int(ancho * escala), int(imagen.height() * escala), Qt::IgnoreAspectRatio, Qt::FastTransformation);
        }
        derecha.push_back(QPixmap::fromImage(cuadro));
        izquierda.push_back(QPixmap::fromImage(cuadro.transformed(QTransform().scale(-1, 1))));
    }
    tirasCargadas[clave] = { derecha, izquierda };
}

void liberarImagenes()
{
    imagenesCargadas.clear();
    tirasCargadas.clear();
}
