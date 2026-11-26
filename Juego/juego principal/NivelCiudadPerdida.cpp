#include "NivelCiudadPerdida.h"
#include "Juego.h"
#include "Sonido.h"
#include "Constantes.h"
#include "Applejack.h"
#include "Relojero.h"
#include "Proyectiles.h"
#include "Pendulares.h"
#include "Enemigos.h"
#include "Recursos.h"
#include "Estilo.h"
#include <QLineF>
#include <QtMath>
#include <QPen>

const double ALTO_CIUDAD = 3600;
const double GRAVEDAD = 1800;
const double VELOCIDAD_SALTO = 780;
const double IMPULSO_SOLTAR = 700;
const double GRAVEDAD_MANZANAS = 1200;
const double TIRO_VX = 420;
const double TIRO_VY = -780;
const double Y_CIMA = 600;
const double Y_TEMPLO = 390;

NivelCiudadPerdida::NivelCiudadPerdida(Juego* juego, MemoriaRelojero* memoria)
    : NivelLateral(juego, ANCHO_PANTALLA, ALTO_CIUDAD, GRAVEDAD), aj(nullptr), relojero(nullptr), maneiac(nullptr),
      escalera(nullptr), fondo(nullptr), perchaActual(0), faseJefe(false), alturaMaxima(0), valorHud(-1)
{
    cargar(memoria);
}

void NivelCiudadPerdida::cargar(MemoriaRelojero* memoria)
{
    juego->getSonido()->musica("ciudad");
    fondo = escena->addPixmap(cargarImagen(":/fondos/ciudad.png", ANCHO_PANTALLA, 1920));
    fondo->setZValue(-100);

    struct DatosPlataforma { double x, y, ancho, alto; const char* textura; TipoPlataforma tipo; double fase; };
    DatosPlataforma datos[] = {
        { 0, 3560, 1280, 40, "plat_piedra", PLAT_FIJA, 0 },
        { 340, 3440, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 660, 3320, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 940, 3200, 300, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 600, 3080, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 340, 2960, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 20, 2840, 320, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 860, 2880, 380, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 600, 2760, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 340, 2640, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 20, 2520, 320, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 400, 2400, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 660, 2280, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 940, 2160, 300, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 600, 2040, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 0, 1920, 560, 40, "plat_piedra", PLAT_FIJA, 0 },
        { 1000, 1920, 280, 40, "plat_piedra", PLAT_FIJA, 0 },
        { 40, 1680, 300, 30, "plat_arenisca", PLAT_DESMORONA, 0 },
        { 460, 1560, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 720, 1440, 240, 30, "plat_eco", PLAT_ECO, 0 },
        { 980, 1320, 260, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 640, 1200, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 340, 1080, 240, 30, "plat_eco", PLAT_ECO, 1.6 },
        { 20, 960, 300, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 400, 840, 240, 30, "plat_arenisca", PLAT_DESMORONA, 0 },
        { 660, 720, 240, 30, "plat_piedra", PLAT_FIJA, 0 },
        { 0, Y_CIMA, 580, 60, "plat_piedra", PLAT_FIJA, 0 },
        { 920, Y_CIMA, 360, 60, "plat_piedra", PLAT_FIJA, 0 },
        { 440, Y_TEMPLO, 400, 24, "plat_piedra", PLAT_FIJA, 0 }
    };
    for (const DatosPlataforma& d : datos) {
        agregarPlataforma(d.x, d.y, d.ancho, d.alto, d.textura, d.tipo, d.fase);
    }
    escalera = agregarPlataforma(400, 1800, 240, 30, "plat_piedra");
    escalera->setHabilitada(false);

    ObjetoFijo* muro = new ObjetoFijo(OBJ_MURO, "muro", QPointF(430, 2960), 84, 200);
    agregarEntidad(muro);
    muros.push_back(muro);
    solidos.push_back(muro);

    QPointF posicionesPinchos[] = { QPointF(780, 3320), QPointF(760, 1200) };
    for (const QPointF& p : posicionesPinchos) {
        ObjetoFijo* pincho = new ObjetoFijo(OBJ_PINCHOS, "pinchos", p, 56, 30);
        agregarEntidad(pincho);
        pinchos.push_back(pincho);
    }
    QPointF posicionesGanchos[] = { QPointF(220, 2300), QPointF(200, 760) };
    for (const QPointF& p : posicionesGanchos) {
        ObjetoFijo* gancho = new ObjetoFijo(OBJ_GANCHO, "gancho", p, 40, 40, false);
        agregarEntidad(gancho);
        ganchos.push_back(gancho);
        double ancla = 0;
        for (Plataforma* plataforma : plataformas) {
            QRectF r = plataforma->getRect();
            if (r.left() < p.x() && r.right() > p.x() && r.bottom() <= p.y()) {
                ancla = qMax(ancla, r.bottom());
            }
        }
        QGraphicsLineItem* cadena = escena->addLine(p.x(), ancla, p.x(), p.y() - 18, QPen(QColor(60, 55, 50), 4));
        cadena->setZValue(4);
    }

    std::vector<QPointF> posicionesTotems;
    if (config.totems >= 4) {
        posicionesTotems = { QPointF(200, 2840), QPointF(200, 2520), QPointF(120, 1920), QPointF(160, 960) };
    } else if (config.totems == 3) {
        posicionesTotems = { QPointF(200, 2840), QPointF(120, 1920), QPointF(160, 960) };
    } else {
        posicionesTotems = { QPointF(120, 1920) };
    }
    for (const QPointF& p : posicionesTotems) {
        ObjetoFijo* totem = new ObjetoFijo(OBJ_TOTEM, "totem", p, 50, 90);
        agregarEntidad(totem);
        totems.push_back(totem);
    }

    QPointF posicionesManzanas[] = { QPointF(1180, 3200), QPointF(460, 2640), QPointF(480, 1920), QPointF(1180, 1320) };
    int cantidadManzanas = qMin(4, qRound(3 * config.multObjetos));
    for (int i = 0; i < cantidadManzanas; i++) {
        Item* manzana = new Item(ITEM_MANZANA, posicionesManzanas[i], false);
        agregarEntidad(manzana);
        manzanas.push_back(manzana);
    }

    Liana* liana = new Liana(QPointF(580, 2430), 350, 50);
    agregarEntidad(liana);
    lianas.push_back(liana);
    trampas.push_back(new TrampaColgante(QPointF(720, 2070), 180, 30, "tronco", 90, 60, config.velocidadTrampas));
    trampas.push_back(new TrampaColgante(QPointF(460, 870), 180, 40, "hacha", 70, 80, config.velocidadTrampas));
    trampas.push_back(new TrampaColgante(QPointF(840, 1230), 200, 25, "tronco", 90, 60, config.velocidadTrampas));
    for (TrampaColgante* trampa : trampas) {
        agregarEntidad(trampa);
    }

    double rangos[6][3] = { { 960, 1220, 3200 }, { 880, 1220, 2880 }, { 960, 1220, 2160 },
                            { 1000, 1220, 1320 }, { 40, 320, 2520 }, { 40, 300, 960 } };
    for (int i = 0; i < config.henchponies; i++) {
        QString sprite = "hench" + QString::number(i % 4 + 1);
        Patrullero* secuaz = new Patrullero(sprite, rangos[i][0], rangos[i][1], rangos[i][2], 80 * config.velocidadEnemigos);
        agregarEntidad(secuaz);
        secuaces.push_back(secuaz);
    }

    maneiac = new ManeIac(QPointF(1160, 1920));
    agregarEntidad(maneiac);

    perchas = { QPointF(1180, 3040), QPointF(100, 2700), QPointF(1180, 2420), QPointF(100, 2060),
                QPointF(1180, 1720), QPointF(100, 1380), QPointF(1180, 1000) };
    for (const QPointF& p : perchas) {
        agregarEntidad(new ObjetoFijo(OBJ_DECORADO, "cornisa", p + QPointF(0, 22), 150, 50, false));
    }

    aj = new Applejack(GRAVEDAD);
    aj->setPos(120, 3560);
    aj->setZValue(30);
    agregarEntidad(aj);
    puntoControl = aj->pos();
    ultimoSeguro = aj->pos();

    relojero = new Relojero(memoria, config);
    relojero->setPos(perchas[0]);
    relojero->setZValue(29);
    agregarEntidad(relojero);

    crearCorazones(3, true);
    textoAltura = crearPlacaDerecha("icono_flecha");
    mostrarMensaje("¡Sube hasta el templo! E lanza manzanas | W liana | R lazo", 5);
    moverCamara(aj->pos());
}

void NivelCiudadPerdida::teclaPresionada(int tecla)
{
    Nivel::teclaPresionada(tecla);
    if (!enCurso()) {
        return;
    }
    if (tecla == Qt::Key_Space) {
        if (aj->estaColgada()) {
            QPointF v = aj->getVelocidad();
            aj->soltarse();
            aj->movimiento.lanzar(aj->pos(), v.x() * 1.25, qMin(v.y() * 1.25, 0.0) - IMPULSO_SOLTAR);
            aj->enSuelo = false;
        } else if (aj->enSuelo) {
            aj->movimiento.setVy(-VELOCIDAD_SALTO);
            aj->enSuelo = false;
        }
    } else if (tecla == Qt::Key_E && aj->puedeLanzar()) {
        lanzarManzana();
    } else if (tecla == Qt::Key_R && !aj->estaColgada() && aj->esperaAgarre <= 0) {
        for (ObjetoFijo* gancho : ganchos) {
            if (gancho->y() < aj->y() - 60 && QLineF(gancho->pos(), aj->pos()).length() < 340) {
                aj->lanzarLazo(gancho->pos(), aj->x() < gancho->x());
                break;
            }
        }
    }
}

void NivelCiudadPerdida::lanzarManzana()
{
    double lado = aj->miraDerecha() ? 1 : -1;
    Manzana* manzana = new Manzana(aj->pos() + QPointF(30 * lado, -60), TIRO_VX * lado, TIRO_VY, GRAVEDAD_MANZANAS);
    agregarEntidad(manzana);
    tiros.push_back(manzana);
    aj->lanzar();
}

void NivelCiudadPerdida::moverApplejack(double dt)
{
    if (aj->estaColgada()) {
        if (aj->colgadaDelLazo()) {
            aj->getPendulo()->actualizar(dt);
        }
        QPointF nueva = aj->getPendulo()->posicion() + QPointF(0, ALTURA_MANOS);
        aj->setVelocidad((nueva - aj->pos()) / dt);
        aj->setPos(nueva);
        return;
    }
    if (estaPresionada(Qt::Key_W) && aj->esperaAgarre <= 0) {
        for (Liana* liana : lianas) {
            if (QLineF(liana->pos(), aj->pos() + QPointF(0, -70)).length() < 95) {
                aj->colgarse(liana->getPendulo());
                return;
            }
        }
    }

    int direccion = 0;
    if (estaPresionada(Qt::Key_A)) {
        direccion -= 1;
    }
    if (estaPresionada(Qt::Key_D)) {
        direccion += 1;
    }
    if (aj->enSuelo || direccion != 0) {
        aj->movimiento.setVx(direccion * aj->getRapidez());
    }
    Plataforma* debajo = nullptr;
    aj->enSuelo = moverConGravedad(aj, aj->movimiento, dt, debajo);
    aj->plataformaActual = debajo;
    if (debajo != nullptr) {
        debajo->pisar();
        if (debajo->getTipo() == PLAT_FIJA) {
            ultimoSeguro = aj->pos();
        }
    }
    aj->setVelocidad(QPointF(aj->enSuelo ? direccion * aj->getRapidez() : aj->movimiento.getVx(), aj->movimiento.getVy()));
}

void NivelCiudadPerdida::actualizarTiros(double dt)
{
    for (Manzana* manzana : tiros) {
        manzana->actualizar(dt);
        if (manzana->y() > altoMundo || manzana->x() < 0 || manzana->x() > anchoMundo) {
            manzana->desactivar();
        }
        QRectF caja = manzana->getCaja();
        for (Plataforma* p : plataformas) {
            if (manzana->estaActiva() && p->esSolida() && caja.intersects(p->getRect())) {
                manzana->desactivar();
            }
        }
        for (ObjetoFijo* muro : muros) {
            if (manzana->estaActiva() && manzana->colisionaCon(muro)) {
                manzana->desactivar();
                if (muro->golpear()) {
                    mostrarMensaje("¡El muro se rompió!", 1.5);
                }
            }
        }
        for (Patrullero* secuaz : secuaces) {
            if (manzana->estaActiva() && manzana->colisionaCon(secuaz)) {
                manzana->desactivar();
                secuaz->desactivar();
                crearEfecto("efecto_golpe", secuaz->pos() + QPointF(0, -40), 70, 0.4, 1.6);
                juego->getSonido()->efecto("golpe");
            }
        }
        if (manzana->estaActiva() && !maneiac->estaVencida() && !maneiac->esInvulnerable() && manzana->colisionaCon(maneiac)) {
            manzana->desactivar();
            maneiac->recibirDanio(1);
            crearEfecto("efecto_explosion", maneiac->pos() + QPointF(0, -80), 110, 0.5, 1.5);
            juego->getSonido()->efecto("golpe");
            if (maneiac->estaVencida()) {
                escalera->setHabilitada(true);
                mostrarMensaje("¡Mane-iac fue vencida! Se abrió la escalera", 3);
            }
        }
        if (manzana->estaActiva() && faseJefe && !relojero->esInvulnerable() && manzana->colisionaCon(relojero)) {
            manzana->desactivar();
            golpesAlRelojero++;
            relojero->recibirDanio(1);
            relojero->hacerInvulnerable(1.5);
            crearEfecto("efecto_explosion", relojero->pos() + QPointF(0, -45), 130, 0.6, 1.5);
            juego->getSonido()->efecto("golpe");
            mostrarMensaje("¡Golpe al Relojero! " + QString::number(golpesAlRelojero) + "/3", 2);
        }
    }
    borrarInactivas(tiros);
    borrarInactivas(secuaces);
}

void NivelCiudadPerdida::actualizarAtaques(double dt)
{
    for (Proyectil* roca : rocas) {
        roca->actualizar(dt);
        if (roca->estaActiva() && roca->colisionaCon(aj)) {
            roca->desactivar();
            danarApplejack(true);
        }
    }
    for (Proyectil* bola : bolas) {
        bola->actualizar(dt);
        if (bola->estaActiva() && bola->colisionaCon(aj)) {
            bola->desactivar();
            danarApplejack(false);
        }
    }
    borrarInactivas(rocas);
    borrarInactivas(bolas);
}

void NivelCiudadPerdida::danarApplejack(bool causadoPorRelojero)
{
    if (aj->esInvulnerable()) {
        return;
    }
    aj->recibirDanio(1);
    crearEfecto("efecto_golpe", aj->pos() + QPointF(0, -50), 60, 0.3, 1.5);
    if (causadoPorRelojero) {
        golpesAlJugador++;
    }
    juego->getSonido()->efecto("golpe");
    if (aj->estaColgada()) {
        aj->soltarse();
    }
    aj->movimiento.lanzar(aj->pos(), aj->miraDerecha() ? -240 : 240, -380);
    aj->enSuelo = false;
    if (!aj->estaVivo()) {
        mostrarMensaje("¡Applejack no se rinde! Vuelve al último tótem", 3);
        aj->setVida(aj->getVidaMax());
        aj->hacerInvulnerable(2.0);
        reaparecer(puntoControl);
    }
}

void NivelCiudadPerdida::reaparecer(QPointF lugar)
{
    if (aj->estaColgada()) {
        aj->soltarse();
    }
    aj->setPos(lugar + QPointF(0, -10));
    aj->movimiento.lanzar(aj->pos(), 0, 0);
    aj->enSuelo = false;
}

void NivelCiudadPerdida::revisarColisiones()
{
    for (Patrullero* secuaz : secuaces) {
        if (aj->colisionaCon(secuaz)) {
            danarApplejack(false);
        }
    }
    for (TrampaColgante* trampa : trampas) {
        if (aj->colisionaCon(trampa)) {
            danarApplejack(false);
        }
    }
    for (ObjetoFijo* pincho : pinchos) {
        if (aj->colisionaCon(pincho)) {
            danarApplejack(false);
        }
    }
    if (!maneiac->estaVencida() && aj->colisionaCon(maneiac)) {
        danarApplejack(false);
    }
    for (ObjetoFijo* totem : totems) {
        if (!totem->estaEncendido() && aj->colisionaCon(totem)) {
            totem->encender();
            puntoControl = totem->pos();
            aj->setVida(aj->getVidaMax());
            juego->getSonido()->efecto("recoger");
            mostrarMensaje("Tótem activado: punto de control", 2);
        }
    }
    for (Item* manzana : manzanas) {
        if (manzana->estaActiva() && aj->colisionaCon(manzana)) {
            aj->curar(1);
            manzana->desactivar();
            juego->getSonido()->efecto("recoger");
        }
    }
    borrarInactivas(manzanas);
    if (aj->y() > camara.bottom() + 200) {
        danarApplejack(false);
        reaparecer(ultimoSeguro);
    }
}

QRectF NivelCiudadPerdida::zonaRelojero() const
{
    if (faseJefe) {
        return QRectF(480, Y_TEMPLO - 10, 320, 20);
    }
    QPointF percha = perchas[perchaActual];
    return QRectF(percha.x() - 70, percha.y() - 10, 140, 20);
}

void NivelCiudadPerdida::actualizarRelojero(double dt)
{
    if (!faseJefe) {
        int mejor = perchaActual;
        double mejorDiferencia = 1e9;
        for (int i = 0; i < int(perchas.size()); i++) {
            double encima = aj->y() - perchas[i].y();
            if (encima > 180 && encima < 700 && qAbs(encima - 420) < mejorDiferencia) {
                mejorDiferencia = qAbs(encima - 420);
                mejor = i;
            }
        }
        if (mejor != perchaActual) {
            perchaActual = mejor;
            relojero->teletransportar(perchas[mejor]);
        }
    }

    QPointF paso = relojero->pasoHuida(dt);
    if (paso != QPointF(0, 0) && zonaRelojero().contains(relojero->pos() + paso)) {
        relojero->mover(paso, dt);
    } else {
        relojero->setVelocidad(QPointF(0, 0));
    }
    relojero->actualizar(dt);
    if (relojero->debeEscapar(aj->pos())) {
        teletransportarRelojero();
    }

    QRectF zona = zonaRelojero();
    DatosNivel datos;
    datos.posJugador = aj->pos();
    datos.jugadorEnAire = !aj->enSuelo;
    datos.relojeroEnBorde = relojero->x() < zona.left() + 90 || relojero->x() > zona.right() - 90;
    datos.golpesAlJugador = golpesAlJugador;
    datos.golpesAlRelojero = golpesAlRelojero;
    datos.progresoJugador = int(alturaMaxima / 300);
    for (int i = 0; i < NUM_ACCIONES; i++) {
        datos.disponible[i] = true;
    }
    datos.disponible[OBSTACULO] = false;
    for (TrampaColgante* trampa : trampas) {
        if (QLineF(trampa->pos(), aj->pos()).length() < 600) {
            datos.disponible[OBSTACULO] = true;
        }
    }
    if (relojero->pensar(dt, datos)) {
        ejecutarAccion(relojero->getAccion());
    }
}

void NivelCiudadPerdida::ejecutarAccion(Accion accion)
{
    if (accion == HUIR) {
        relojero->huirDe(aj->pos(), 1.5);
    } else if (accion == PROYECTIL) {
        relojero->lanzarHechizo();
        QPointF objetivo = aj->pos() + QPointF(azar(-config.errorPunteria, config.errorPunteria), -40 + azar(-config.errorPunteria, config.errorPunteria));
        Proyectil* roca = new Proyectil("roca", 46, relojero->pos() + QPointF(0, -20), objetivo, config.rapidezProyectil);
        agregarEntidad(roca);
        rocas.push_back(roca);
        relojero->activarEspera(PROYECTIL, faseJefe ? config.esperaProyectil : config.intervaloRocas);
    } else if (accion == OBSTACULO) {
        TrampaColgante* cercana = nullptr;
        double menor = 1e9;
        for (TrampaColgante* trampa : trampas) {
            double d = QLineF(trampa->pos(), aj->pos()).length();
            if (d < menor) {
                menor = d;
                cercana = trampa;
            }
        }
        if (cercana != nullptr) {
            relojero->lanzarHechizo();
            cercana->acelerar();
        }
        relojero->activarEspera(OBSTACULO, 6.0);
    } else if (accion == TELETRANSPORTE) {
        teletransportarRelojero();
        relojero->activarEspera(TELETRANSPORTE, 4.0);
    }
}

void NivelCiudadPerdida::teletransportarRelojero()
{
    if (faseJefe) {
        // Al extremo del templo que queda lejos de Applejack
        relojero->teletransportar(QPointF(aj->x() < 640 ? 780 : 500, Y_TEMPLO));
        return;
    }
    int mejor = perchaActual;
    double mejorDiferencia = 1e9;
    for (int i = 0; i < int(perchas.size()); i++) {
        bool otroLado = (perchas[i].x() < 640) != (perchas[perchaActual].x() < 640);
        double diferencia = qAbs(perchas[i].y() - perchas[perchaActual].y());
        if (otroLado && diferencia < mejorDiferencia && perchas[i].y() < aj->y() - 150) {
            mejorDiferencia = diferencia;
            mejor = i;
        }
    }
    if (mejor == perchaActual) {
        // Sin percha al otro lado por encima: la mas lejana de Applejack
        for (int i = 0; i < int(perchas.size()); i++) {
            if (QLineF(perchas[i], aj->pos()).length() > QLineF(perchas[mejor], aj->pos()).length()) {
                mejor = i;
            }
        }
    }
    perchaActual = mejor;
    relojero->teletransportar(perchas[mejor]);
}

void NivelCiudadPerdida::empezarJefe()
{
    faseJefe = true;
    relojero->teletransportar(QPointF(640, Y_TEMPLO));
    TrampaColgante* hacha1 = new TrampaColgante(QPointF(240, 0), 560, 40, "hacha", 70, 80, config.velocidadTrampas);
    TrampaColgante* hacha2 = new TrampaColgante(QPointF(1040, 0), 560, -40, "hacha", 70, 80, config.velocidadTrampas);
    agregarEntidad(hacha1);
    agregarEntidad(hacha2);
    trampas.push_back(hacha1);
    trampas.push_back(hacha2);
    mostrarMensaje("¡Lánzale manzanas al Relojero!", 4);
}

void NivelCiudadPerdida::actualizar(double dt)
{
    if (empezarCuadro(dt)) {
        moverApplejack(dt);
        aj->actualizar(dt);
        for (Plataforma* plataforma : plataformas) {
            plataforma->actualizar(dt);
        }
        for (Liana* liana : lianas) {
            liana->actualizar(dt);
        }
        for (TrampaColgante* trampa : trampas) {
            trampa->actualizar(dt);
        }
        for (Patrullero* secuaz : secuaces) {
            secuaz->actualizar(dt);
        }
        for (ObjetoFijo* muro : muros) {
            muro->actualizar(dt);
        }
        maneiac->actualizar(dt);
        actualizarTiros(dt);
        actualizarAtaques(dt);
        revisarColisiones();
        actualizarRelojero(dt);

        if (!maneiac->estaVencida() && QLineF(maneiac->pos(), aj->pos()).length() < 750 && maneiac->quiereLanzar()) {
            Proyectil* bola = new Proyectil("bola_cabello", 46, maneiac->pos() + QPointF(-40, -120), aj->pos() + QPointF(0, -40), config.rapidezProyectil);
            agregarEntidad(bola);
            bolas.push_back(bola);
        }
        if (!faseJefe && aj->enSuelo && aj->y() <= Y_CIMA + 1) {
            empezarJefe();
        }

        alturaMaxima = qMax(alturaMaxima, ALTO_CIUDAD - aj->y());
        if (faseJefe) {
            moverCamara(QPointF(ANCHO_PANTALLA / 2.0, ALTO_PANTALLA / 2.0));
        } else {
            moverCamara(QPointF(ANCHO_PANTALLA / 2.0, aj->y() - 120));
        }
        double recorrido = 1920 - ALTO_PANTALLA;
        fondo->setY(camara.top() - camara.top() / (ALTO_CIUDAD - ALTO_PANTALLA) * recorrido);
        actualizarHud();
        if (golpesAlRelojero >= 3) {
            ganar(1000 + aj->getVida() * 300);
        }
    }
    terminarCuadro(dt);
}

void NivelCiudadPerdida::actualizarHud()
{
    mostrarCorazones(aj->getVida());
    // El texto solo se arma de nuevo cuando cambia el numero que muestra
    int porcentaje = qBound(0, int(100 * (ALTO_CIUDAD - aj->y()) / (ALTO_CIUDAD - Y_CIMA)), 100);
    int valor = faseJefe ? 1000 + golpesAlRelojero : porcentaje;
    if (valor == valorHud) {
        return;
    }
    valorHud = valor;
    if (faseJefe) {
        textoAltura->setTexto("Relojero " + QString(3 - golpesAlRelojero, QChar(0x2665)) + QString(golpesAlRelojero, QChar(0x2661)));
    } else {
        textoAltura->setTexto("Altura " + QString::number(porcentaje) + "%");
    }
}
