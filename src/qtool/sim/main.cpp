#include <QtGui/QApplication>
#include "SimMain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtool::sim::SimMain simMain;
    simMain.show();
    return a.exec();
}
