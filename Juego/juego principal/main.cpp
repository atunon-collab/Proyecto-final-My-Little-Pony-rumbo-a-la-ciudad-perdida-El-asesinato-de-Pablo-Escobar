#include <QApplication>
#include <QMainWindow>
#include "Constantes.h"

int main(int argc, char* argv[])
{
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    QApplication app(argc, argv);

    QMainWindow ventana;
    ventana.setWindowTitle("Rumbo a la Ciudad Perdida: El Asesinato de Pablo Escobar");
    ventana.setFixedSize(ANCHO_PANTALLA, ALTO_PANTALLA);
    ventana.show();
    return app.exec();
}
