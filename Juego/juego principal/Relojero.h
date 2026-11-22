#ifndef RELOJERO_H
#define RELOJERO_H

#include "Personaje.h"
#include "TiposAgente.h"
#include "Configuracion.h"

// El Relojero, el agente que aprende con la tabla Q
class Relojero : public Personaje
{
public:
    Relojero(MemoriaRelojero* memoria, const Configuracion& config);

    void actualizar(double dt);
    bool pensar(double dt, const DatosNivel& datos);
    Accion getAccion() const { return accion; }
    void activarEspera(Accion a, double segundos) { esperas[a] = segundos; }

    void huirDe(QPointF punto, double segundos);
    QPointF pasoHuida(double dt) const;
    void mover(QPointF paso, double dt);
    void teletransportar(QPointF destino);
    void lanzarHechizo() { tiempoHechizo = 0.7; }

private:
    MemoriaRelojero* memoria;
    double alfa;
    double esperas[NUM_ACCIONES];
    double reloj;
    double observando;
    Accion accion;
    Situacion situacionAntes;
    DatosNivel datosAntes;
    QPointF puntoHuida;
    double tiempoHuida;
    double tiempoHechizo;
    double efectoTeleport;

    double distanciaA(QPointF punto) const;
    Situacion percibir(const DatosNivel& datos) const;
    Accion decidir(Situacion situacion, const DatosNivel& datos);
    void aprender(const DatosNivel& datos);
};

#endif
