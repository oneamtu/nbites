
#include "SimView.h"
#include <vector>

namespace qtool {
namespace sim {

using namespace image;
using namespace viewer;

SimView::SimView(WorldModel* worldModel) {

    this->setWindowTitle(tr("Frozen"));

    PaintField* paintField = new PaintField(this);
    PaintWorld* paintWorld = new PaintWorld(worldModel, this);
    OverlayedImage* overlayedField = new OverlayedImage(paintField, paintWorld, this);
    fieldView = new BMPImageViewer(overlayedField, this);

    // Adds the Field
    this->setCentralWidget(fieldView);

    fieldView->updateView();
}



}
}
