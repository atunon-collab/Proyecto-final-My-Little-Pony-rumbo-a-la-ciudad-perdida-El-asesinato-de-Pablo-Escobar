#include "Estilo.h"
#include "ErrorJuego.h"
#include <QFontDatabase>
#include <QPen>
#include <QBrush>

static QString familia = "Verdana";

void cargarFuente()
{
    int id = QFontDatabase::addApplicationFont(":/fuentes/PixelifySans.ttf");
    if (id < 0) {
        throw ErrorJuego("No se pudo cargar la fuente del juego");
    }
    familia = QFontDatabase::applicationFontFamilies(id).at(0);
}

QFont fuenteTexto(int pixeles)
{
    QFont fuente(familia);
    fuente.setPixelSize(pixeles);
    fuente.setWeight(QFont::DemiBold);
    // Sin ligaduras: en esta fuente "fi" se dibuja como una sola letra que parece una A
    fuente.setStyleStrategy(QFont::PreferNoShaping);
    return fuente;
}

TextoBorde::TextoBorde(const QString& texto, const QFont& fuente, const QColor& color, double grosor)
    : margen(grosor + 1)
{
    setFont(fuente);
    setText(texto);
    setBrush(CONTORNO);
    setPen(QPen(CONTORNO, grosor * 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    relleno = new QGraphicsSimpleTextItem(texto, this);
    relleno->setFont(fuente);
    relleno->setBrush(color);
    relleno->setPen(Qt::NoPen);
    // El contorno grueso es caro de trazar: Qt lo guarda como imagen y solo lo redibuja si cambia el texto
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

QRectF TextoBorde::boundingRect() const
{
    // El contorno sale "grosor" pixeles por fuera de las letras; sin este margen la imagen guardada lo recorta
    return QGraphicsSimpleTextItem::boundingRect().adjusted(-margen, -margen, margen, margen);
}

void TextoBorde::setTexto(const QString& texto)
{
    setText(texto);
    relleno->setText(texto);
}

void TextoBorde::setColor(const QColor& color)
{
    relleno->setBrush(color);
}
