
#include "SimMain.h"

namespace qtool {
namespace sim {

SimMain::SimMain() :
        worldModel(), simView(&worldModel, this) {

    this->setWindowTitle("Frozen");

    this->setCentralWidget(&simView);

}

}
}
