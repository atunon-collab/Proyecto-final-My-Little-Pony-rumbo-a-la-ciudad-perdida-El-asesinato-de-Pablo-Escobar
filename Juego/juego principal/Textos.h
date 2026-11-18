#ifndef TEXTOS_H
#define TEXTOS_H

#include "Escena.h"
#include <vector>

// Dialogos y textos del juego

DatosEscena escenaIntro();
DatosEscena escenaCaptura();
DatosEscena escenaViajeSierra();
DatosEscena escenaFinal();
DatosEscena escenaCreditos(int puntaje);

QString nombrePrueba(int indice);
QString imagenPrueba(int indice);
QString analisisPrueba(int indice);

#endif
