#ifndef ERRORJUEGO_H
#define ERRORJUEGO_H

#include <QString>

// Excepcion propia del juego 
class ErrorJuego
{
public:
    ErrorJuego(const QString& mensaje) : mensaje(mensaje) {}
    QString getMensaje() const { return mensaje; }

private:
    QString mensaje;
};

#endif
