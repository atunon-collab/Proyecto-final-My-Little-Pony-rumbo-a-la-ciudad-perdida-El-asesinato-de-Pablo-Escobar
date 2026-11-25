#include "NivelMedellin.h"
#include "Juego.h"
#include "Sonido.h"
#include "Constantes.h"
#include "Twilight.h"
#include "Relojero.h"
#include "Objetos.h"
#include "Orbitales.h"
#include "Proyectiles.h"
#include "Textos.h"
#include "Recursos.h"
#include "Estilo.h"
#include <QRandomGenerator>
#include <QtMath>
#include <algorithm>

const double ANCHO_MEDELLIN = 3072;
const double ALTO_MEDELLIN = 2048;

struct DatosTecho
{
    double x, y, ancho, alto;
    const char* imagen;
};

static const DatosTecho TECHOS[] = {
    { 60, 60, 420, 376, "" },     { 540, 60, 340, 310, "" },    { 66, 524, 294, 396, "" },    { 444, 440, 440, 476, "" },
    { 1064, 60, 396, 430, "" },   { 1540, 60, 410, 340, "" },   { 1080, 560, 440, 380, "" },  { 1590, 484, 360, 452, "" },
    { 2120, 60, 460, 360, "" },   { 2660, 60, 360, 420, "" },   { 2120, 500, 340, 440, "" },  { 2540, 560, 480, 380, "" },
    { 60, 1100, 400, 420, "" },   { 536, 1104, 350, 366, "" },  { 60, 1600, 456, 380, "" },   { 580, 1544, 296, 436, "" },
    { 1080, 1100, 420, 380, "" }, { 1576, 1106, 434, 438, "" }, { 1064, 1566, 376, 414, "" }, { 1500, 1624, 440, 356, "" },
    { 2120, 1100, 360, 420, "" }, { 2560, 1100, 460, 360, "" }, { 2124, 1600, 500, 380, "" }, { 2680, 1540, 340, 440, "" },
    { 200, 900, 56, 220, "puente_v" },  { 1270, 920, 56, 200, "puente_v" },  { 2260, 920, 56, 200, "puente_v" },
    { 2780, 920, 56, 200, "puente_v" }, { 864, 690, 236, 56, "puente_h" },   { 1930, 660, 210, 56, "puente_h" },
    { 866, 1240, 234, 56, "puente_h" }, { 1990, 1260, 150, 56, "puente_h" }
};

static double azar(double minimo, double maximo)
{
    return minimo + QRandomGenerator::global()->generateDouble() * (maximo - minimo);
}

static double largo(QPointF v)
{
    return qSqrt(v.x() * v.x() + v.y() * v.y());
}

NivelMedellin::NivelMedellin(Juego* juego, MemoriaRelojero* memoria)
    : Nivel(juego, ANCHO_MEDELLIN, ALTO_MEDELLIN), twilight(nullptr), relojero(nullptr), portal(nullptr), recogidas(0),
      analizadas(0), tiempoRestante(0), entroPortal(false), granadaPendiente(0), tiempoFlecha(0), tiempoAyuda(9),
      hudSegundos(-1), hudRecogidas(-1), hudPorAnalizar(-1)
{
    cargar(memoria);
}

void NivelMedellin::cargar(MemoriaRelojero* memoria)
{
    tiempoRestante = config.tiempoPortal;
    juego->getSonido()->musica("medellin");
    QGraphicsPixmapItem* ciudad = escena->addPixmap(cargarImagen(":/fondos/medellin_mapa.png"));
    ciudad->setZValue(-50);

    for (const DatosTecho& d : TECHOS) {
        Techo* techo = new Techo(d.x, d.y, d.ancho, d.alto, d.imagen);
        agregarEntidad(techo);
        techos.push_back(techo);
    }

    zonasResbalosas = { QRectF(500, 520, 300, 280), QRectF(1640, 540, 260, 300), QRectF(600, 1600, 240, 300), QRectF(2180, 1640, 380, 280) };
    for (const QRectF& zona : zonasResbalosas) {
        agregarEntidad(new ObjetoFijo(OBJ_DECORADO, "tejas_sueltas", zona.center() + QPointF(-50, -40), 90, 62, false));
        agregarEntidad(new ObjetoFijo(OBJ_DECORADO, "tejas_sueltas", zona.center() + QPointF(60, 50), 80, 55, false));
    }

    struct Obstaculo { const char* imagen; double x, y, ancho, alto; };
    Obstaculo cosas[] = {
        { "casetica", 170, 1760, 90, 98 }, { "aire", 1420, 1200, 70, 60 }, { "chimenea", 820, 560, 56, 80 },
        { "tanque", 2400, 1200, 70, 92 }, { "antena", 2500, 150, 60, 100 }, { "casetica", 2960, 680, 90, 98 },
        { "aire", 1380, 160, 70, 60 }, { "chimenea", 1380, 1660, 56, 80 }, { "tanque", 330, 260, 70, 92 }
    };
    for (const Obstaculo& o : cosas) {
        ObjetoFijo* obstaculo = new ObjetoFijo(OBJ_OBSTACULO, o.imagen, QPointF(o.x, o.y), o.ancho, o.alto);
        agregarEntidad(obstaculo);
        obstaculos.push_back(obstaculo);
    }
    struct Adorno { const char* imagen; double x, y, tamano; };
    Adorno adornos[] = {
        { "macetas", 400, 1180, 60 }, { "caneca", 2700, 1420, 40 }, { "ladrillos", 820, 1420, 50 },
        { "satelite", 2420, 600, 55 }, { "matera", 1150, 1480, 40 }, { "ropa", 1700, 1900, 80 }
    };
    for (const Adorno& a : adornos) {
        agregarEntidad(new ObjetoFijo(OBJ_DECORADO, a.imagen, QPointF(a.x, a.y), a.tamano, a.tamano));
    }

    QPointF posicionesPruebas[] = { QPointF(720, 1300), QPointF(1720, 1820), QPointF(1745, 240), QPointF(2850, 1780) };
    for (int i = 0; i < 4; i++) {
        Prueba* prueba = new Prueba(i, imagenPrueba(i), posicionesPruebas[i]);
        agregarEntidad(prueba);
        pruebas.push_back(prueba);
        recogida.push_back(false);
    }

    QPointF posicionesCristales[] = { QPointF(240, 760), QPointF(1250, 300), QPointF(2350, 300), QPointF(1250, 1320), QPointF(2330, 1820), QPointF(720, 1820) };
    int cantidadCristales = qMin(6, qRound(4 * config.multObjetos));
    for (int i = 0; i < cantidadCristales; i++) {
        Item* cristal = new Item(ITEM_CRISTAL, posicionesCristales[i], true);
        agregarEntidad(cristal);
        cristales.push_back(cristal);
    }

    portal = new Portal(QPointF(2840, 280), 120, 8);
    agregarEntidad(portal);

    for (int i = 0; i < 4; i++) {
        int cantidad = config.anomaliasPorPrueba;
        if (config.dificultad == FACIL && i == 3) {
            cantidad = 1;
        }
        for (int k = 0; k < cantidad; k++) {
            crearAnomalia(i);
        }
    }

    twilight = new Twilight(config);
    twilight->setPos(260, 1330);
    twilight->setZValue(30);
    agregarEntidad(twilight);
    ultimoSeguro = twilight->pos();

    relojero = new Relojero(memoria, config);
    relojero->setPos(1300, 760);
    relojero->setZValue(29);
    agregarEntidad(relojero);

    crearCorazones(3, false);
    barraMagia = barraConIcono("icono_magia", 66, QColor(160, 110, 255));
    iconoHud("icono_tiempo", 540, 12, 46);
    textoTiempo = textoHud("", fuenteTitulo(30), Qt::white, 594, 22);
    textoPruebas = crearPlacaDerecha("icono_pruebas");
    textoAnalizar = textoHud("", fuenteTexto(21), CREMA, 935, 80);
    textoAyuda = textoHud("WASD moverse   Espacio saltar   E teletransporte   Q analizar   Esc pausa",
                          fuenteTexto(21), QColor(255, 245, 210), 215, 680);
    flecha = escena->addPixmap(cargarImagen(":/objetos/icono_flecha.png", 56, 40));
    flecha->setOffset(-28, -20);
    flecha->setVisible(false);
    flecha->setZValue(60);
    mostrarMensaje("¡Reúne las 4 pruebas y entra al portal antes de que se cierre!", 4);
    moverCamara(twilight->pos());
}

void NivelMedellin::crearAnomalia(int indicePrueba)
{
    QPointF centro = pruebas[indicePrueba]->pos() + QPointF(0, -20);
    double sentido = QRandomGenerator::global()->bounded(2) == 0 ? 1 : -1;
    double omega = azar(1.6, 2.6) * config.velocidadAnomalias * sentido;
    AnomaliaTemporal* anomalia = new AnomaliaTemporal(centro, azar(60, 100), omega, azar(0, 360), 40);
    agregarEntidad(anomalia);
    AnomaliaDePrueba registro;
    registro.anomalia = anomalia;
    registro.prueba = indicePrueba;
    anomalias.push_back(registro);
}

void NivelMedellin::teclaPresionada(int tecla)
{
    Nivel::teclaPresionada(tecla);
    if (!enCurso()) {
        return;
    }
    if (tecla == Qt::Key_Space) {
        if (!twilight->estaEnElAire() && twilight->congelada <= 0) {
            twilight->saltar();
        }
    } else if (tecla == Qt::Key_E) {
        if (twilight->puedeTeletransportarse()) {
            QPointF direccion(0, 0);
            if (estaPresionada(Qt::Key_A)) direccion.rx() -= 1;
            if (estaPresionada(Qt::Key_D)) direccion.rx() += 1;
            if (estaPresionada(Qt::Key_W)) direccion.ry() -= 1;
            if (estaPresionada(Qt::Key_S)) direccion.ry() += 1;
            if (direccion == QPointF(0, 0)) {
                direccion = QPointF(twilight->miraDerecha() ? 1 : -1, 0);
            }
            teletransportar(direccion / largo(direccion));
        }
    } else if (tecla == Qt::Key_Q) {
        if (porAnalizar.empty()) {
            mostrarMensaje("No hay pruebas por analizar", 1.5);
        } else if (!twilight->estaEnElAire() && twilight->analizando <= 0) {
            twilight->analizando = 1.5;
            mostrarMensaje("Analizando " + nombrePrueba(porAnalizar.front()) + "... (no te muevas)", 1.5);
        }
    }
}

void NivelMedellin::teletransportar(QPointF direccion)
{
    QPointF antes = twilight->pos();
    QPointF destino = antes + direccion * 160;
    destino.setX(qBound(20.0, destino.x(), ANCHO_MEDELLIN - 20));
    destino.setY(qBound(40.0, destino.y(), ALTO_MEDELLIN - 20));
    twilight->setPos(destino);
    if (chocaObstaculo(twilight)) {
        twilight->setPos(antes);
        return;
    }
    twilight->terminarTeletransporte();
    crearEfecto("efecto_teleport", antes + QPointF(0, -40), 90, 0.45, 1.4);
    crearEfecto("efecto_teleport", destino + QPointF(0, -40), 90, 0.45, 1.4);
}

bool NivelMedellin::sobreTecho(Entidad* entidad) const
{
    for (Techo* techo : techos) {
        if (entidad->colisionaCon(techo)) {
            return true;
        }
    }
    return false;
}

bool NivelMedellin::chocaObstaculo(Entidad* entidad) const
{
    for (ObjetoFijo* obstaculo : obstaculos) {
        if (entidad->colisionaCon(obstaculo)) {
            return true;
        }
    }
    return false;
}

void NivelMedellin::moverTwilight(double dt)
{
    QPointF direccion(0, 0);
    if (estaPresionada(Qt::Key_A)) direccion.rx() -= 1;
    if (estaPresionada(Qt::Key_D)) direccion.rx() += 1;
    if (estaPresionada(Qt::Key_W)) direccion.ry() -= 1;
    if (estaPresionada(Qt::Key_S)) direccion.ry() += 1;
    if (direccion.x() != 0 && direccion.y() != 0) {
        direccion *= 0.7071;
    }

    if (twilight->congelada > 0) {
        twilight->setVelocidad(QPointF(0, 0));
        return;
    }
    if (twilight->analizando > 0) {
        if (direccion != QPointF(0, 0)) {
            twilight->analizando = 0;
            mostrarMensaje("Análisis interrumpido", 1.2);
        } else {
            twilight->analizando -= dt;
            if (twilight->analizando <= 0) {
                terminarAnalisis();
            }
            twilight->setVelocidad(QPointF(0, 0));
            return;
        }
    }

    double rapidez = twilight->getRapidez();
    for (const QRectF& zona : zonasResbalosas) {
        if (zona.contains(twilight->pos())) {
            rapidez = rapidez / 2;
        }
    }
    QPointF velocidad = direccion * rapidez;
    twilight->setVelocidad(velocidad);

    QPointF inicio = twilight->pos();
    twilight->setX(inicio.x() + velocidad.x() * dt);
    if (chocaObstaculo(twilight)) {
        twilight->setX(inicio.x());
    }
    twilight->setY(inicio.y() + velocidad.y() * dt);
    if (chocaObstaculo(twilight)) {
        twilight->setY(inicio.y());
    }

    if (twilight->estaEnElAire()) {
        return;
    }
    if (!sobreTecho(twilight)) {
        crearEfecto("efecto_polvo", twilight->pos(), 90, 0.5, 1.8);
        twilight->setVida(twilight->getVida() - 1);
        twilight->hacerInvulnerable(1.5);
        twilight->setPos(ultimoSeguro);
        twilight->setVelocidad(QPointF(0, 0));
        twilight->congelada = 0.6;
        juego->getSonido()->efecto("golpe");
        mostrarMensaje("¡Twilight cayó a la calle!", 1.8);
        return;
    }
    for (Techo* techo : techos) {
        if (techo->getRect().adjusted(45, 45, -45, -15).contains(twilight->pos())) {
            ultimoSeguro = twilight->pos();
            break;
        }
    }
}

void NivelMedellin::terminarAnalisis()
{
    int indice = porAnalizar.front();
    porAnalizar.erase(porAnalizar.begin());
    analizadas++;
    tiempoRestante = qMin(tiempoRestante + 6, config.tiempoPortal);
    tiempoFlecha = 6;
    juego->getSonido()->efecto("recoger");
    mostrarMensaje(analisisPrueba(indice) + " (+6 s)", 4);
}

void NivelMedellin::revisarColisiones(double dt)
{
    bool enElPiso = twilight->getAltura() < 25;

    for (Prueba* prueba : pruebas) {
        int i = prueba->getIndice();
        if (!recogida[i] && enElPiso && twilight->colisionaCon(prueba)) {
            recogida[i] = true;
            recogidas++;
            porAnalizar.push_back(i);
            prueba->setVisible(false);
            crearEfecto("efecto_brillo", prueba->pos() + QPointF(0, -30), 70, 0.6, 2.0);
            juego->getSonido()->efecto("recoger");
            mostrarMensaje("Prueba recogida: " + nombrePrueba(i) + "  (Q para analizarla)", 3);
            for (AnomaliaDePrueba& registro : anomalias) {
                if (registro.prueba == i) {
                    registro.anomalia->desactivar();
                    registro.anomalia->setVisible(false);
                }
            }
            if (recogidas == 4) {
                portal->setListo(true);
                mostrarMensaje("¡Tienes las 4 pruebas! Corre al portal (arriba a la derecha)", 4);
            }
        }
    }

    // Las anomalias de las pruebas ya recogidas se borran: antes se seguian moviendo invisibles en cada cuadro
    auto fin = std::remove_if(anomalias.begin(), anomalias.end(), [](const AnomaliaDePrueba& registro) {
        if (registro.anomalia->estaActiva()) {
            return false;
        }
        delete registro.anomalia;
        return true;
    });
    anomalias.erase(fin, anomalias.end());

    for (Item* cristal : cristales) {
        if (cristal->estaActiva() && enElPiso && twilight->colisionaCon(cristal)) {
            twilight->magia = qMin(100.0, twilight->magia + 40);
            cristal->desactivar();
            crearEfecto("efecto_brillo", cristal->pos() + QPointF(0, -25), 55, 0.5, 2.0);
            juego->getSonido()->efecto("recoger");
        }
    }
    borrarInactivas(cristales);

    for (AnomaliaDePrueba& registro : anomalias) {
        AnomaliaTemporal* anomalia = registro.anomalia;
        if (anomalia->estaActiva() && anomalia->espera <= 0 && enElPiso && twilight->colisionaCon(anomalia)) {
            twilight->setPos(twilight->pos() + anomalia->empujeTangente() * 70);
            anomalia->espera = 1.0;
            crearEfecto("efecto_golpe", twilight->pos() + QPointF(0, -40), 55, 0.3, 1.5);
            golpesAlJugador++;
            juego->getSonido()->efecto("golpe");
        }
    }

    for (Proyectil* granada : granadas) {
        granada->actualizar(dt);
        if (granada->estaActiva() && twilight->getAltura() < 30 && twilight->colisionaCon(granada)) {
            granada->desactivar();
            crearEfecto("efecto_explosion", granada->pos(), 130, 0.45, 1.4);
            if (twilight->congelar(config.congelamiento)) {
                twilight->analizando = 0;
                golpesAlJugador++;
                juego->getSonido()->efecto("golpe");
            }
        }
    }
    borrarInactivas(granadas);

    if (recogidas == 4 && twilight->colisionaCon(portal)) {
        entroPortal = true;
    }
}

bool NivelMedellin::posicionValidaRelojero(QPointF posicion) const
{
    for (ObjetoFijo* obstaculo : obstaculos) {
        if (obstaculo->getCaja().contains(posicion)) {
            return false;
        }
    }
    for (Techo* techo : techos) {
        if (techo->getRect().adjusted(15, 15, -15, -15).contains(posicion)) {
            return true;
        }
    }
    return false;
}

void NivelMedellin::lanzarGranada(QPointF objetivo)
{
    QPointF error(azar(-config.errorPunteria, config.errorPunteria), azar(-config.errorPunteria, config.errorPunteria));
    Proyectil* granada = new Proyectil("cronogranada", 34, relojero->pos() + QPointF(0, -30), objetivo + error, config.rapidezProyectil);
    agregarEntidad(granada);
    granadas.push_back(granada);
}

void NivelMedellin::actualizarRelojero(double dt)
{
    QPointF paso = relojero->pasoHuida(dt);
    if (paso != QPointF(0, 0) && posicionValidaRelojero(relojero->pos() + paso)) {
        relojero->mover(paso, dt);
    } else {
        relojero->setVelocidad(QPointF(0, 0));
    }
    relojero->actualizar(dt);

    if (granadaPendiente > 0) {
        granadaPendiente -= dt;
        if (granadaPendiente <= 0) {
            lanzarGranada(objetivoPendiente);
        }
    }

    DatosNivel datos;
    datos.posJugador = twilight->pos();
    datos.jugadorEnAire = twilight->estaEnElAire();
    datos.relojeroEnBorde = relojero->x() < 90 || relojero->x() > anchoMundo - 90 || relojero->y() < 90 || relojero->y() > altoMundo - 90;
    datos.golpesAlJugador = golpesAlJugador;
    datos.golpesAlRelojero = golpesAlRelojero;
    datos.progresoJugador = recogidas;
    for (int i = 0; i < NUM_ACCIONES; i++) {
        datos.disponible[i] = true;
    }
    datos.disponible[OBSTACULO] = false;
    for (int i = 0; i < 4; i++) {
        int cantidad = 0;
        for (const AnomaliaDePrueba& registro : anomalias) {
            if (registro.prueba == i && registro.anomalia->estaActiva()) {
                cantidad++;
            }
        }
        if (!recogida[i] && cantidad < config.anomaliasPorPrueba + 2) {
            datos.disponible[OBSTACULO] = true;
        }
    }
    if (relojero->pensar(dt, datos)) {
        ejecutarAccion(relojero->getAccion());
    }
}

void NivelMedellin::ejecutarAccion(Accion accion)
{
    if (accion == HUIR) {
        relojero->huirDe(twilight->pos(), 1.5);
    } else if (accion == PROYECTIL) {
        relojero->lanzarHechizo();
        lanzarGranada(twilight->pos());
        if (config.proyectilDoble) {
            granadaPendiente = 0.35;
            objetivoPendiente = twilight->pos() + QPointF(azar(-70, 70), azar(-70, 70));
        }
        relojero->activarEspera(PROYECTIL, config.esperaProyectil);
    } else if (accion == OBSTACULO) {
        int mejor = -1;
        double mejorDistancia = 1e9;
        for (int i = 0; i < 4; i++) {
            double d = largo(pruebas[i]->pos() - twilight->pos());
            if (!recogida[i] && d < mejorDistancia) {
                mejorDistancia = d;
                mejor = i;
            }
        }
        if (mejor >= 0) {
            relojero->lanzarHechizo();
            crearAnomalia(mejor);
        }
        relojero->activarEspera(OBSTACULO, 6.0);
    } else if (accion == TELETRANSPORTE) {
        int cantidadTechos = int(sizeof(TECHOS) / sizeof(TECHOS[0]));
        for (int intento = 0; intento < 40; intento++) {
            const DatosTecho& d = TECHOS[QRandomGenerator::global()->bounded(cantidadTechos)];
            QPointF destino(azar(d.x + 30, d.x + d.ancho - 30), azar(d.y + 40, d.y + d.alto - 20));
            double distancia = largo(destino - twilight->pos());
            if (distancia > 350 && distancia < 700 && posicionValidaRelojero(destino)) {
                crearEfecto("efecto_teleport", relojero->pos() + QPointF(0, -40), 90, 0.45, 1.4);
                relojero->teletransportar(destino);
                crearEfecto("efecto_teleport", destino + QPointF(0, -40), 90, 0.45, 1.4);
                break;
            }
        }
        relojero->activarEspera(TELETRANSPORTE, 4.0);
    }
}

void NivelMedellin::apuntarFlecha(double dt)
{
    QPointF destino;
    int siguiente = -1;
    for (int i = 3; i >= 0; i--) {
        if (!recogida[i]) {
            siguiente = i;
        }
    }
    if (siguiente >= 0) {
        destino = pruebas[siguiente]->pos();
    } else {
        destino = portal->pos();
    }
    if (tiempoFlecha <= 0) {
        flecha->setVisible(false);
        return;
    }
    tiempoFlecha -= dt;
    QPointF hacia = destino - twilight->pos();
    double distancia = qMax(1.0, largo(hacia));
    flecha->setPos(twilight->pos() + hacia / distancia * 80 + QPointF(0, -50));
    flecha->setRotation(qRadiansToDegrees(qAtan2(hacia.y(), hacia.x())));
    flecha->setVisible(true);
}

void NivelMedellin::actualizar(double dt)
{
    if (empezarCuadro(dt)) {
        tiempoRestante -= dt;
        if (tiempoAyuda > 0) {
            tiempoAyuda -= dt;
            textoAyuda->setVisible(tiempoAyuda > 0);
        }
        moverTwilight(dt);
        twilight->actualizar(dt);
        portal->actualizar(dt);
        for (Prueba* prueba : pruebas) {
            if (!recogida[prueba->getIndice()]) {
                prueba->actualizar(dt);
            }
        }
        for (Item* cristal : cristales) {
            cristal->actualizar(dt);
        }
        for (AnomaliaDePrueba& registro : anomalias) {
            registro.anomalia->actualizar(dt);
        }
        revisarColisiones(dt);
        actualizarRelojero(dt);
        apuntarFlecha(dt);
        moverCamara(twilight->pos() + QPointF(0, -40));
        actualizarHud();

        if (entroPortal) {
            ganar(int(tiempoRestante) * 10 + analizadas * 200);
        } else if (tiempoRestante <= 0) {
            perder("¡El portal se cerró!");
        } else if (!twilight->estaVivo()) {
            perder("¡Twilight se quedó sin corazones!");
        }
    }
    terminarCuadro(dt);
}

void NivelMedellin::actualizarHud()
{
    mostrarCorazones(twilight->getVida());
    barraMagia->setRect(0, 0, 230 * twilight->magia / 100.0, 12);
    // Los textos solo se arman de nuevo cuando cambia el numero (el reloj cambia una vez por segundo, no 60)
    int segundos = qMax(0, int(tiempoRestante));
    if (segundos != hudSegundos) {
        hudSegundos = segundos;
        textoTiempo->setTexto(QString::number(segundos / 60) + ":" + QString("%1").arg(segundos % 60, 2, 10, QChar('0')));
        textoTiempo->setColor(tiempoRestante < 25 ? QColor(255, 80, 80) : QColor(Qt::white));
    }
    if (recogidas != hudRecogidas) {
        hudRecogidas = recogidas;
        textoPruebas->setTexto("Pruebas " + QString::number(recogidas) + "/4");
    }
    int pendientes = int(porAnalizar.size());
    if (pendientes != hudPorAnalizar) {
        hudPorAnalizar = pendientes;
        textoAnalizar->setTexto(pendientes == 0 ? "" : "Q: " + QString::number(pendientes) + " por analizar");
    }
}
