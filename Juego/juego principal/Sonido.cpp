#include "Sonido.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QUrl>
#include <QFile>

Sonido::Sonido()
{
    reproductorMusica = new QMediaPlayer();
    salidaMusica = new QAudioOutput();
    salidaMusica->setVolume(0.45);
    reproductorMusica->setAudioOutput(salidaMusica);
    reproductorMusica->setLoops(QMediaPlayer::Infinite);

    const char* nombres[] = { "golpe", "recoger", "victoria" };
    for (const char* nombre : nombres) {
        QSoundEffect* efecto = new QSoundEffect();
        efecto->setSource(QUrl(QString("qrc:/sonidos/") + nombre + ".wav"));
        efecto->setVolume(0.6);
        efectos[nombre] = efecto;
    }
}

Sonido::~Sonido()
{
    for (auto& par : efectos) {
        delete par.second;
    }
    delete reproductorMusica;
    delete salidaMusica;
}

void Sonido::musica(const QString& nombre)
{
    if (nombre == musicaActual) {
        return;
    }
    musicaActual = nombre;
    reproductorMusica->stop();
    const char* extensiones[] = { ".mp3", ".ogg", ".wav" };
    for (const char* extension : extensiones) {
        QString ruta = ":/sonidos/musica_" + nombre + extension;
        if (QFile::exists(ruta)) {
            reproductorMusica->setSource(QUrl("qrc" + ruta));
            reproductorMusica->play();
            return;
        }
    }
}

void Sonido::detenerMusica()
{
    musicaActual = "";
    reproductorMusica->stop();
}

void Sonido::efecto(const QString& nombre)
{
    auto encontrado = efectos.find(nombre);
    if (encontrado != efectos.end()) {
        encontrado->second->play();
    }
}
