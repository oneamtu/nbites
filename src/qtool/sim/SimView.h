/**
 * @class SimView
 *
 * Draws the world model of the simulator
 */

#pragma once

#include <QWidget>
#include <QtGui>
#include <vector>

#include "image/PaintField.h"
#include "viewer/BMPImageViewer.h"

#include "WorldModel.h"
#include "PaintWorld.h"

namespace qtool {
namespace sim {

class SimView: public QMainWindow{
    Q_OBJECT

public:
    SimView(WorldModel* worldModel);

private:
    viewer::BMPImageViewer* fieldView;
};

}
}
