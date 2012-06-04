
#include "SimView.h"
#include <vector>

namespace qtool {
namespace sim {

using namespace image;
using namespace viewer;

SimView::SimView(WorldModel* worldModel, QWidget* parent) :
    QWidget(parent) {

    PaintField* paintField = new PaintField(this);
    PaintWorld* paintWorld = new PaintWorld(worldModel, this);
    OverlayedImage* overlayedField = new OverlayedImage(paintField, paintWorld, this);
    fieldView = new BMPImageViewer(overlayedField, this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(fieldView);

    this->setLayout(layout);

    fieldView->updateView();
}


}
}
