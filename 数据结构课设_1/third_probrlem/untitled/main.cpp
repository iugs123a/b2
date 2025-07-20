#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("交通咨询系统");
    w.resize(800, 600);
    w.show();
    return app.exec();
}
