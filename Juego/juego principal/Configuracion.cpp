#include "Configuracion.h"

Configuracion configuracionPara(int dificultad)
{
    Configuracion c;
    c.dificultad = MEDIO;
    c.nombre = "Medio";
    c.tiempoPortal = 120;
    c.multObjetos = 1.0;
    c.esperaTeleport = 2.5;
    c.congelamiento = 1.5;
    c.anomaliasPorPrueba = 1;
    c.velocidadAnomalias = 1.0;
    c.esperaProyectil = 3.0;
    c.proyectilDoble = false;
    c.errorPunteria = 40;
    c.rapidezProyectil = 420;
    c.alfa = 0.3;
    c.intervaloRocas = 4.0;
    c.henchponies = 4;
    c.velocidadEnemigos = 1.0;
    c.velocidadTrampas = 1.0;
    c.velocidadPlataformas = 1.0;
    c.totems = 3;

    if (dificultad == FACIL) {
        c.dificultad = FACIL;
        c.nombre = "Facil";
        c.tiempoPortal = 168;
        c.multObjetos = 1.5;
        c.esperaTeleport = 1.75;
        c.congelamiento = 0.9;
        c.anomaliasPorPrueba = 0;
        c.velocidadAnomalias = 0.8;
        c.esperaProyectil = 4.5;
        c.errorPunteria = 80;
        c.rapidezProyectil = 340;
        c.alfa = 0.2;
        c.intervaloRocas = 6.0;
        c.henchponies = 2;
        c.velocidadEnemigos = 0.8;
        c.velocidadTrampas = 0.8;
        c.velocidadPlataformas = 0.75;
        c.totems = 4;
    } else if (dificultad == DISCORDIA) {
        c.dificultad = DISCORDIA;
        c.nombre = "Discordia";
        c.tiempoPortal = 90;
        c.multObjetos = 0.5;
        c.esperaTeleport = 4.0;
        c.congelamiento = 2.4;
        c.anomaliasPorPrueba = 2;
        c.velocidadAnomalias = 1.4;
        c.esperaProyectil = 1.8;
        c.proyectilDoble = true;
        c.errorPunteria = 15;
        c.rapidezProyectil = 520;
        c.alfa = 0.5;
        c.intervaloRocas = 2.5;
        c.henchponies = 6;
        c.velocidadEnemigos = 1.3;
        c.velocidadTrampas = 1.3;
        c.velocidadPlataformas = 1.35;
        c.totems = 1;
    }
    return c;
}
