#include <QApplication>
#include <QMessageBox>
#include "Juego.h"
#include "ErrorJuego.h"
#include "Estilo.h"

int main(int argc, char* argv[])
{
    // El juego se dibuja en pixeles reales (1280x720) aunque Windows tenga la pantalla escalada
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    QApplication app(argc, argv);
    try {
        cargarFuentes();
        Juego juego;
        juego.show();
        return app.exec();
    } catch (const ErrorJuego& error) {
        QMessageBox::critical(nullptr, "Error", error.getMensaje());
        return 1;
    }
}
