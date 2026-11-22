#include "Relojero.h"
#include <QRandomGenerator>
#include <QtMath>

const double PERIODO_DECISION = 0.25;
const double TIEMPO_OBSERVACION = 1.5;

Relojero::Relojero(MemoriaRelojero* memoria, const Configuracion& config)
    : Personaje(3, 150), memoria(memoria), alfa(config.alfa), reloj(0), observando(0), accion(OBSERVAR),
      situacionAntes(LEJOS), puntoHuida(0, 0), tiempoHuida(0), tiempoHechizo(0), efectoTeleport(0)
{
    agregarAnimacion("quieto", "relojero_quieto");
    agregarAnimacion("trote", "relojero_trote");
    agregarAnimacion("hechizo", "relojero_hechizo");
    definirCaja(50, 70, true);
    for (int i = 0; i < NUM_ACCIONES; i++) {
        esperas[i] = 0;
    }
}

void Relojero::actualizar(double dt)
{
    for (int i = 0; i < NUM_ACCIONES; i++) {
        if (esperas[i] > 0) {
            esperas[i] -= dt;
        }
    }
    if (tiempoHuida > 0) {
        tiempoHuida -= dt;
    }
    if (tiempoHechizo > 0) {
        tiempoHechizo -= dt;
    }
    if (efectoTeleport > 0) {
        efectoTeleport -= dt;
        setOpacity(1.0 - efectoTeleport * 2);
    } else {
        actualizarParpadeo(dt);
    }
    if (tiempoHechizo > 0) {
        ponerAnimacion("hechizo");
    } else if (velocidad.x() != 0 || velocidad.y() != 0) {
        ponerAnimacion("trote");
    } else {
        ponerAnimacion("quieto");
    }
    mirarSegunVelocidad();
    avanzarAnimacion(dt);
}

bool Relojero::pensar(double dt, const DatosNivel& datos)
{
    reloj += dt;
    if (reloj < PERIODO_DECISION) {
        return false;
    }
    reloj = 0;
    if (velocidad.x() == 0 && velocidad.y() == 0) {
        mirarHacia(datos.posJugador.x() > x());
    }
    if (observando > 0) {
        observando -= PERIODO_DECISION;
        if (observando <= 0) {
            aprender(datos);
        }
        return false;
    }
    situacionAntes = percibir(datos);
    accion = decidir(situacionAntes, datos);
    datosAntes = datos;
    observando = TIEMPO_OBSERVACION;
    return true;
}

double Relojero::distanciaA(QPointF punto) const
{
    QPointF d = punto - pos();
    return qSqrt(d.x() * d.x() + d.y() * d.y());
}

Situacion Relojero::percibir(const DatosNivel& datos) const
{
    double distancia = distanciaA(datos.posJugador);
    if (datos.jugadorEnAire && distancia < 500) {
        return JUGADOR_AEREO;
    }
    if (distancia < 230) {
        return datos.relojeroEnBorde ? ACORRALADO : CERCA;
    }
    if (distancia < 520) {
        return MEDIA;
    }
    return LEJOS;
}

Accion Relojero::decidir(Situacion situacion, const DatosNivel& datos)
{
    // Arreglo fijo: nunca hay mas de NUM_ACCIONES acciones validas
    Accion validas[NUM_ACCIONES];
    int cantidad = 0;
    for (int i = 0; i < NUM_ACCIONES; i++) {
        if (esperas[i] <= 0 && datos.disponible[i]) {
            validas[cantidad++] = Accion(i);
        }
    }
    if (cantidad == 0) {
        return OBSERVAR;
    }
    if (QRandomGenerator::global()->generateDouble() < memoria->epsilon) {
        return validas[QRandomGenerator::global()->bounded(cantidad)];
    }
    Accion mejor = validas[0];
    for (int i = 1; i < cantidad; i++) {
        if (memoria->tablaQ[situacion][validas[i]] > memoria->tablaQ[situacion][mejor]) {
            mejor = validas[i];
        }
    }
    return mejor;
}

void Relojero::aprender(const DatosNivel& datos)
{
    double r = 0;
    if (datos.golpesAlJugador > datosAntes.golpesAlJugador) {
        r += 10;
    }
    bool seAlejo = distanciaA(datos.posJugador) > distanciaA(datosAntes.posJugador) + 40;
    if ((accion == HUIR || accion == TELETRANSPORTE) && seAlejo) {
        r += 3;
    }
    if (datos.progresoJugador > datosAntes.progresoJugador) {
        r -= 5;
    }
    if (datos.golpesAlRelojero > datosAntes.golpesAlRelojero) {
        r -= 8;
    }
    if (r == 0) {
        r = -1;
    }
    // Q = Q + alfa*(r - Q)
    double& q = memoria->tablaQ[situacionAntes][accion];
    q = q + alfa * (r - q);
    memoria->epsilon = qMax(0.05, memoria->epsilon * 0.98);
}

void Relojero::huirDe(QPointF punto, double segundos)
{
    puntoHuida = punto;
    tiempoHuida = segundos;
}

QPointF Relojero::pasoHuida(double dt) const
{
    double distancia = distanciaA(puntoHuida);
    if (tiempoHuida <= 0 || distancia < 1) {
        return QPointF(0, 0);
    }
    return (pos() - puntoHuida) / distancia * rapidez * dt;
}

void Relojero::mover(QPointF paso, double dt)
{
    setPos(pos() + paso);
    velocidad = paso / dt;
}

void Relojero::teletransportar(QPointF destino)
{
    setPos(destino);
    tiempoHuida = 0;
    velocidad = QPointF(0, 0);
    efectoTeleport = 0.5;
}
