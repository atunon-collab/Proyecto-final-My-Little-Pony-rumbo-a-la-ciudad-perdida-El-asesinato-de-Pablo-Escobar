#include "Recursos.h"
#include "ErrorJuego.h"
#include <QImage>
#include <QTransform>
#include <map>

static std::map<QString, QPixmap> imagenesCargadas;

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
    return cargarImagen(ruta).scaled(ancho, alto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void cargarTira(const QString& archivo, double escala, std::vector<QPixmap>& derecha, std::vector<QPixmap>& izquierda)
{
    QImage imagen(":/sprites/" + archivo + ".png");
    if (imagen.isNull() || cuadrosPorTira.count(archivo) == 0) {
        throw ErrorJuego("No se pudo cargar la animacion: " + archivo);
    }
    int cuadros = cuadrosPorTira[archivo];
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
}

void liberarImagenes()
{
    imagenesCargadas.clear();
}
