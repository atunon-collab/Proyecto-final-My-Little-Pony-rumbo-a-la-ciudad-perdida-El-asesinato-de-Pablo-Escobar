#include "Twilight.h"

const double GRAVEDAD_SALTO = 1500;
const double VELOCIDAD_SALTO = 520;

Twilight::Twilight(const Configuracion& config)
    : Personaje(3, 210), magia(100), esperaTeleport(0), analizando(0), congelada(0),
      esperaTeleportMax(config.esperaTeleport), efectoTeleport(0), inmuneCongelar(0), vz(0)
{
    agregarAnimacion("quieta", "twilight_quieta");
    agregarAnimacion("trote", "twilight_trote");
    agregarAnimacion("galope", "twilight_galope");
    agregarAnimacion("teleport", "twilight_teleport", false);
    agregarAnimacion("magia", "twilight_magia");
    agregarAnimacion("leer", "twilight_leer");
    definirCaja(34, 18, true);
    crearSombra(60);
}

void Twilight::saltar()
{
    if (altura <= 0 && congelada <= 0) {
        vz = VELOCIDAD_SALTO;
        analizando = 0;
    }
}

void Twilight::actualizar(double dt)
{
    actualizarParpadeo(dt);
    if (esperaTeleport > 0) {
        esperaTeleport -= dt;
    }
    if (efectoTeleport > 0) {
        efectoTeleport -= dt;
    }
    if (congelada > 0) {
        congelada -= dt;
    }
    if (inmuneCongelar > 0) {
        inmuneCongelar -= dt;
    }
    magia += 3 * dt;
    if (magia > 100) {
        magia = 100;
    }

    if (altura > 0 || vz > 0) {
        // vz = vz - g*dt ; z = z + vz*dt
        vz -= GRAVEDAD_SALTO * dt;
        double z = altura + vz * dt;
        if (z <= 0) {
            z = 0;
            vz = 0;
        }
        setAltura(z);
    }

    if (efectoTeleport > 0) {
        ponerAnimacion("teleport");
    } else if (analizando > 0) {
        ponerAnimacion("leer");
    } else if (congelada > 0) {
        ponerAnimacion("magia");
    } else if (altura > 0) {
        ponerAnimacion("galope");
    } else if (velocidad.x() != 0 || velocidad.y() != 0) {
        ponerAnimacion("trote");
    } else {
        ponerAnimacion("quieta");
    }
    mirarSegunVelocidad();
    avanzarAnimacion(dt);
    if (congelada > 0) {
        setOpacity(0.6);
    }
}

bool Twilight::puedeTeletransportarse() const
{
    return esperaTeleport <= 0 && magia >= 25 && congelada <= 0 && altura <= 0;
}

void Twilight::terminarTeletransporte()
{
    magia -= 25;
    esperaTeleport = esperaTeleportMax;
    efectoTeleport = 0.5;
}

bool Twilight::congelar(double segundos)
{
    if (inmuneCongelar > 0) {
        return false;
    }
    congelada = segundos;
    inmuneCongelar = segundos + 1.5;
    return true;
}
