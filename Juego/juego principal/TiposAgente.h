#ifndef TIPOSAGENTE_H
#define TIPOSAGENTE_H

#include <QPointF>
#include <QString>

enum Situacion { LEJOS, MEDIA, CERCA, ACORRALADO, JUGADOR_AEREO, NUM_SITUACIONES };

enum Accion { HUIR, PROYECTIL, OBSTACULO, TELETRANSPORTE, OBSERVAR, NUM_ACCIONES };

// Lo que el nivel le pasa al Relojero
struct DatosNivel
{
    QPointF posJugador;
    bool jugadorEnAire;
    bool relojeroEnBorde;
    int golpesAlJugador;
    int golpesAlRelojero;
    int progresoJugador;
    bool disponible[NUM_ACCIONES];
};

// Lo que aprende el Relojero (lo guarda Juego)
struct MemoriaRelojero
{
    double tablaQ[NUM_SITUACIONES][NUM_ACCIONES];
    double epsilon;
};

#endif
