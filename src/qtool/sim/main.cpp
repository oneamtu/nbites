#include <QtGui/QApplication>
#include "WorldModel.h"
#include "SimView.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtool::sim::WorldModel worldModel;
    qtool::sim::SimView simView(&worldModel);
    simView.show();
    return a.exec();
}
