#ifndef CONFIGURACION_H
#define CONFIGURACION_H

const int FACIL = 0;
const int MEDIO = 1;
const int DISCORDIA = 2;
const int SUSTENTACION = 3;

// Valores de cada dificultad
struct Configuracion
{
    int dificultad;

    double tiempoPortal;
    double multObjetos;
    double esperaTeleport;
    double congelamiento;
    int anomaliasPorPrueba;
    double velocidadAnomalias;

    double esperaProyectil;
    bool proyectilDoble;
    double errorPunteria;
    double rapidezProyectil;
    double alfa;

    double intervaloRocas;
    int henchponies;
    double velocidadEnemigos;
    double velocidadTrampas;
    double velocidadPlataformas;
    int totems;
};

Configuracion configuracionPara(int dificultad);

#endif
