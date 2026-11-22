#ifndef RECURSOS_H
#define RECURSOS_H

#include <QPixmap>
#include <QString>
#include <vector>

QPixmap cargarImagen(const QString& ruta);
QPixmap cargarImagen(const QString& ruta, int ancho, int alto);
void cargarTira(const QString& archivo, double escala, std::vector<QPixmap>& derecha, std::vector<QPixmap>& izquierda);
void liberarImagenes();

#endif
