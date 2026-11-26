#ifndef SONIDO_H
#define SONIDO_H

#include <QString>
#include <map>

class QMediaPlayer;
class QAudioOutput;
class QSoundEffect;

// Musica y efectos
class Sonido
{
public:
    Sonido();
    ~Sonido();

    void musica(const QString& nombre);
    void efecto(const QString& nombre);

private:
    QMediaPlayer* reproductorMusica;
    QAudioOutput* salidaMusica;
    std::map<QString, QSoundEffect*> efectos;
    QString musicaActual;
};

#endif
