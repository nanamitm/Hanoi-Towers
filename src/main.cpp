#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Hanoi Towers Slideshow");
    QApplication::setOrganizationName("Codex");

    MainWindow window;
    window.show();

    return app.exec();
}
