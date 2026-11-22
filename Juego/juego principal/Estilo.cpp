#include "Estilo.h"
#include "ErrorJuego.h"
#include <QFontDatabase>
#include <QPen>
#include <QBrush>

static QString familiaTexto = "Verdana";
static QString familiaTitulo = "Verdana";

static QString registrar(const QString& ruta)
{
    int id = QFontDatabase::addApplicationFont(ruta);
    if (id < 0) {
        throw ErrorJuego("No se pudo cargar la fuente: " + ruta);
    }
    return QFontDatabase::applicationFontFamilies(id).at(0);
}

void cargarFuentes()
{
    familiaTexto = registrar(":/fuentes/PixelifySans.ttf");
    familiaTitulo = registrar(":/fuentes/PressStart2P.ttf");
}

QFont fuenteTexto(int pixeles)
{
    QFont fuente(familiaTexto);
    fuente.setPixelSize(pixeles);
    fuente.setWeight(QFont::DemiBold);
    return fuente;
}

QFont fuenteTitulo(int pixeles)
{
    QFont fuente(familiaTitulo);
    fuente.setPixelSize(pixeles);
    return fuente;
}

TextoBorde::TextoBorde(const QString& texto, const QFont& fuente, const QColor& color, double grosor)
{
    setFont(fuente);
    setText(texto);
    setBrush(CONTORNO);
    setPen(QPen(CONTORNO, grosor * 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    relleno = new QGraphicsSimpleTextItem(texto, this);
    relleno->setFont(fuente);
    relleno->setBrush(color);
    relleno->setPen(Qt::NoPen);
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
