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

class SimView: public QWidget{
    Q_OBJECT

public:
    SimView(WorldModel* worldModel, QWidget* parent = 0);

private:
    viewer::BMPImageViewer* fieldView;
};

}
}
