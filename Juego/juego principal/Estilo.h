#ifndef ESTILO_H
#define ESTILO_H

#include <QFont>
#include <QColor>
#include <QString>
#include <QGraphicsSimpleTextItem>

void cargarFuentes();
QFont fuenteTexto(int pixeles);
QFont fuenteTitulo(int pixeles);

const QColor DORADO(255, 214, 92);
const QColor CREMA(250, 240, 215);
const QColor CONTORNO(25, 12, 6);

// Texto con contorno: abajo una copia oscura con borde grueso y encima el relleno de color
class TextoBorde : public QGraphicsSimpleTextItem
{
public:
    TextoBorde(const QString& texto, const QFont& fuente, const QColor& color, double grosor);
    void setTexto(const QString& texto);
    void setColor(const QColor& color);

private:
    QGraphicsSimpleTextItem* relleno;
};

#endif
