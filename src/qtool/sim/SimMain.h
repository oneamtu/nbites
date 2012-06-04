
#pragma once

#include <QMainWindow>

#include "WorldModel.h"
#include "SimView.h"

namespace qtool {
namespace sim {

class SimMain : public QMainWindow {

    Q_OBJECT

public:
    SimMain();

private:
    WorldModel worldModel;
    SimView simView;

};

}
}
