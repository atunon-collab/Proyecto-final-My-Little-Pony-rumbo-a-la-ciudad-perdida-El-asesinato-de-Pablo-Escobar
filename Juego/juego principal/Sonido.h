#ifndef SONIDO_H
#define SONIDO_H

#include <QString>
#include <map>

class QMediaPlayer;
class QAudioOutput;
class QSoundEffect;

// Musica, efectos y voces
class Sonido
{
public:
    Sonido();
    ~Sonido();

    void musica(const QString& nombre);
    void detenerMusica();
    void efecto(const QString& nombre);
    void voz(const QString& nombre);

private:
    QMediaPlayer* reproductorMusica;
    QAudioOutput* salidaMusica;
    QMediaPlayer* reproductorVoz;
    QAudioOutput* salidaVoz;
    std::map<QString, QSoundEffect*> efectos;
    QString musicaActual;
};

#endif
