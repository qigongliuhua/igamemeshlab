#include "iGameMeshLab.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    iGameMeshLab w;
    w.show();
    return a.exec();
}
